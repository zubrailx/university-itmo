package commands;

import collection.CollectionManager;
import collection.Dragon;
import file.DragonFieldsReader;
import io.UserIO;

public class InsertElementCommand implements CommandWithArguments {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;
    /**
     * Поле, хранящее ссылку на объект класса UserIO.
     */
    private UserIO userIO;
    /**
     * Поле, хранящее ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    private DragonFieldsReader dragonFieldsReader;
    /**
     * Массив, хранящий аргументы команды.
     */
    private String[] arguments;

    /**
     * Конструктор класса.
     *
     * @param collectionManager  Хранит ссылку на созданный в объекте Application объект CollectionManager.
     * @param userIO             Хранит ссылку на объект класса UserIO.
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    public InsertElementCommand(CollectionManager collectionManager, UserIO userIO, DragonFieldsReader dragonFieldsReader) {
        this.collectionManager = collectionManager;
        this.userIO = userIO;
        this.dragonFieldsReader = dragonFieldsReader;
    }

    /**
     * Метод, исполняющий команду. При запуске команды запрашивает ввод указанных полей. При успешном выполнении команды в потоке вывода высветится уведомление о добавлении элемента в коллекцию. В случае критической ошибки выполнение команды прерывается.
     */
    @Override
    public void execute() {
        try {
            if (!collectionManager.containsKey(Integer.parseInt(arguments[0]))) {
                userIO.printCommandText("Введите значения полей для элемента коллекции\n");
                Dragon dragon = dragonFieldsReader.read(Integer.parseInt(arguments[0]));
                collectionManager.insert(Integer.parseInt(arguments[0]), dragon);
                userIO.printCommandText("Элемент добавлен в коллекцию\n");
            } else System.err.println("Элемент с данным ключом уже существует в коллекции");
        } catch (IndexOutOfBoundsException ex) {
            System.err.println("Не указаны аргументы команды.");
        } catch (NumberFormatException ex) {
            System.err.println("Формат аргумента не соответствует целочисленному" + ex.getMessage());
        }
    }

    /**
     * @param arguments Аргументы команды.
     * @see CommandWithArguments
     */
    @Override
    public void getCommandArguments(String[] arguments) {
        this.arguments = arguments;
    }

    /**
     * @return Возвращает описание данной команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "добавляет элемент с указанным ключом в качестве атрибута";
    }
}
