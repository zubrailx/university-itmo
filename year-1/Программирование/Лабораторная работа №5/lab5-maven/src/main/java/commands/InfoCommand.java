package commands;

import collection.CollectionManager;

public class InfoCommand implements Command {
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;

    /**
     * Конструктор класса.
     *
     * @param collectionManager Хранит ссылку на созданный в объекте Application объект CollectionManager.
     */
    public InfoCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. Выводит описание коллекции TreeMap экземпляров Dragon.
     */
    @Override
    public void execute() {
        collectionManager.info();
    }

    /**
     * @return Возвращает описание команды.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "команда получает информацию о коллекции (тип, дата инициализации, количество элементов, тип элементов коллекции)";
    }
}
