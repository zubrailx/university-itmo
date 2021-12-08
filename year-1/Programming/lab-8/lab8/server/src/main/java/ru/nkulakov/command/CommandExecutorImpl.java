package ru.nkulakov.command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.database.UserData;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

public class CommandExecutorImpl implements CommandExecutor {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final CommandProcessor commandProcessor;
    private final UserData userData;
    private final ArrayList<UserData> udal;

    public CommandExecutorImpl(CommandProcessor commandProcessor, UserData userData, ArrayList<UserData> udal) {
        this.commandProcessor = commandProcessor;
        this.userData = userData;
        this.udal = udal;
    }

    @Override
    public void run() {
        ByteArrayOutputStream printBaos = new ByteArrayOutputStream();
        PrintStream printStream;
        try {
            printStream = new PrintStream(printBaos, true, "UTF-8");
        } catch (UnsupportedEncodingException ex) {
            printStream = new PrintStream(printBaos, true);
        }
        if (userData.getCommandContainer() != null) {
            if (commandProcessor.getCommandInvoker().executeServer(userData.getCommandContainer().getName(),
                    userData.getCommandContainer().getResult(), printStream, userData)) {
                rootLogger.info("Была исполнена команда " + userData.getCommandContainer().getName());
            } else {
                rootLogger.info("Не была исполнена команда " + userData.getCommandContainer().getName());
            }

            userData.getCommandContainer().setByteArr(printBaos.toByteArray());
        }
        if (userData.getIsConnected())
            userData.setDragonCollection(commandProcessor.getCommandInvoker().getCollectionManager().getTreeMap());
        synchronized (udal) {
            udal.add(userData);
        }
    }
}
