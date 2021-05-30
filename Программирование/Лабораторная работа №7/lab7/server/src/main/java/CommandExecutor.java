import database.UserData;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.util.HashMap;

public class CommandExecutor implements Runnable {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final CommandProcessor commandProcessor;
    private final UserData userData;
    private final HashMap<UserData, ByteArrayOutputStream> responseMap;

    public CommandExecutor(CommandProcessor commandProcessor, UserData userData, HashMap<UserData, ByteArrayOutputStream> responseMap) {
        this.commandProcessor = commandProcessor;
        this.userData = userData;
        this.responseMap = responseMap;
    }

    @Override
    public void run() {
        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            if (userData.getCommandContainer() != null) {
                PrintStream printStream = new PrintStream(baos);
                if (commandProcessor.getCommandInvoker().executeServer(userData.getCommandContainer().getName(),
                        userData.getCommandContainer().getResult(), printStream, userData)) {
                    rootLogger.info("Была исполнена команда " + userData.getCommandContainer().getName());
                } else {
                    rootLogger.info("Не была исполнена команда " + userData.getCommandContainer().getName());
                }
            } else {
                ObjectOutputStream oos = new ObjectOutputStream(baos);
                oos.writeBoolean(userData.getIsConnected());
                oos.close();
            }
            synchronized (responseMap) {
                responseMap.put(userData, baos);
            }
        } catch (IOException ex) {
            rootLogger.error("Ошибка при попытке отправки ответа на клиент.");
            ex.printStackTrace();
        }
    }
}
