import database.UserData;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;
import java.util.Base64;

public class ResponseSender {

    private final DatagramChannel clientChannel;

    public ResponseSender(DatagramChannel clientChannel) {
        this.clientChannel = clientChannel;
    }

    public void sendUserData(UserData userData, InetSocketAddress inetSocketAddress) throws IOException {
        ByteBuffer byteBuffer = ByteBuffer.allocate(4096);
        byteBuffer.clear();

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(baos);

        oos.writeObject(userData);

        byteBuffer.put(Base64.getEncoder().withoutPadding().encode(baos.toByteArray()));

        byteBuffer.flip();

        clientChannel.send(byteBuffer, inetSocketAddress);
    }
}
