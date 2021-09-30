package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.collection.Dragon;
import ru.nkulakov.collection.DragonFieldReplacer;
import ru.nkulakov.collection.DragonFieldValidation;
import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;
import ru.nkulakov.io.UserIO;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.concurrent.locks.Lock;

public class UpdateElementCommand extends Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    private CollectionDatabaseHandler cdh;
    /**
     * Поле, хранящее ссылку на объект класса UserIO.
     */
    private UserIO userIO;

    public UpdateElementCommand(UserIO userIO) {
        super("update");
        this.userIO = userIO;
    }

    public UpdateElementCommand(CollectionManager collectionManager, CollectionDatabaseHandler cdh) {
        this.cdh = cdh;
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. При вызове изменяется указанной элемент коллекции до тех пор, пока не будет передана пустая строка. В случае некорректного ввода высветится ошибка.
     *
     * @param invocationEnum режим, с которым должна быть исполнена данная команда.
     * @param printStream    поток вывода.
     * @param arguments      аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream, UserData userData, Lock locker) throws CannotExecuteCommandException, SQLException {
        if (invocationEnum.equals(InvocationStatus.CLIENT)) {
            result = new ArrayList<>();
            try {
                if (arguments.length != 1) {
                    throw new CannotExecuteCommandException("Количество аргументов данной команды должно равняться 1.");
                }
                if (!DragonFieldValidation.validate("id", arguments[0])) {
                    throw new CannotExecuteCommandException("Введены невалидные аргументы: id =" + arguments[0]);
                } else {
                    result.add(arguments[0]);
//                    printStream.println(CollectionManagerImpl.getFieldNames());
//                    printStream.println("\nВыберите поля для изменения:");
                    String[] line;

                    boolean isInputEnd = false;

                    do {
                        line = userIO.readLine().trim().split("\\s+");
                        if (line.length == 0 || line[0] == null || line[0].equals("")) isInputEnd = true;
                        else {
                            if (line.length == 1) {
                                if (DragonFieldValidation.validate(line[0], "")) {
                                    result.add(line[0] + ";");
                                } //else printStream.println("Введены некорректные данные: \"" + line[0] + "\" + null");
                            }
                            if (line.length == 2) {
                                if (DragonFieldValidation.validate(line[0], line[1])) {
                                    result.add(line[0] + ";" + line[1]);
                                } //else printStream.println("Введены некорректные данные: " + line[0] + " + " + line[1]);
                            }
                        }
                    } while (!isInputEnd);
                }
            } catch (NoSuchElementException ex) {
                throw new CannotExecuteCommandException("Сканнер достиг конца файла.");
            }
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            String[] spArguments = result.toArray(new String[0]);
            Integer id = Integer.parseInt(spArguments[0]);

            String[] fields = new String[spArguments.length - 1];
            String[] values = new String[spArguments.length - 1];
            String delimeter = ";"; // Разделитель

            try {
                locker.lock();

                if (cdh.isOwner(id, userData)) {
                    for (int i = 1; i < spArguments.length; i++) {
                        String[] subStr = spArguments[i].split(delimeter);

                        fields[i - 1] = subStr[0];
                        values[i - 1] = subStr[1];
                    }
                    Dragon dragon = DragonFieldReplacer.update(cdh.getDragonById(id), fields, values, printStream);
                    cdh.replaceRow(dragon);
                    collectionManager.removeKey(dragon.getId());
                    collectionManager.insert(dragon.getId(), dragon, printStream);
                } else {
                    printStream.println("Элемента с указанным id не существует");
                }
            } finally {
                locker.unlock();
            }

        }
    }


    /**
     * Метод, возвращающий описание команды.
     *
     * @return Метод, возвращающий описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "replaces the fields of the element by id";
    }

    public void setUserIO(UserIO userIO) {
        this.userIO = userIO;
    }
}
