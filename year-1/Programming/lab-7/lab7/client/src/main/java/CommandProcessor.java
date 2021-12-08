import commands.CommandInvoker;
import database.UserData;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class CommandProcessor {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final CommandInvoker commandInvoker;

    public CommandProcessor(CommandInvoker commandInvoker) {
        this.commandInvoker = commandInvoker;
    }

    public boolean executeCommand(String firstCommandLine, UserData userData) {

        if (!commandInvoker.executeClient(firstCommandLine, System.out, userData)) {
            rootLogger.warn("Команда не была исполнена");
            return false;
        } else {
            return !commandInvoker.getLastCommandContainer().getName().equals("help");
        }
    }
}
