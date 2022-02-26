import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.DatagramChannel;

public class ClientConnection {

    private DatagramChannel clientChannel;

    public void connect(InetSocketAddress inetServerAddress) throws IOException {
        clientChannel = DatagramChannel.open();
        clientChannel.configureBlocking(false);
        clientChannel.connect(inetServerAddress);
    }

    public DatagramChannel getClientChannel() {
        return clientChannel;
    }
}
