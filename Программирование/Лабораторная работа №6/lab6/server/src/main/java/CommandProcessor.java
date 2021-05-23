import commands.CommandInvoker;
import commands.abstr.CommandContainer;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.PrintStream;

public class CommandProcessor {

    private final CommandInvoker commandInvoker;

    private static final Logger rootLogger = LogManager.getRootLogger();

    public CommandProcessor(CommandInvoker commandInvoker) {
        this.commandInvoker = commandInvoker;
    }

    public void executeCommand(CommandContainer command, PrintStream printStream) {

        if (commandInvoker.executeServer(command.getName(), command.getResult(), printStream)) {
            rootLogger.info("Была исполнена команда " + command.getName());
        } else {
            rootLogger.info("Не была исполнена команда " + command.getName());
        }
    }
}
