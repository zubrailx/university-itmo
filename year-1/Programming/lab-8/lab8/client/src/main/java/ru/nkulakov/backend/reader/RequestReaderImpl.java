package ru.nkulakov.backend.reader;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.database.UserData;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.DatagramChannel;

public class RequestReaderImpl implements RequestReader {

    private static final Logger rootLogger = LogManager.getRootLogger();
    private final DatagramChannel clientChannel;

    public RequestReaderImpl(DatagramChannel clientChannel) {
        this.clientChannel = clientChannel;
    }

    public UserData receiveUserData() throws IOException, InterruptedException {

        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
        int waitingTime = 0;

        while (waitingTime < 5) {
            byteBuffer.clear();
            SocketAddress from = clientChannel.receive(byteBuffer);

            if (from != null) {
                byteBuffer.flip();
                //конвертируем сразу в UserData.
                ByteArrayInputStream bais = new ByteArrayInputStream(byteBuffer.array(), 0, byteBuffer.limit());
                ObjectInputStream ois = new ObjectInputStream(bais);

                try {  //чтобы не обрабатывать выше, поскольку это не касается данного метода.
                    return (UserData) ois.readObject();
                } catch (ClassNotFoundException ex) {
                    return null;
                }
            }
            Thread.sleep(500);
            waitingTime++;
        }
        rootLogger.error("Сервер не отвечает.");
        throw new ClosedChannelException();
    }
}
