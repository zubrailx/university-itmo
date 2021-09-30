package commands;

import collection.CollectionManager;
import collection.Color;

public class RemoveAnyByColorCommand implements CommandWithArguments {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;
    /**
     * Поле, хранящее массив аргументов команды.
     */
    private String[] commandArguments;

    /**
     * Конструктор класса.
     *
     * @param collectionManager Хранит ссылку на созданный в объекте Application объект CollectionManager.
     */
    public RemoveAnyByColorCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В случае успешного выполнения удалится случайный элемент коллекции с соответствующим полем Color color и высветится результат, иначе ошибка.
     */
    @Override
    public void execute() {
        try {
            collectionManager.removeAnyByColor(Color.valueOf(commandArguments[0]));
        } catch (IllegalArgumentException ex) {
            System.err.println("Выбранной константы нет в перечислении.");
            System.out.println("Список всех констант:");
            for (Color color : Color.values()) {
                System.out.println(color);
            }
        } catch (ArrayIndexOutOfBoundsException ex) {
            System.err.println("Не указаны аргументы команды.");
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
        return "удаляет случайный элемент коллекции, поля Color которого равно указанному при вводе в качестве атрибута";
    }
}
