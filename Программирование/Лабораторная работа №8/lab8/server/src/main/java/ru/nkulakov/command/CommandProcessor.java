package ru.nkulakov.command;

import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.commands.abstr.CommandContainer;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.database.UserDatabaseHandler;

import java.io.PrintStream;

public interface CommandProcessor {
    void executeCommand(CommandContainer commandContainer, PrintStream printStream, UserData userData);

    UserDatabaseHandler getUdh();

    CollectionDatabaseHandler getCdh();

    CommandInvoker getCommandInvoker();
}
