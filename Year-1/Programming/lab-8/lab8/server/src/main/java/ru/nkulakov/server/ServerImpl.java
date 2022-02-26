package ru.nkulakov.server;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.command.CommandExecutor;
import ru.nkulakov.command.CommandExecutorImpl;
import ru.nkulakov.command.CommandProcessor;
import ru.nkulakov.database.UserData;
import ru.nkulakov.reader.RequestReader;
import ru.nkulakov.sender.ResponseSender;
import ru.nkulakov.sender.ResponseSenderImpl;

import java.util.ArrayList;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class ServerImpl implements Server {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final RequestReader requestReader;
    private final ResponseSender responseSender;
    private final CommandProcessor commandProcessor;
    private final ArrayList<UserData> requestList = new ArrayList<>();
    private final ArrayList<UserData> commandList = new ArrayList<>();
    private final ArrayList<UserData> responseList = new ArrayList<>();
    ExecutorService serviceReader = Executors.newFixedThreadPool(2);

    public ServerImpl(RequestReader rr, ResponseSender rs, CommandProcessor cp) {
        this.requestReader = rr;
        this.responseSender = rs;
        this.commandProcessor = cp;
    }

    @Override
    public void run() {
        while (true) {
            Future<UserData> request = serviceReader.submit(requestReader);
            synchronized (requestList) {
                try {
                    requestList.add(request.get());
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
            }
            UserData[] requestArray = requestList.toArray(new UserData[0]);
            for (UserData userData : requestArray) {
                this.findNextStep(userData);
            }

            UserData[] commandArray = commandList.toArray(new UserData[0]);
            ArrayList<Thread> commandThread = new ArrayList<>();
            for (UserData userData : commandArray) {
                commandThread.add(this.startExecution(userData));
            }
            for (Thread thread : commandThread) {
                try {
                    thread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            UserData[] userDatas;
            ArrayList<Thread> responseThread = new ArrayList<>();
            synchronized (responseList) {
                userDatas = responseList.toArray(new UserData[0]);
            }
            for (UserData userData : userDatas) {
                responseThread.add(this.sendResponse(userData));
            }
            for (Thread thread : responseThread) {
                try {
                    thread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void findNextStep(UserData userData) {
        synchronized (requestList) {
            requestList.remove(userData);
        }
        if (!userData.getIsConnected()) {
            if (userData.getIsNewUser()) {
                commandProcessor.getUdh().addUser(userData);
            }
            if (!userData.getIsConnected()) {
                commandProcessor.getUdh().verifyUser(userData);
            }
            if (!userData.getIsNewUser()) {
                commandProcessor.getUdh().ConnectUser(userData); //установка необходимых булек
            }
        }
        synchronized (commandList) {
            commandList.add(userData);
        }
    }

    private Thread startExecution(UserData userData) {
        rootLogger.info("Обработка запроса для " + userData.getPort() + " началась.");
        synchronized (commandList) {
            commandList.remove(userData);
        }
        CommandExecutor commandExecutor = new CommandExecutorImpl(commandProcessor, userData, responseList);
        Thread t = new Thread(commandExecutor);
        t.start();
        return t;
    }

    private Thread sendResponse(UserData userData) {
        synchronized (responseList) {
            responseList.remove(userData);
        }
        rootLogger.info("Отправка для " + userData.getPort() + " началась.");
        Thread t = new Thread(new ResponseSenderImpl(responseSender.getServerSocket(), userData));
        t.start();
        return t;
    }
}

