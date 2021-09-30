package ru.nkulakov.commands;

import ru.nkulakov.collection.CollectionManager;
import ru.nkulakov.collection.CollectionManagerImpl;
import ru.nkulakov.commands.abstr.Command;
import ru.nkulakov.commands.abstr.CommandContainer;
import ru.nkulakov.commands.abstr.InvocationStatus;
import ru.nkulakov.database.CollectionDatabaseHandler;
import ru.nkulakov.database.UserData;
import ru.nkulakov.exceptions.CannotExecuteCommandException;
import ru.nkulakov.file.DragonFieldsReader;
import ru.nkulakov.io.UserIO;

import java.io.PrintStream;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Locale;
import java.util.concurrent.locks.Lock;

/**
 * Класс, через который осуществляется исполнение команд. Хранит коллекции всех существующих команд.
 */
public class CommandInvokerImpl implements CommandInvoker {

    /**
     * Хранит объект класса ExecuteScript.Script.
     *
     * @see ExecuteScriptCommand
     */
    ExecuteScriptCommand.Script script;
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
     * @see CollectionManagerImpl
     */
    private CollectionManager collectionManager;
    private CollectionDatabaseHandler cdh;
    /**
     * Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     *
     * @see DragonFieldsReader
     */
    private DragonFieldsReader dragonFieldsReader; //TODO убрать DragonFieldsReader
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

    private Lock locker;

    /**
     * Конструктор класса. Внутри вызывается метод putCommands, добавляющий команды в коллекции команд, создается новый объект класса ExecuteScript.Script.
     *
     * @param userIO читает данные из указанного потока.
     */
    public CommandInvokerImpl(UserIO userIO, DragonFieldsReader dragonFieldsReader) { //для клиента
        this.clientCommands = new HashMap<>();
        this.userIO = userIO;
        this.dragonFieldsReader = dragonFieldsReader;

        this.script = new ExecuteScriptCommand.Script();
        this.putClientCommands();
        System.out.println("Элементы коллекции для клиента были загружены.");

    }

    /**
     * Конструктор класса, предназначенный для исполнения скрипта на клиенте.
     *
     * @param userIO             читает данные из указанного потока.
     * @param dragonFieldsReader осуществляет чтение полей, валидацию и преобразование в объект класса Dragon.
     * @param script             скрипт, хранит пути до файлов, из которых считывать команды.
     */
    public CommandInvokerImpl(UserIO userIO, DragonFieldsReader dragonFieldsReader, ExecuteScriptCommand.Script script) {
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
     */
    public CommandInvokerImpl(CollectionManager collectionManager, CollectionDatabaseHandler cdh, Lock locker) {
        this.serverCommands = new HashMap<>();

        this.collectionManager = collectionManager;

        this.cdh = cdh;
        this.putServerCommands();
        this.locker = locker;
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

        clientCommands.put("insert", new InsertElementCommand(userIO));
        clientCommands.put("update", new UpdateElementCommand(userIO));
        clientCommands.put("remove_key", new RemoveElementCommand());
        clientCommands.put("execute_script", new ExecuteScriptCommand(userIO, script));
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
        serverCommands.put("clear", new ClearCommand(collectionManager, cdh));//y
//        serverCommands.put("save", new SaveDragonArrCommand(collectionManager, inp));//y
        serverCommands.put("average_of_age", new AverageAgeCommand(collectionManager));//y
        serverCommands.put("max_by_age", new MaxElementByAgeCommand(collectionManager));//y
        serverCommands.put("help", new HelpCommand(serverCommands));//y

        serverCommands.put("insert", new InsertElementCommand(collectionManager, cdh));//y
        serverCommands.put("update", new UpdateElementCommand(collectionManager, cdh));//y
        serverCommands.put("remove_key", new RemoveElementCommand(collectionManager, cdh));//y
        serverCommands.put("replace_if_greater", new ReplaceIfGreaterCommand(collectionManager, cdh));//y
        serverCommands.put("replace_if_lower", new ReplaceIfLowerCommand(collectionManager, cdh));//y
        serverCommands.put("remove_lower_key", new RemoveLowerKeyCommand(collectionManager, cdh));
        serverCommands.put("remove_any_by_color", new RemoveAnyByColorCommand(collectionManager, cdh));
        serverCommands.put("execute_script", new ExecuteScriptCommand(collectionManager, cdh));
    }

    /**
     * Метод, который определяет из полученной строки команду со стороны клиента, исполняет ее и передает ей необходимые аргументы.
     * Если команда не распознана, то в заданный поток вывода выводится соответствующее сообщение.
     *
     * @param firstCommandLine Первая строка команды, где хранится само ее название и переданные на этой же строке аргументы.
     * @param printStream      поток вывода, куда следует записывать информацию  при исполнеии команды.
     * @return boolean: true - команда исполнена, false - команда не исполнена.
     */
    public boolean executeClient(String firstCommandLine, PrintStream printStream, UserData userData) {

        String[] words = firstCommandLine.trim().split("\\s+");
        String[] arguments = Arrays.copyOfRange(words, 1, words.length);

        try {
            if (clientCommands.containsKey(words[0].toLowerCase(Locale.ROOT))) {
                Command command;
                command = clientCommands.get(words[0].toLowerCase(Locale.ROOT));

                command.execute(arguments, InvocationStatus.CLIENT, printStream, userData, null);
                lastCommandContainer = new CommandContainer(command.getName(), command.getResult());
                return true;
            }
        } catch (NullPointerException ex) {
            ex.printStackTrace();
        } catch (CannotExecuteCommandException ex) {
            printStream.println(ex.getMessage());
        } catch (SQLException ignored) {
        }
        return false;
    }

    /**
     * Метод, который определяет из полученной строки команду со стороны сервера, исполняет ее и передает ей необходимые аргументы.
     *
     * @param firstCommandLine имя команды, аргументы.
     * @param result           данные, необходимые для исполнения серверной части команды, полученные от клиента.
     * @param printStream      поток вывода, куда следует записывать информацию  при исполнеии команды.
     * @return boolean: true - команда исполнена, false - команда не исполнена.
     */
    public boolean executeServer(String firstCommandLine, ArrayList<Object> result, PrintStream printStream, UserData userData) {

        String[] words = firstCommandLine.trim().split("\\s+");
        String[] arguments = Arrays.copyOfRange(words, 1, words.length);
        try {
            if (serverCommands.containsKey(words[0].toLowerCase(Locale.ROOT))) {
                Command command;
                command = serverCommands.get(words[0].toLowerCase(Locale.ROOT));

                command.setResult(result);
                command.execute(arguments, InvocationStatus.SERVER, printStream, userData, locker); //throws CannotExecuteCommandException
                return true;
            }
        } catch (NullPointerException ex) {
            printStream.println("Команда " + words[0] + " не распознана, для получения справки введите команду help");

        } catch (CannotExecuteCommandException ex) {
            System.out.println(ex.getMessage());
        } catch (SQLException ex) {
            System.out.println("Ошибка доступа к базе данных или недопустимый запрос. Команда не была исполнена.\n" + ex.getMessage());
        }
        return false;
    }

    /**
     * Метод, возвращающий созданный контейнер с командой.
     *
     * @return CommandContainer - контейнер с командой.
     */
    public CommandContainer getLastCommandContainer() {
        return lastCommandContainer;
    }

    public CollectionManager getCollectionManager() {
        return collectionManager;
    }

    public void reinitCommandsUserIO(UserIO userIO) {
        InsertElementCommand insertElementCommand = (InsertElementCommand) clientCommands.get("insert");
        insertElementCommand.setUserIO(userIO);

        ExecuteScriptCommand executeScriptCommand = (ExecuteScriptCommand) clientCommands.get("execute_script");
        executeScriptCommand.setUserIO(userIO);

        UpdateElementCommand updateElementCommand = (UpdateElementCommand) clientCommands.get("update");
        updateElementCommand.setUserIO(userIO);

        ReplaceIfGreaterCommand replaceIfGreaterCommand = (ReplaceIfGreaterCommand) clientCommands.get("replace_if_greater");
        replaceIfGreaterCommand.setUserIO(userIO);

        ReplaceIfLowerCommand replaceIfLowerCommand = (ReplaceIfLowerCommand) clientCommands.get("replace_if_lower");
        replaceIfLowerCommand.setUserIO(userIO);
    }
}
