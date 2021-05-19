package commands;

import collection.CollectionManager;
import file.DragonFieldsReader;
import io.UserIO;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Locale;

/**
 * Класс, через который осуществляется исполнение команд. Хранит коллекции всех существующих команд.
 */
public class CommandInvoker {
    /**
     * Коллекция команд без дополнительных аргументов, которые записываются с новой строки.
     */
    private HashMap<String, Command> commandsWithoutArguments;
    /**
     * Коллекция команд с дополнительными аргументами, которые записываются с новой строки.
     */
    private HashMap<String, CommandWithArguments> commandsWithArguments;
    /**
     * Поле, хранящее ссылку на объект класса CollectionManager.
     */
    private CollectionManager collectionManager;
    /**
     * Поле, хранящее ссылку на объект класса UserIO.
     */
    private UserIO userIO;
    /**
     * Поле, хранящее строку, в которой записан адрес файла, куда следует сохранять полученную коллекцию (экземпляры коллекции).
     */
    private String inputFile;
    /**
     * Поле, хранящее ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    private DragonFieldsReader dragonFieldsReader;
    /**
     * Поле, хранящее объект класса ExecuteScript.Script.
     */
    ExecuteScriptCommand.Script script;

    /**
     * Конструктор класса. Внутри вызывается метод putCommands, добавляющий команды в коллекции команд, создается новый объект класса ExecuteScript.Script.
     *
     * @param collectionManager  Хранит ссылку созданный в объекте Application объект CollectionManager.
     * @param userIO             Хранит ссылку на объект класса UserIO.
     * @param inputFile          Хранит строку, в которой записан адрес файла, куда следует сохранять полученную коллекцию (экземпляры коллекции).
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     */
    public CommandInvoker(CollectionManager collectionManager, UserIO userIO, String inputFile, DragonFieldsReader dragonFieldsReader) {
        this.collectionManager = collectionManager;
        this.userIO = userIO;
        this.inputFile = inputFile;
        this.dragonFieldsReader = dragonFieldsReader;

        commandsWithoutArguments = new HashMap<>();
        commandsWithArguments = new HashMap<>();
        this.script = new ExecuteScriptCommand.Script();

        this.putCommands();
    }

    /**
     * Конструктор класса. Внутри вызывается метод putCommands, инициализируется поле, в которое присваивается существующий объект класса ExecuteScript.Script.
     *
     * @param collectionManager  Хранит ссылку на созданный в объекте Application объект CollectionManager.
     * @param userIO             Хранит ссылку на объект класса UserIO.
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     * @param script             Хранит ссылку на объект класса ExecuteScript.Script.
     */
    public CommandInvoker(CollectionManager collectionManager, UserIO userIO, DragonFieldsReader dragonFieldsReader, ExecuteScriptCommand.Script script) {
        this.collectionManager = collectionManager;
        this.userIO = userIO;
        this.dragonFieldsReader = dragonFieldsReader;

        commandsWithoutArguments = new HashMap<>();
        commandsWithArguments = new HashMap<>();
        this.script = script;
        this.putCommands();
    }

    /**
     * Метод, добавляющий команды в соответствующие им коллекции.
     */
    private void putCommands() {
        commandsWithoutArguments.put("info", new InfoCommand(collectionManager));//y
        commandsWithoutArguments.put("show", new ShowCommand(collectionManager));//y
        commandsWithoutArguments.put("clear", new ClearCommand(collectionManager));//y
        commandsWithoutArguments.put("save", new SaveDragonArrCommand(collectionManager, inputFile));//y
        commandsWithoutArguments.put("exit", new ExitCommand());//y
        commandsWithoutArguments.put("average_of_age", new AverageAgeCommand(collectionManager));//y
        commandsWithoutArguments.put("max_by_age", new MaxElementByAgeCommand(collectionManager));//y
        commandsWithoutArguments.put("help", new HelpCommand(commandsWithoutArguments, commandsWithArguments));//y

        commandsWithArguments.put("insert", new InsertElementCommand(collectionManager, userIO, dragonFieldsReader));//y
        commandsWithArguments.put("update", new UpdateElementCommand(collectionManager, userIO));//y
        commandsWithArguments.put("remove_key", new RemoveElementCommand(collectionManager));//y
        commandsWithArguments.put("execute_script", new ExecuteScriptCommand(collectionManager, dragonFieldsReader, script));
        commandsWithArguments.put("replace_if_greater", new ReplaceIfGreaterCommand(collectionManager, userIO));//y
        commandsWithArguments.put("replace_if_lowe", new ReplaceIfLowerCommand(collectionManager, userIO));//y
        commandsWithArguments.put("remove_lower_key", new RemoveLowerKeyCommand(collectionManager));
        commandsWithArguments.put("remove_any_by_color", new RemoveAnyByColorCommand(collectionManager));
    }

    /**
     * Метод, который определяет из полученной строки команду, исполняет ее и передает ей необходимые аргументы.
     * Если команда не распознана, то в стандартный поток вывода выводится соответствующее сообщение.
     *
     * @param firstCommandLine Первая строка команды, где хранится само ее название и переданные на этой же строке аргументы.
     */
    public void execute(String firstCommandLine) {
        String[] words = firstCommandLine.trim().split("\\s+");
        String[] args = Arrays.copyOfRange(words, 1, words.length); //проверка на размер больше 1

        if (commandsWithArguments.containsKey(words[0].toLowerCase(Locale.ROOT))) {
            CommandWithArguments command;

            command = commandsWithArguments.get(words[0].toLowerCase(Locale.ROOT));
            command.getCommandArguments(args);
            command.execute();
        } else if (commandsWithoutArguments.containsKey(words[0].toLowerCase(Locale.ROOT))) {
            Command command;

            command = commandsWithoutArguments.get(words[0].toLowerCase(Locale.ROOT));
            command.execute();
        } else {
            System.err.println("Команда " + words[0] + " не распознана, для получения справки введите команду help");
        }
    }
}
