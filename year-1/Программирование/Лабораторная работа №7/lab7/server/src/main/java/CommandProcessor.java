import commands.CommandInvoker;
import commands.abstr.CommandContainer;
import database.CollectionDatabaseHandler;
import database.UserData;
import database.UserDatabaseHandler;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.PrintStream;

public class CommandProcessor {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final CommandInvoker commandInvoker;

    private final UserDatabaseHandler udh;
    private final CollectionDatabaseHandler cdh;

    public CommandProcessor(UserDatabaseHandler userDatabaseHandler, CollectionDatabaseHandler collectionDatabaseHandler, CommandInvoker commandInvoker) {
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

    public void putCommandArguments(PrintStream printStream, UserData userData) {

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
