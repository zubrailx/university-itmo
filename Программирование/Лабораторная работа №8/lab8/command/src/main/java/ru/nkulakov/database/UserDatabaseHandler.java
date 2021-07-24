package ru.nkulakov.database;

public interface UserDatabaseHandler {
    void addUser(UserData userData);

    void ConnectUser(UserData userData);

    void verifyUser(UserData userData);
}
