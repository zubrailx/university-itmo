package ru.nkulakov.backend.connection;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.DatagramChannel;

public interface ClientConnection {

    void connect(InetSocketAddress isa) throws IOException;

    DatagramChannel getClientChannel();
}
