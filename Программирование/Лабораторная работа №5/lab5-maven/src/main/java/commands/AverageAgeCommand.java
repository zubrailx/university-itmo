package commands;

import collection.CollectionManager;

/**
 * Класс команды, которая выводит средний возраст всех элементов коллекции.
 */
public class AverageAgeCommand implements Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Конструктор класса.
     *
     * @param collectionManager хранит ссылку на созданный в объекте Application объект CollectionManager.
     */
    public AverageAgeCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. Выводит сообщение если коллекция пуста.
     */
    @Override
    public void execute() {
        Double avAge = collectionManager.averageAge();
        if (avAge == 0) System.out.println("Коллекция пуста.");
        else {
            System.out.print("Средний возраст всех элементов коллекции: ");
            System.out.printf("%.2f\n", avAge);
        }
    }

    /**
     * @return Описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "выводит средний возраст элементов из коллекции";
    }
}
