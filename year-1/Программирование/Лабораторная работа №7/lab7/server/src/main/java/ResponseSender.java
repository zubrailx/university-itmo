import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ResponseSender implements Runnable {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final DatagramSocket serverSocket;

    private InetAddress receiverAddress;
    private int receiverPort;
    private byte[] byteArr;


    public ResponseSender(DatagramSocket serverSocket) {
        this.serverSocket = serverSocket;
    }

    public ResponseSender(DatagramSocket serverSocket, InetAddress receiverAddress, int receiverPort, byte[] byteArr) {
        this.serverSocket = serverSocket;
        this.receiverAddress = receiverAddress;
        this.receiverPort = receiverPort;
        this.byteArr = byteArr;
    }

    @Override
    public void run() {
        if (byteArr.length > 4096) {
            rootLogger.warn("Размер пакета превышает допустимый. Разделить пакеты пока не представляется возможным");
        } else {
            byte[] byteUdp = new byte[byteArr.length];
            System.arraycopy(byteArr, 0, byteUdp, 0, byteUdp.length);
            DatagramPacket dp = new DatagramPacket(byteUdp, byteUdp.length, receiverAddress, receiverPort);
            try {
                serverSocket.send(dp);
            } catch (IOException exception) {
                rootLogger.warn("Произошла ошибка при отправке: " + exception.getMessage());
            }
        }
    }

    public DatagramSocket getServerSocket() {
        return serverSocket;
    }
}
