package ru.nkulakov;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.collection.CollectionManagerImpl;
import ru.nkulakov.collection.Dragon;
import ru.nkulakov.command.CommandProcessor;
import ru.nkulakov.command.CommandProcessorImpl;
import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.commands.CommandInvokerImpl;
import ru.nkulakov.connection.ServerConnection;
import ru.nkulakov.connection.ServerConnectionImpl;
import ru.nkulakov.database.*;
import ru.nkulakov.reader.RequestReader;
import ru.nkulakov.reader.RequestReaderImpl;
import ru.nkulakov.sender.ResponseSender;
import ru.nkulakov.sender.ResponseSenderImpl;
import ru.nkulakov.server.Server;
import ru.nkulakov.server.ServerImpl;

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
        UserDatabaseHandler udh = new UserDatabaseHandlerImpl(dbConnection);
        CollectionDatabaseHandler cdh = new CollectionDatabaseHandlerImpl(dbConnection);

        try {
            Dragon[] dragons = cdh.loadInMemory();
            CollectionManager collectionManager = new CollectionManagerImpl(dragons);
            rootLogger.info("Коллекция была загружена из бд.");
            Lock locker = new ReentrantLock();
            CommandInvoker commandInvoker = new CommandInvokerImpl(collectionManager, cdh, locker);
            rootLogger.info("Класс ru.nkulakov.Application готов.");

            ServerConnection serverConnection = new ServerConnectionImpl();//здесь хранится datagramSocket сервера.
            serverConnection.createFromPort(port);

            RequestReader requestReader = new RequestReaderImpl(serverConnection.getServerSocket());
            ResponseSender responseSender = new ResponseSenderImpl(serverConnection.getServerSocket());
            CommandProcessor commandProcessor = new CommandProcessorImpl(udh, cdh, commandInvoker);

            Server server = new ServerImpl(requestReader, responseSender, commandProcessor);
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
        DatabaseConnection databaseConnection = new DatabaseConnectionImpl(jdbcLocalURL, login, password);
        try {
            dbConnection = databaseConnection.connectToDatabase();
            rootLogger.info("Соединение с бд установлено.");
        } catch (SQLException ex) {
            rootLogger.error("Соединение с бд не установлено. Завершение работы сервера");
            System.exit(-1);
        }
    }
}
