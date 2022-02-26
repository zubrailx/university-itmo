package ru.nkulakov.backend.command;

import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.database.UserData;

import java.io.PrintStream;

public class CommandProcessorImpl implements CommandProcessor {

    private final CommandInvoker commandInvoker;

    public CommandProcessorImpl(CommandInvoker commandInvoker) {
        this.commandInvoker = commandInvoker;
    }

    public boolean executeCommand(String firstCommandLine, UserData userData) {

        if (!commandInvoker.executeClient(firstCommandLine, System.out, userData)) {
            return false;
        } else {
            return !commandInvoker.getLastCommandContainer().getName().equals("help");
        }
    }

    public CommandInvoker getCommandInvoker() {
        return commandInvoker;
    }

    public boolean executeHelpCommand(PrintStream printStream, UserData userData) {
        return commandInvoker.executeClient("help", printStream, userData);
    }
}
