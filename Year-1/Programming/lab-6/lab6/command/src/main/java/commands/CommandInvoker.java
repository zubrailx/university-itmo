package commands;

import collection.CollectionManager;
import commands.abstr.Command;
import commands.abstr.CommandContainer;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;
import file.DragonFieldsReader;
import io.UserIO;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Locale;

/**
 * Класс, через который осуществляется исполнение команд. Хранит коллекции всех существующих команд.
 */
public class CommandInvoker {

    /**
     * Коллекция команд для клиента.
     */
    private HashMap<String, Command> clientCommands;

    /**
     * Коллекция команд для сервера.
     */
    private HashMap<String, Command> serverCommands;

    /**
     * Хранит ссылку на менеджер коллекции.
     *
     * @see CollectionManager
     */
    private CollectionManager collectionManager;

    /**
     * Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     *
     * @see DragonFieldsReader
     */
    private DragonFieldsReader dragonFieldsReader;

    /**
     * Хранит объект класса ExecuteScript.Script.
     *
     * @see ExecuteScriptCommand
     */
    ExecuteScriptCommand.Script script;

    /**
     * Хранит объект, предназначенный для чтения данных из указанного потока ввода.
     *
     * @see UserIO
     */
    private UserIO userIO;

    /**
     * Контейнер с командой.
     */
    private CommandContainer lastCommandContainer;

    /**
     * Файл, куда сохранять коллекцию в случае завершения работы сервера.
     */
    private String inputFile;

    /**
     * Конструктор класса. Внутри вызывается метод putCommands, добавляющий команды в коллекции команд, создается новый объект класса ExecuteScript.Script.
     *
     * @param userIO читает данные из указанного потока.
     */
    public CommandInvoker(UserIO userIO) { //для клиента
        this.clientCommands = new HashMap<>();
        this.userIO = userIO;
        this.dragonFieldsReader = new DragonFieldsReader(userIO);

        this.script = new ExecuteScriptCommand.Script();
        this.putClientCommands();
        System.out.println("Элементы коллекции для клиента были загружены.");
    }

    /**
     * Конструктор класса, предназначенный для исполнения скрипта на клиенте.
     * @param userIO читает данные из указанного потока.
     * @param dragonFieldsReader осуществляет чтение полей, валидацию и преобразование в объект класса Dragon.
     * @param script скрипт, хранит пути до файлов, из которых считывать команды.
     */
    public CommandInvoker(UserIO userIO, DragonFieldsReader dragonFieldsReader, ExecuteScriptCommand.Script script) {
        this.clientCommands = new HashMap<>();

        this.userIO = userIO;
        this.dragonFieldsReader = dragonFieldsReader;
        this.script = script;

        this.putClientCommands();
    }

    /**
     * Конструктор класса, предназначенный для сервера.
     *
     * @param collectionManager менеджер коллекции.
     * @param inputFile файл, куда следует сохранять коллекцию по завершении работы сервера.
     */
    public CommandInvoker(CollectionManager collectionManager, String inputFile) {
        this.serverCommands = new HashMap<>();

        this.collectionManager = collectionManager;

        this.inputFile = inputFile;

        script = new ExecuteScriptCommand.Script();

        this.putServerCommands();
        System.out.println("Элементы коллекции для сервера были загружены");
    }

    /**
     * Конструктор класса, предназначенный для сервера.
     * @param collectionManager менеджер коллекции.
     */
    public CommandInvoker(CollectionManager collectionManager) {
        this.serverCommands = new HashMap<>();

        this.collectionManager = collectionManager;

        this.putServerCommands();
    }

    /**
     * Метод, добавляющий клиентские команды в соответствующую коллекции.
     */
    private void putClientCommands() {
        clientCommands.put("info", new InfoCommand());
        clientCommands.put("show", new ShowCommand());
        clientCommands.put("clear", new ClearCommand());
        clientCommands.put("exit", new ExitCommand());
        clientCommands.put("average_of_age", new AverageAgeCommand());
        clientCommands.put("max_by_age", new MaxElementByAgeCommand());
        clientCommands.put("help", new HelpCommand(clientCommands));

        clientCommands.put("insert", new InsertElementCommand(dragonFieldsReader));
        clientCommands.put("update", new UpdateElementCommand(userIO));
        clientCommands.put("remove_key", new RemoveElementCommand());
        clientCommands.put("execute_script", new ExecuteScriptCommand(userIO, dragonFieldsReader, script));
        clientCommands.put("replace_if_greater", new ReplaceIfGreaterCommand(userIO));
        clientCommands.put("replace_if_lower", new ReplaceIfLowerCommand(userIO));
        clientCommands.put("remove_lower_key", new RemoveLowerKeyCommand());
        clientCommands.put("remove_any_by_color", new RemoveAnyByColorCommand());
    }

    /**
     * Метод, добавляющий серверные команды в соответствующую коллекцию.
     */
    private void putServerCommands() {
        serverCommands.put("info", new InfoCommand(collectionManager));//y
        serverCommands.put("show", new ShowCommand(collectionManager));//y
        serverCommands.put("clear", new ClearCommand(collectionManager));//y
        serverCommands.put("save", new SaveDragonArrCommand(collectionManager, inputFile));//y
        serverCommands.put("average_of_age", new AverageAgeCommand(collectionManager));//y
        serverCommands.put("max_by_age", new MaxElementByAgeCommand(collectionManager));//y
        serverCommands.put("help", new HelpCommand(serverCommands));//y

        serverCommands.put("insert", new InsertElementCommand(collectionManager));//y
        serverCommands.put("update", new UpdateElementCommand(collectionManager));//y
        serverCommands.put("remove_key", new RemoveElementCommand(collectionManager));//y
        serverCommands.put("replace_if_greater", new ReplaceIfGreaterCommand(collectionManager));//y
        serverCommands.put("replace_if_lower", new ReplaceIfLowerCommand(collectionManager));//y
        serverCommands.put("remove_lower_key", new RemoveLowerKeyCommand(collectionManager));
        serverCommands.put("remove_any_by_color", new RemoveAnyByColorCommand(collectionManager));
        serverCommands.put("execute_script", new ExecuteScriptCommand(collectionManager));
    }

    /**
     * Метод, который определяет из полученной строки команду со стороны клиента, исполняет ее и передает ей необходимые аргументы.
     * Если команда не распознана, то в заданный поток вывода выводится соответствующее сообщение.
     *
     * @param firstCommandLine Первая строка команды, где хранится само ее название и переданные на этой же строке аргументы.
     * @param printStream поток вывода, куда следует записывать информацию  при исполнеии команды.
     *
     * @return boolean: true - команда исполнена, false - команда не исполнена.
     */
    public boolean executeClient(String firstCommandLine, PrintStream printStream) {

        String[] words = firstCommandLine.trim().split("\\s+");
        String[] arguments = Arrays.copyOfRange(words, 1, words.length);

        try {
            if (clientCommands.containsKey(words[0].toLowerCase(Locale.ROOT))) {
                Command command;
                command = clientCommands.get(words[0].toLowerCase(Locale.ROOT));

                command.execute(arguments, InvocationStatus.CLIENT, printStream);
                lastCommandContainer = new CommandContainer(command.getName(), command.getResult());
                return true;
            }
        } catch (NullPointerException ex) {
            ex.printStackTrace();
        } catch (CannotExecuteCommandException ex) {
            printStream.println(ex.getMessage());
        }
        return false;
    }

    /**
     * Метод, который определяет из полученной строки команду со стороны сервера, исполняет ее и передает ей необходимые аргументы.
     *
     * @param firstCommandLine имя команды, аргументы.
     * @param result данные, необходимые для исполнения серверной части команды, полученные от клиента.
     * @param printStream поток вывода, куда следует записывать информацию  при исполнеии команды.
     *
     * @return boolean: true - команда исполнена, false - команда не исполнена.
     */
    public boolean executeServer(String firstCommandLine, ArrayList<Object> result, PrintStream printStream) {

        String[] words = firstCommandLine.trim().split("\\s+");
        String[] arguments = Arrays.copyOfRange(words, 1, words.length);
        try {
            if (serverCommands.containsKey(words[0].toLowerCase(Locale.ROOT))) {
                Command command;
                command = serverCommands.get(words[0].toLowerCase(Locale.ROOT));

                command.setResult(result);
                command.execute(arguments, InvocationStatus.SERVER, printStream); //throws CannotExecuteCommandException
                return true;
            }
        } catch (NullPointerException ex) {
            System.out.println("Команда " + words[0] + " не распознана, для получения справки введите команду help");
            ex.printStackTrace();
        } catch (CannotExecuteCommandException ex) {
            System.out.println(ex.getMessage());
        }
        return false;
    }

    /**
     * Метод, возвращающий созданный контейнер с командой.
     * @return CommandContainer - контейнер с командой.
     */
    public CommandContainer getLastCommandContainer() {
        return lastCommandContainer;
    }
}
