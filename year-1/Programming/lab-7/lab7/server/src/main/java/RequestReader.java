import database.UserData;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.ByteArrayInputStream;
import java.io.ObjectInputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.nio.charset.StandardCharsets;
import java.util.Base64;
import java.util.concurrent.Callable;

public class RequestReader implements Callable<UserData> {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final DatagramSocket serverSocket;
    private final DatagramPacket dp;
    private byte[] byteUPD = new byte[4096];

    public RequestReader(DatagramSocket serverSocket) {
        this.serverSocket = serverSocket;
        dp = new DatagramPacket(byteUPD, byteUPD.length);
    }

    @Override
    public UserData call() throws Exception {
        serverSocket.receive(dp);
        byteUPD = dp.getData();


        String str = new String(byteUPD);
        str = str.replace("\0", "");
        byte[] byteArr = str.getBytes(StandardCharsets.UTF_8);

        ByteArrayInputStream bais = new ByteArrayInputStream(Base64.getDecoder().decode(byteArr));
        ObjectInputStream ois = new ObjectInputStream(bais);

        rootLogger.info("Получен пакет с командой от " + dp.getAddress().getHostAddress() + " " + dp.getPort());

        UserData userData = (UserData) ois.readObject();
        userData.setInetAddress(dp.getAddress());
        userData.setPort(dp.getPort());
        return userData;
    }
}
