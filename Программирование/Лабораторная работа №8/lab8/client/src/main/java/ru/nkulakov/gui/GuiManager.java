package ru.nkulakov.gui;

import ru.nkulakov.collection.ClientCollection;

public interface GuiManager {
    int authenticate(String login, String password, boolean bool);

    boolean executeCommand(String commandAdaptation);

    Integer getClientPort();

    void setClientPort(int port);

    void createPortFrame();

    ClientCollection getClientCollection();

    String getUserLogin();

    String getCommandResult();

    String executeHelpCommand();
}
