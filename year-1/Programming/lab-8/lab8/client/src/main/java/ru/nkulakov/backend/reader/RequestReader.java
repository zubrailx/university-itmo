package ru.nkulakov.backend.reader;

import ru.nkulakov.database.UserData;

import java.io.IOException;

public interface RequestReader {
    UserData receiveUserData() throws IOException, InterruptedException;
}
