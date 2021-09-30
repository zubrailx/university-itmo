import collection.CollectionManager;
import collection.Dragon;
import commands.CommandInvoker;
import database.CollectionDatabaseHandler;
import database.DatabaseConnection;
import database.UserDatabaseHandler;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.net.SocketException;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Application {
    private static final Logger rootLogger = LogManager.getRootLogger();

    private Connection dbConnection;

    public void start(int port) {
        this.createDatabaseConnection();
        UserDatabaseHandler udh = new UserDatabaseHandler(dbConnection);
        CollectionDatabaseHandler cdh = new CollectionDatabaseHandler(dbConnection);

        try {
            Dragon[] dragons = cdh.loadInMemory();
            CollectionManager collectionManager = new CollectionManager(dragons);
            rootLogger.info("Коллекция была загружена из бд.");
            Lock locker = new ReentrantLock();
            CommandInvoker commandInvoker = new CommandInvoker(collectionManager, cdh, locker);
            rootLogger.info("Класс Application готов.");

            ServerConnection serverConnection = new ServerConnection();//здесь хранится datagramSocket сервера.
            serverConnection.createFromPort(port);

            RequestReader requestReader = new RequestReader(serverConnection.getServerSocket());
            ResponseSender responseSender = new ResponseSender(serverConnection.getServerSocket());
            CommandProcessor commandProcessor = new CommandProcessor(udh, cdh, commandInvoker);

            Server server = new Server(requestReader, responseSender, commandProcessor);
            new Thread(server).start();

        } catch (SocketException ex) {
            rootLogger.error("Сокет не может быть открыт с указанным портом:" + port);
            System.exit(-1);
        } catch (SQLException ex) {
            System.out.println("Ошибка при загрузке коллекции в память. Завершение работы сервера.");
            System.exit(-10);
        } catch (IOException ex) {
            rootLogger.error(ex.getClass());
            ex.printStackTrace();
            System.exit(-15);
        }
    }

    private void createDatabaseConnection() {
        Scanner scanner = new Scanner(System.in);
        String jdbcHeliosURL = "jdbc:postgresql://pg:5432/studs";
        String jdbcLocalURL = "jdbc:postgresql://localhost:3330/studs";
        String login = "";
        String password = "";

        try {
            scanner = new Scanner(new FileReader("credentials.txt"));
        } catch (FileNotFoundException ex) {
            rootLogger.error("Не найден файл credentials.txt с данными для входа. Завершение работы.");
            System.exit(-1);
        }
        try {
            login = scanner.nextLine().trim();
            password = scanner.nextLine().trim();
        } catch (NoSuchElementException ex) {
            rootLogger.error("Не найдены данные для входа. Завершение работы.");
            System.exit(-1);
        }
        DatabaseConnection databaseConnection = new DatabaseConnection(jdbcLocalURL, login, password);
        try {
            dbConnection = databaseConnection.connectToDatabase();
            rootLogger.info("Соединение с бд установлено.");
        } catch (SQLException ex) {
            rootLogger.error("Соединение с бд не установлено. Завершение работы сервера");
            System.exit(-1);
        }
    }
}
