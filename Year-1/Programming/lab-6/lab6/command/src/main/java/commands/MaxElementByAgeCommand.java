package commands;

import collection.CollectionManager;
import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;

import java.io.PrintStream;

public class MaxElementByAgeCommand extends Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */

    private CollectionManager collectionManager;

    /**
     * Конструктор класса.
     */
    public MaxElementByAgeCommand() {
        super("max_by_age");
    }

    public MaxElementByAgeCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В случае успешного выполнения высветится элемент коллекции, значение поля age которого максимально, иначе предупреждение.
     *
     * @param invocationStatus режим, с которым должна быть исполнена данная команда.
     * @param printStream поток вывода.
     * @param arguments аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationStatus, PrintStream printStream) throws CannotExecuteCommandException {
        if (invocationStatus.equals(InvocationStatus.CLIENT)) {
            if (arguments.length > 0) {
                throw new CannotExecuteCommandException("Введены неверные аргументы команды. ");
            }
        } else if (invocationStatus.equals(InvocationStatus.SERVER)) {
            if (collectionManager.maxAge() == null) printStream.println("Коллекция пуста.");
            else printStream.println(collectionManager.maxAge().toString());
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
        return "выводит информацию об элементе коллекции, с максимальным возрастом";
    }
}
