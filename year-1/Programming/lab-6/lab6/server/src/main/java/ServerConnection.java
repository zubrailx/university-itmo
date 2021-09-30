import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.net.DatagramSocket;
import java.net.SocketException;

public class ServerConnection {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private DatagramSocket serverSocket;

    public boolean createFromPort(Integer port) {

        try {
            serverSocket = new DatagramSocket(port);
            rootLogger.info("Сервер готов.");
            return true;
        } catch (SocketException ex) {
            rootLogger.warn("Порт " + port + " занят или сокет не может быть открыт. Попробуйте с другим портом.");
            System.exit(-1);
        }
        return false;
    }

    public DatagramSocket getServerSocket() {
        return serverSocket;
    }
}
