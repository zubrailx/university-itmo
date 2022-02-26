package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;

import java.io.PrintStream;
import java.util.concurrent.locks.Lock;

/**
 * Класс команды, отвечающей за получение информации о текущей коллекции.
 */
public class InfoCommand extends Command {

    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Конструктор класса.
     *
     * @param collectionManager Хранит ссылку на объект CollectionManager.
     */
    public InfoCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Конструктор без параметров.
     */
    public InfoCommand() {
        super("info");
    }

    /**
     * Метод, исполняющий команду. Выводит описание коллекции TreeMap экземпляров Dragon.
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
            }

        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            printStream.println(collectionManager.info());
        }
    }

    /**
     * Метод, возвращющий описание команды.
     *
     * @return Описание команды.
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "sout the information about collection (type, collection initialization, collection info, count of elements)";
    }
}
