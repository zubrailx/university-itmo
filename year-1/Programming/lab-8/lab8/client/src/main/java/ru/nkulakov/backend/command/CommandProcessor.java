package ru.nkulakov.backend.command;

import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.database.UserData;

import java.io.PrintStream;

public interface CommandProcessor {
    boolean executeCommand(String firstCommandLine, UserData userData);

    CommandInvoker getCommandInvoker();

    boolean executeHelpCommand(PrintStream printStream, UserData userData);
}
