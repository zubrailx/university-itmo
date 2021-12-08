package ru.nkulakov.connection;

import java.io.IOException;
import java.net.DatagramSocket;

public interface ServerConnection {
    void createFromPort(Integer port) throws IOException;

    DatagramSocket getServerSocket();
}
