package commands;

import collection.CollectionManager;
import io.UserIO;

public class ReplaceIfGreaterCommand implements CommandWithArguments {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;
    /**
     * Поле, хранящее ссылку на объект класса UserIO.
     */
    private UserIO userIO;
    /**
     * Поле, хранящее массив аргументов команды.
     */
    private String[] commandArguments;

    /**
     * Конструктор класса.
     *
     * @param collectionManager Хранит ссылку на созданный в объекте Application объект CollectionManager.
     * @param userIO            Хранит ссылку на объект класса UserIO.
     */
    public ReplaceIfGreaterCommand(CollectionManager collectionManager, UserIO userIO) {

        this.collectionManager = collectionManager;
        this.userIO = userIO;
    }

    /**
     * Метод, исполняющий команду. Если поля указанные значения полей больше, то они изменятся, иначе нет. В случае некорректного ввода высветится ошибка или предупреждение.
     */
    @Override
    public void execute() {
        try {
            if (collectionManager.containsKey(Integer.parseInt(commandArguments[0]))) {
                userIO.printCommandText(collectionManager.getFieldNames());
                userIO.printCommandText("Введите имя поля и значение через пробел:\n");
                String[] str = userIO.readLine().trim().split("\\s+");
                if (str.length == 1) {
                    collectionManager.replaceIfGreater(Integer.parseInt(commandArguments[0]), str[0], "");
                } else {
                    collectionManager.replaceIfGreater(Integer.parseInt(commandArguments[0]), str[0], str[1]);
                }
            } else System.out.println("Элемента в коллекции по данному ключу не существует");
        } catch (IndexOutOfBoundsException ex) {
            System.err.println("Не указаны аргументы команды.");
        } catch (NumberFormatException ex) {
            System.err.println("Формат команды не соответствует требуемому.");
        }
    }

    /**
     * @param commandArguments Аргументы команды.
     * @see CommandWithArguments
     */
    @Override
    public void getCommandArguments(String[] commandArguments) {
        this.commandArguments = commandArguments;
    }

    /**
     * @return Возвращает описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "заменяет поле указанного элемента коллекции (по id) если то меньше, чем введенного значения поля";
    }
}
