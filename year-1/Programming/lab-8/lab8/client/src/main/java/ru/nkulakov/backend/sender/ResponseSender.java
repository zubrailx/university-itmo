package ru.nkulakov.backend.sender;

import ru.nkulakov.database.UserData;

import java.io.IOException;
import java.net.InetSocketAddress;

public interface ResponseSender {

    void sendUserData(UserData userData, InetSocketAddress isa) throws IOException;
}
