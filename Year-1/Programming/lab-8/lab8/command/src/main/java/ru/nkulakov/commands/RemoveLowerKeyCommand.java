package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.collection.DragonFieldValidation;
import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.concurrent.locks.Lock;

public class RemoveLowerKeyCommand extends Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    private CollectionDatabaseHandler cdh;

    /**
     * Конструктор класса.
     */
    public RemoveLowerKeyCommand() {
        super("remove_lower_key");
    }

    public RemoveLowerKeyCommand(CollectionManager collectionManager, CollectionDatabaseHandler cdh) {
        this.cdh = cdh;
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В случае успешного выполнения удалятся все элементы коллекции, значения идентификатора которых меньше переданного в качестве аргументов значения, иначе предупреждение.
     *
     * @param invocationEnum режим, с которым должна быть исполнена данная команда.
     * @param printStream    поток вывода.
     * @param arguments      аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream, UserData userData, Lock locker) throws CannotExecuteCommandException, SQLException {
        if (invocationEnum.equals(InvocationStatus.CLIENT)) {
            result = new ArrayList<>();
            if (arguments.length != 1) {
                throw new CannotExecuteCommandException("Количество аргументов у данной команды равно 1.");
            }
            if (DragonFieldValidation.validate("id", arguments[0])) {
                result.add(Integer.parseInt(arguments[0])); //сохраняем id, меньше которых следует удалять.
            } else {
                throw new CannotExecuteCommandException("Введены невалидные аргументы: id = " + arguments[0]);
            }
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            Integer id = (Integer) this.getResult().get(0);
            try {
                locker.lock();

                Integer[] keys = collectionManager.getLowerKeys(id);
                for (Integer key : keys) {
                    if (cdh.isOwner(id, userData)) {
                        cdh.deleteRowById(key);
                        collectionManager.removeKey(key);
                    }
                }
            } finally {
                locker.unlock();
            }
            printStream.println("Элементы с id < " + id + " были удалены, принадлежащие пользователю " + userData.getLogin());
        }
    }

    /**
     * Метод, возвращающий описание команды.
     *
     * @return Возращает описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "deletes all elements whose id is lower";
    }
}
