package commands;

import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;

import java.io.PrintStream;

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
     * @param invocationEnum режим, с которым должна быть исполнена данная команда.
     * @param printStream поток вывода.
     * @param arguments аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream) throws CannotExecuteCommandException {
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
     *
     * @return String - описание команды.
     *
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "завершает работу программы";
    }
}
