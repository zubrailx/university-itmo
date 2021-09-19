package commands;

import collection.CollectionManager;
import collection.Dragon;
import collection.DragonFieldReplacer;
import collection.DragonFieldValidation;
import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import database.CollectionDatabaseHandler;
import database.UserData;
import exceptions.CannotExecuteCommandException;
import io.UserIO;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.concurrent.locks.Lock;

public class ReplaceIfLowerCommand extends Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    private CollectionDatabaseHandler cdh;
    /**
     * Поле, хранящее ссылку на объект класса UserIO.
     */
    private UserIO userIO;

    /**
     * Конструктор класса.
     *
     * @param userIO чтение-запись с помощью указанного сканнера.
     */
    public ReplaceIfLowerCommand(UserIO userIO) {
        super("replace_if_lower");
        this.userIO = userIO;
    }

    public ReplaceIfLowerCommand(CollectionManager collectionManager, CollectionDatabaseHandler cdh) {
        this.cdh = cdh;
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. Если поля указанные значения полей меньше, то они изменятся, иначе нет. В случае некорректного ввода высветится предупреждение.
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
                    throw new CannotExecuteCommandException("Введены невалидные аргументы: id " + arguments[0]);
                } else {
                    result.add(arguments[0]);
                    printStream.println(CollectionManager.getFieldNames());
                    printStream.println("\nВыберите поля для изменения:");
                    String[] line;

                    boolean isInputEnd = false;

                    do {
                        line = userIO.readLine().trim().split("\\s+");
                        if (line.length == 0 || line[0] == null || line[0].equals("")) isInputEnd = true;
                        else {
                            if (line.length == 1) {
                                if (DragonFieldValidation.validate(line[0], "")) {
                                    result.add(line[0] + ";");
                                } else printStream.println("Введены некорректные данные: \"" + line[0] + "\" + null");
                            }
                            if (line.length == 2) {
                                if (DragonFieldValidation.validate(line[0], line[1])) {
                                    result.add(line[0] + ";" + line[1]);
                                } else printStream.println("Введены некорректные данные: " + line[0] + " + " + line[1]);
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
                        values[i - 1] = subStr.length == 1 ? "" : subStr[1];

                    }
                    Dragon dragon = DragonFieldReplacer.replaceIfLower(cdh.getDragonById(id), fields, values, printStream);
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
     * @return Возвращает описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "Заменяет поле указанного элемента коллекции (по id) если то больше, чем введенного значения поля";
    }
}
