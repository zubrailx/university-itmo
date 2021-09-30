package ru.nkulakov.command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.commands.abstr.CommandContainer;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.database.UserDatabaseHandler;

import java.io.PrintStream;

public class CommandProcessorImpl implements CommandProcessor {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final CommandInvoker commandInvoker;

    private final UserDatabaseHandler udh;
    private final CollectionDatabaseHandler cdh;

    public CommandProcessorImpl(UserDatabaseHandler userDatabaseHandler, CollectionDatabaseHandler collectionDatabaseHandler, CommandInvoker commandInvoker) {
        this.udh = userDatabaseHandler;
        this.cdh = collectionDatabaseHandler;
        this.commandInvoker = commandInvoker;
    }

    public void executeCommand(CommandContainer command, PrintStream printStream, UserData userData) {

        if (commandInvoker.executeServer(command.getName(), command.getResult(), printStream, userData)) {
            rootLogger.info("Была исполнена команда " + command.getName());
        } else {
            rootLogger.info("Не была исполнена команда " + command.getName());
        }
    }

    public UserDatabaseHandler getUdh() {
        return udh;
    }

    public CollectionDatabaseHandler getCdh() {
        return cdh;
    }

    public CommandInvoker getCommandInvoker() {
        return commandInvoker;
    }
}
