package ru.nkulakov.backend.client;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.backend.command.CommandProcessor;
import ru.nkulakov.backend.encryptor.DataEncryptor;
import ru.nkulakov.backend.encryptor.DataEncryptorImpl;
import ru.nkulakov.backend.reader.RequestReader;
import ru.nkulakov.backend.sender.ResponseSender;
import ru.nkulakov.collection.ClientCollection;
import ru.nkulakov.collection.ClientCollectionImpl;
import ru.nkulakov.database.UserData;
import ru.nkulakov.io.UserIO;

import java.io.*;
import java.net.BindException;
import java.net.InetSocketAddress;
import java.net.PortUnreachableException;
import java.net.SocketTimeoutException;
import java.nio.channels.ClosedChannelException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Scanner;

public class ClientImpl implements Client {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final RequestReader requestReader;
    private final ResponseSender responseSender;
    private final CommandProcessor commandProcessor;

    private InetSocketAddress isa = new InetSocketAddress("localhost", 20); //for test***

    private ClientCollection clientCollection;

    private UserData userData;

    public ClientImpl(RequestReader rr, ResponseSender rs, CommandProcessor cp) {
        this.requestReader = rr;
        this.responseSender = rs;
        this.commandProcessor = cp;
    }

    public int authenticate(String login, String password, boolean isNewUser) {
//        try {
//            Scanner scanner = new Scanner(new File("C:\\Users\\nikit\\OneDrive\\Desktop\\credentials.txt"));
//            scanner.nextLine();
//            scanner.nextLine();
//            login = scanner.nextLine();
//            password = scanner.nextLine();
//        } catch (FileNotFoundException ignored) {
//        }

        userData = new UserData(isNewUser);

        DataEncryptor dataEncryptor = new DataEncryptorImpl();
        userData.setLogin(login);
        userData.setPassword(dataEncryptor.encryptStringSHA256(password));

        try {
            responseSender.sendUserData(userData, isa);

            userData = requestReader.receiveUserData();
            if (userData == null) return -1;
            if (userData.getIsConnected()) {
                clientCollection = new ClientCollectionImpl();
                clientCollection.setClientArrayList(new ArrayList<>(userData.getDragonCollection().values()));
            }
            return userData.getIsConnected() ? 1 : 0;
        } catch (ClosedChannelException | BindException ex) {
            rootLogger.error("Ошибка подключения к сокету. port:" + isa.getPort());

        } catch (IOException ex) {
            rootLogger.error("Ошибка ввода вывода." + isa.getPort());
            ex.printStackTrace();
        } catch (InterruptedException exception) {
            rootLogger.error("Interrupted exc");
            exception.printStackTrace();
        }
        return -1;
    }

    public ClientCollection getClientCollection() {
        return clientCollection;
    }

    public int executeCommand(String commandAdaptation) {

        UserIO userIO = new UserIO(new Scanner(commandAdaptation));
        commandProcessor.getCommandInvoker().reinitCommandsUserIO(userIO);

        boolean isCommandAcceptable = commandProcessor.executeCommand(userIO.readLine(), userData);

        try {
            if (isCommandAcceptable) {

                userData.setCommandContainer(commandProcessor.getCommandInvoker().getLastCommandContainer());
                responseSender.sendUserData(userData, isa);

                rootLogger.info("Данные были отправлены.");
                UserData userData = requestReader.receiveUserData();
                if (userData != null) {
                    this.userData = userData;
                    clientCollection.setClientArrayList(new ArrayList<>(userData.getDragonCollection().values()));
                } else {
                    return 0;
                }
                rootLogger.info("Данные были получены.");

                return 1;
            }
        } catch (PortUnreachableException | SocketTimeoutException ex) {
            if (ex instanceof PortUnreachableException) {
                rootLogger.warn("Порт " + isa.getPort() + " не доступен. Повторить отправку команды? y/n");
            } else {
                rootLogger.warn("Сервер не отвечает. Повторить отправку команды? y/n");
            }
        } catch (InterruptedException ex) {
            rootLogger.error("Interrupted exception");
            ex.printStackTrace();
        } catch (ClosedChannelException ex) {
            rootLogger.error("Closed channel exception");
        } catch (IOException ex) {
            rootLogger.error("IOException");
            ex.printStackTrace();
        }
        return -1;
    }

    public Integer getPort() {
        return isa.getPort();
    }

    public void setPort(int port) {
        isa = new InetSocketAddress(isa.getHostName(), port);
        rootLogger.info("Установлен порт: " + port);
    }

    public UserData getUserData() {
        return userData;
    }

    public String getCommandResult() {
        return new String(userData.getCommandContainer().getByteArray(), StandardCharsets.UTF_8);
    }

    public String executeHelpCommand() {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream printStream;
        try {
            printStream = new PrintStream(baos, true, "UTF-8");

        } catch (UnsupportedEncodingException ex) {
            printStream = new PrintStream(baos, true);
        }
        commandProcessor.executeHelpCommand(printStream, userData);
        rootLogger.info("Команда help была исполнена.");
        return baos.toString();
    }
}
