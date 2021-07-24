package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.collection.Dragon;
import ru.nkulakov.collection.DragonFieldValidation;
import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;
import ru.nkulakov.file.DragonFieldsReader;
import ru.nkulakov.io.UserIO;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.concurrent.locks.Lock;

/**
 * Класс команды, отвечающей за добавление элемента в коллкцию.
 */
public class InsertElementCommand extends Command {

    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    private CollectionDatabaseHandler cdh;

    /**
     * Поле, хранящее ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    private UserIO userIO;


    public InsertElementCommand(UserIO userIO) {
        super("insert");
        this.userIO = userIO;
    }

    /**
     * Конструктор класса, предназначенный для серверной части команды.
     *
     * @param collectionManager менеджер коллекции.
     */
    public InsertElementCommand(CollectionManager collectionManager, CollectionDatabaseHandler cdh) {
        this.cdh = cdh;
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. При запуске команды запрашивает ввод указанных полей. При успешном выполнении команды на стороне сервера высветится уведомление о добавлении элемента в коллекцию. В случае критической ошибки выполнение команды прерывается.
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
                throw new CannotExecuteCommandException("Количество аргументов у данной команды должно равняться 1.");
            }
            if (DragonFieldValidation.validate("id", arguments[0])) {
                printStream.println("Введите значения полей для элемента коллекции:\n");
                DragonFieldsReader dragonFieldsReader = new DragonFieldsReader(userIO);

                Dragon dragon = dragonFieldsReader.read(Integer.parseInt(arguments[0]));
                dragon.setOwner(userData.getLogin());
                super.result.add(Integer.parseInt(arguments[0])); //Integer id - result(0), dragon - result(1)
                super.result.add(dragon);
            } else
                throw new CannotExecuteCommandException("Введены невалидные аргументы: id = " + arguments[0]);
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            try {
                locker.lock();

                Dragon dragon = (Dragon) this.getResult().get(1);
                if (!cdh.isAnyRowById(dragon.getId())) {
                    cdh.insertRow(dragon);
                    collectionManager.insert((Integer) this.getResult().get(0), dragon, printStream);
                    printStream.println("The element was added.");
                } else {
                    printStream.println("Элемент с указанным id уже существует.");
                }
            } finally {
                locker.unlock();
            }

        }
    }


    /**
     * Метод, возвращающий описание данной команды.
     *
     * @return Описание данной команды.
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "inserts the element is collection with id";
    }

    public void setUserIO(UserIO userIO) {
        this.userIO = userIO;
    }
}
