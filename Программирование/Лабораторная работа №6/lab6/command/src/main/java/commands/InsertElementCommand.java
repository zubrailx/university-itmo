package commands;

import collection.CollectionManager;
import collection.Dragon;
import collection.DragonFieldValidation;
import commands.abstr.Command;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;
import file.DragonFieldsReader;

import java.io.PrintStream;
import java.util.ArrayList;

/**
 * Класс команды, отвечающей за добавление элемента в коллкцию.
 */
public class InsertElementCommand extends Command {

    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Поле, хранящее ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    private DragonFieldsReader dragonFieldsReader;

    /**
     * Конструктор класса.
     *
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    public InsertElementCommand(DragonFieldsReader dragonFieldsReader) {
        super("insert");
        this.dragonFieldsReader = dragonFieldsReader;
    }

    /**
     * Конструктор класса, предназначенный для серверной части команды.
     *
     * @param collectionManager менеджер коллекции.
     */
    public InsertElementCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. При запуске команды запрашивает ввод указанных полей. При успешном выполнении команды на стороне сервера высветится уведомление о добавлении элемента в коллекцию. В случае критической ошибки выполнение команды прерывается.
     *
     * @param invocationStatus режим, с которым должна быть исполнена данная команда.
     * @param printStream поток вывода.
     * @param arguments аргументы команды.
     */
    @Override
    public void execute(String[] arguments, InvocationStatus invocationStatus, PrintStream printStream) throws CannotExecuteCommandException {
        if (invocationStatus.equals(InvocationStatus.CLIENT)) {
            result = new ArrayList<>();
            if (arguments.length != 1) {
                throw new CannotExecuteCommandException("Количество аргументов у данной команды должно равняться 1.");
            }
            if (DragonFieldValidation.validate("id", arguments[0])) {
                printStream.println("Введите значения полей для элемента коллекции:\n");
                Dragon dragon = dragonFieldsReader.read(Integer.parseInt(arguments[0]));
                super.result.add(Integer.parseInt(arguments[0])); //Integer id - result(0), dragon - result(1)
                super.result.add(dragon);
            } else
                throw new CannotExecuteCommandException("Введены невалидные аргументы: id = " + arguments[0]);
        } else if (invocationStatus.equals(InvocationStatus.SERVER)) {
            Dragon dragon = (Dragon) this.getResult().get(1);
            collectionManager.insert((Integer) this.getResult().get(0), dragon, printStream);
        }
    }

    /**
     * Метод, возвращающий описание данной команды.
     * @return Описание данной команды.
     *
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "добавляет элемент с указанным ключом в качестве атрибута";
    }
}
