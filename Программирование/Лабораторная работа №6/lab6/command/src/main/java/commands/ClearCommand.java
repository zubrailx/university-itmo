package commands;

import collection.CollectionManager;
import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;

import java.io.PrintStream;

/**
 * Команда, очищающая коллекцию.
 */
public class ClearCommand extends Command {

    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Конструктор класса
     */
    public ClearCommand() {
        super("clear");
    }

    /**
     * Конструктор класа, предназначенный для сервера.
     * @param collectionManager менеджер коллекции
     */
    public ClearCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. Выводит сообщение когда коллекция очищена.
     * @param arguments      аргументы команды.
     * @param invocationEnum режим работы команды
     * @param printStream поток, куда следует выводит результат команды
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream) throws CannotExecuteCommandException {
        if (invocationEnum.equals(InvocationStatus.CLIENT)) {
            if (arguments.length > 0) {
                throw new CannotExecuteCommandException("У данной команды нет аргументов.");
            }
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            collectionManager.clear();
            printStream.println("Коллекция " + collectionManager.getClass().getSimpleName() + " была очищена.");
        }
    }

    /**
     * Метод, возвращающий описание команды.
     * @return Описание команды
     *
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "очищает все элементы коллекции";
    }
}