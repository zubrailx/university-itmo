import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;

public class RequestReader {

    private static final Logger rootLogger = LogManager.getRootLogger();
    private final DatagramChannel clientChannel;

    public RequestReader(DatagramChannel clientChannel) {
        this.clientChannel = clientChannel;
    }

    public ByteBuffer receiveBuffer() throws IOException, InterruptedException {

        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
        int waitingTime = 0;

        while (waitingTime < 1000) {
            byteBuffer.clear();
            SocketAddress from = clientChannel.receive(byteBuffer);

            if (from != null) {
                byteBuffer.flip();
                return byteBuffer;
            }
            Thread.sleep(500);
            waitingTime++;
        }
        rootLogger.error("Сервер не отвечает. Завершение работы клиента");
        System.exit(0);
        return byteBuffer;
    }
}
