package commands;

import collection.CollectionManager;
import collection.Color;
import collection.DragonFieldValidation;
import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import database.CollectionDatabaseHandler;
import database.UserData;
import exceptions.CannotExecuteCommandException;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Locale;
import java.util.concurrent.locks.Lock;

public class RemoveAnyByColorCommand extends Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    private CollectionDatabaseHandler cdh;

    /**
     * Конструктор класса.
     */
    public RemoveAnyByColorCommand() {
        super("remove_any_by_color");
    }


    public RemoveAnyByColorCommand(CollectionManager collectionManager, CollectionDatabaseHandler cdh) {
        this.cdh = cdh;
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В случае успешного выполнения удалится случайный элемент коллекции с соответствующим полем Color color и высветится результат, иначе предупреждение.
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
                throw new CannotExecuteCommandException("Количество аргументов у данной команды должно равняться 1");
            }
            if (DragonFieldValidation.validate("color", arguments[0].toUpperCase(Locale.ROOT))) {
                result.add(arguments[0].toUpperCase(Locale.ROOT));
            } else {
                StringBuilder stringBuilder = new StringBuilder();
                stringBuilder.append("Выбранной константы нет в перечислении.\nСписок всех констант:\n");
                for (Color color : Color.values()) {
                    stringBuilder.append(color).append("\n");
                }
                throw new CannotExecuteCommandException(stringBuilder.toString());
            }
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            Color color = Color.valueOf((String) this.getResult().get(0));
            try {
                locker.lock();

                Integer dId = cdh.getOwnedRowByColor(color, userData);
                if (dId != null) {
                    cdh.deleteRowById(dId);
                    collectionManager.removeKey(dId);
                    printStream.println("Элемент с id = " + dId + "был удален.");
                } else {
                    printStream.println("Соответствующих элементов в бд не существует.");
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
        return "удаляет случайный элемент коллекции, поля Color которого равно указанному при вводе в качестве атрибута";
    }
}
