package ru.nkulakov.sender;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.database.UserData;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class ResponseSenderImpl implements ResponseSender {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final DatagramSocket serverSocket;
    private UserData userData;


    public ResponseSenderImpl(DatagramSocket serverSocket) {
        this.serverSocket = serverSocket;
    }

    //отправляем сразу сериализованный объект
    public ResponseSenderImpl(DatagramSocket serverSocket, UserData userData) {
        this.serverSocket = serverSocket;
        this.userData = userData;
    }

    @Override
    public void run() { //TODO cut the packages if their length > 4096
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(userData);
            oos.close();
        } catch (IOException ex) {
            rootLogger.error("Невозможно записать объект.");
            ex.printStackTrace();
        }
        if (baos.toByteArray().length > 4096) {
            rootLogger.warn("Размер пакета превышает допустимый. Разделить пакеты пока не представляется возможным");
        } else {
            byte[] byteUdp = new byte[baos.size()];
            System.arraycopy(baos.toByteArray(), 0, byteUdp, 0, byteUdp.length);
            DatagramPacket dp = new DatagramPacket(byteUdp, byteUdp.length, userData.getInetAddress(), userData.getPort());
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
