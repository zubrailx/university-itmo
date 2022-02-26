package ru.nkulakov.backend.client;

import ru.nkulakov.collection.ClientCollection;
import ru.nkulakov.database.UserData;

public interface Client {
    int authenticate(String login, String password, boolean isNewUser);

    int executeCommand(String commandAdaptation);

    String executeHelpCommand();

    ClientCollection getClientCollection();

    Integer getPort();

    void setPort(int port);

    UserData getUserData();

    String getCommandResult();
}
