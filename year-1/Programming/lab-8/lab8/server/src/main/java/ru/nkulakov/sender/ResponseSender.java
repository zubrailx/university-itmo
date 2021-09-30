package ru.nkulakov.sender;

import java.net.DatagramSocket;

public interface ResponseSender extends Runnable {
    void run();

    DatagramSocket getServerSocket();
}
