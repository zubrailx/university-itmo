package commands;

import collection.CollectionManager;

public class MaxElementByAgeCommand implements Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Конструктор класса.
     * @param collectionManager Хранит ссылку на созданный в объекте Application объект CollectionManager.
     */
    public MaxElementByAgeCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В случае успешного выполнения высветится элемент коллекции, значение поля age которого максимально, иначе ошибка.
     */
    @Override
    public void execute() {
        try {
            String str = collectionManager.maxAge().toString();
            System.out.print("Элемент с максимальный возрастов в коллекции:\n" + str);
        } catch (NullPointerException ex) {
            System.out.println("Коллекция пуста.");
        }
    }

    /**
     * @see Command
     * @return Возвращает описание команды.
     */
    @Override
    public String getDescription() {
        return "выводит информацию об элементе коллекции, с максимальным возрастом";
    }
}
