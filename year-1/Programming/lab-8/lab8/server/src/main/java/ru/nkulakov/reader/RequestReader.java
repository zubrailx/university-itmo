package ru.nkulakov.reader;

import ru.nkulakov.database.UserData;

import java.util.concurrent.Callable;

public interface RequestReader extends Callable<UserData> {
    UserData call() throws Exception;
}
