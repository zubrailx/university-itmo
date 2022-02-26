package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.commands.abstr.CommandContainer;
import ru.nkulakov.database.UserData;
import ru.nkulakov.io.UserIO;

import java.io.PrintStream;
import java.util.ArrayList;

public interface CommandInvoker {
    boolean executeClient(String firstCommandLine, PrintStream printStream, UserData userData);

    boolean executeServer(String firstCommandLine, ArrayList<Object> result, PrintStream printStream, UserData userData);

    CommandContainer getLastCommandContainer();

    CollectionManager getCollectionManager();

    void reinitCommandsUserIO(UserIO userIO);

}
