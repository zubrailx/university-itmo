import database.UserData;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Server implements Runnable {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final RequestReader requestReader;
    private final ResponseSender responseSender;
    private final CommandProcessor commandProcessor;
    private final ArrayList<UserData> requestList = new ArrayList<>();
    private final ArrayList<UserData> commandList = new ArrayList<>();
    private final HashMap<UserData, ByteArrayOutputStream> responseMap = new HashMap<>();
    ExecutorService serviceReader = Executors.newFixedThreadPool(2);

    public Server(RequestReader rr, ResponseSender rs, CommandProcessor cp) {
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

            UserData[] responseKeys;
            ByteArrayOutputStream[] responseValues;
            ArrayList<Thread> responseThread = new ArrayList<>();
            synchronized (responseMap) {
                responseKeys = responseMap.keySet().toArray(new UserData[0]);
                responseValues = responseMap.values().toArray(new ByteArrayOutputStream[0]);
            }
            for (int i = 0; i < responseKeys.length; i++) {
                responseThread.add(this.sendResponse(responseKeys[i], responseValues[i]));
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
                commandProcessor.getUdh().ConnectUser(userData);
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
        CommandExecutor commandExecutor = new CommandExecutor(commandProcessor, userData, responseMap);
        Thread t = new Thread(commandExecutor);
        t.start();
        return t;
    }

    private Thread sendResponse(UserData userData, ByteArrayOutputStream baos) {
        synchronized (responseMap) {
            responseMap.remove(userData, baos);
        }
        rootLogger.info("Отправка для " + userData.getPort() + " началась.");
        Thread t = new Thread(new ResponseSender(responseSender.getServerSocket(), userData.getInetAddress(), userData.getPort(), baos.toByteArray()));
        t.start();
        return t;
    }
}

