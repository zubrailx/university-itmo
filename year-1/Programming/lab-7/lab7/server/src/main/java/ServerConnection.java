import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.DatagramSocket;

public class ServerConnection {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private DatagramSocket serverSocket;

    public void createFromPort(Integer port) throws IOException {
        serverSocket = new DatagramSocket(port);
        rootLogger.info("Сервер готов.");
    }

    public DatagramSocket getServerSocket() {
        return serverSocket;
    }
}
