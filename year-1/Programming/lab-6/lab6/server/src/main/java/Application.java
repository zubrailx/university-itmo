import collection.CollectionManager;
import collection.Dragon;
import commands.CommandInvoker;
import commands.abstr.CommandContainer;
import file.FileManager;
import file.XmlParser;
import io.UserIO;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.*;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Application {

    CollectionManager collectionManager;

    FileManager fileManager;

    XmlParser xmlParser;

    UserIO userIO;

    CommandInvoker commandInvoker;

    ServerConnection serverConnection;

    private boolean isConnected;

    private static final Logger rootLogger = LogManager.getRootLogger();

    Application() {
        collectionManager = new CollectionManager();
        fileManager = new FileManager();
        xmlParser = new XmlParser();
        userIO = new UserIO();
        rootLogger.info("Конструктор класса Application был загружен.");
    }

    public void start(String inputFile) throws IOException, ParserConfigurationException, SAXException {

        try {
            File ioFile = new File(inputFile);
            if (!ioFile.canWrite() || ioFile.isDirectory() || !ioFile.isFile()) throw new IOException();
            String file = fileManager.readFromFile(inputFile);

            Dragon[] dragons = xmlParser.parseToCollection(new InputSource(new StringReader(file)));
            for (Dragon dragon : dragons) collectionManager.insert(dragon.getId(), dragon, System.out);

            this.commandInvoker = new CommandInvoker(collectionManager, inputFile);

            rootLogger.printf(Level.INFO, "Элементы коллекций из файла %1$s были загружены.", inputFile);


            serverConnection = new ServerConnection();//здесь хранится datagramSocket сервера.

            Scanner scanner = new Scanner(System.in);

            do {
                System.out.print("Введите порт: ");
                int port = scanner.nextInt();
                if (port <= 0){
                    rootLogger.error("Введенный порт невалиден.");
                }
                else{
                    isConnected = serverConnection.createFromPort(port);//создание datagramSocket для сервера
                }
            } while (!isConnected);
            rootLogger.info("Порт установлен.");
        }catch(NoSuchElementException ex){
            rootLogger.error("Аварийное завершение работы");
            System.exit(-1);
        }
        try {
            cycle(commandInvoker);
        } catch (NoSuchElementException | InterruptedException ex) {
            rootLogger.warn(ex.getMessage());
            rootLogger.warn("Работа сервера завершена.");
        }
    }

    private void cycle(CommandInvoker commandInvoker) throws InterruptedException {

        RequestReader requestReader = new RequestReader(serverConnection.getServerSocket()); //создание читателя запросов

        ResponseSender responseSender = new ResponseSender(serverConnection.getServerSocket()); //создание отправителя запросов

        CommandProcessor commandProcessor = new CommandProcessor(commandInvoker);//создание объекта, работающего с командами

        while (isConnected) {
            try {
                requestReader.readCommand();
                CommandContainer command = requestReader.getCommandContainer();

                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                PrintStream printStream = new PrintStream(baos);

                commandProcessor.executeCommand(command, printStream);

                Thread.sleep(1000);
                responseSender.send(baos.toString(), requestReader.getSenderAddress(), requestReader.getSenderPort());
                rootLogger.info("Пакет был отправлен " + requestReader.getSenderAddress().getHostAddress() + " " + requestReader.getSenderPort());

            } catch (IOException ex) {
                rootLogger.warn("Произошла ошибка при чтении: " + ex.getMessage());
            } catch (ClassNotFoundException ex) {
                rootLogger.error("Неизвестная ошибка: " + ex);
            }
        }
    }

    public CollectionManager getCollectionManager() {
        return collectionManager;
    }
}
