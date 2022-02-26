package ru.nkulakov.commands;

import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;

import java.io.PrintStream;
import java.util.concurrent.locks.Lock;

/**
 * Класс команды, которая завершает работу программы.
 */
public class ExitCommand extends Command {

    /**
     * Конструктор класса.
     */
    public ExitCommand() {
        super("exit");
    }

    /**
     * Метод, завершающий работу клиента. При завершении выводит соответствующее сообщение.
     *
     * @param invocationEnum режим, с которым должна быть исполнена данная команда.
     * @param printStream    поток вывода.
     * @param arguments      аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream, UserData userData, Lock locker) throws CannotExecuteCommandException {
        if (invocationEnum.equals(InvocationStatus.CLIENT)) {
            if (arguments.length > 0) {
                throw new CannotExecuteCommandException("У данной команды нет аргументов.");
            } else {
                printStream.println("Работа клиента завершена.");
                System.exit(0);
            }
        }
    }

    /**
     * @return Метод, возвращающий строку описания программы.
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "close the application";
    }
}
