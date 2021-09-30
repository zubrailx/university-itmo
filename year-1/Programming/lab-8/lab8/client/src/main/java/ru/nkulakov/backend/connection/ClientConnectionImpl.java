package ru.nkulakov.backend.connection;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.DatagramChannel;

public class ClientConnectionImpl implements ClientConnection {

    private final DatagramChannel clientChannel;

    public ClientConnectionImpl() throws IOException {
        clientChannel = DatagramChannel.open();
        clientChannel.configureBlocking(false);
    }

    public void connect(InetSocketAddress inetServerAddress) throws IOException {
        clientChannel.connect(inetServerAddress);
    }

    public DatagramChannel getClientChannel() {
        return clientChannel;
    }
}
