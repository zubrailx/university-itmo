import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.charset.StandardCharsets;

public class ResponseSender {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final DatagramSocket serverSocket;

    public ResponseSender(DatagramSocket serverSocket) {
        this.serverSocket = serverSocket;
    }

    public void send(String str, InetAddress receiverAddress, int receiverPort) throws IOException {

        byte[] byteUDP = new byte[4096];

        DatagramPacket dp = new DatagramPacket(byteUDP, byteUDP.length, receiverAddress, receiverPort);

        byte[] byteArr = str.getBytes(StandardCharsets.UTF_8);
        if (byteArr.length > 4096) {
            rootLogger.warn("Размер пакета превышает допустимый. Разделить пакеты пока не представляется возможным");
            return;
        } else {
            System.arraycopy(byteArr, 0, byteUDP, 0, byteArr.length);
        }
        serverSocket.send(dp);
    }
}
