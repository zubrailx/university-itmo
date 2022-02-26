package commands;

import collection.CollectionManager;
import commands.abstr.Command;
import commands.abstr.CommandContainer;
import commands.abstr.InvocationStatus;
import exceptions.CannotExecuteCommandException;
import exceptions.RecursiveCallException;
import file.DragonFieldsReader;
import io.UserIO;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;

public class ExecuteScriptCommand extends Command {

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
     * Поле, хранящее адрес файла, из которого следует исполнять скрипт.
     */
    private String scriptPath;

    /**
     * Поле, хранящее объект класса ExecuteScript.Script.
     */
    private Script script;

    /**
     * Конструктор класса, предназначенный для клиентской части команды.
     *
     * @param userIO читает данные из указанного потока.
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     * @param script             Хранит объект класса ExecuteScript.Script, из которого нам следует получить список адресов скриптов.
     */
    public ExecuteScriptCommand(UserIO userIO, DragonFieldsReader dragonFieldsReader, Script script) {
        super("execute_script");
        this.userIO = userIO;
        this.dragonFieldsReader = dragonFieldsReader;
        this.script = script;
    }

    /**
     * Конструктор класса, предназначенный для серверной части команды.
     * @param collectionManager менеджер коллекции.
     */
    public ExecuteScriptCommand(CollectionManager collectionManager) {
        this.collectionManager = collectionManager;
    }

    /**
     * Метод, исполняющий команду. В коллекцию scripts при начале исполнения добавляется адрес скрипта, далее идет само его исполнение, в конце адрес файла удаляется. В случае ошибки выводится соответствующее сообщение.
     * На сервере полученные данные переводятся в команды с необходимыми аргументами, которые потом исполняются.
     *
     * @param arguments аргументы команды.
     * @param invocationEnum режим, с которым должна быть исполнена команда.
     * @param printStream поток вывода.
     */
    public void execute(String[] arguments, InvocationStatus invocationEnum, PrintStream printStream) throws CannotExecuteCommandException {
        if (invocationEnum.equals(InvocationStatus.CLIENT)) {
            result = new ArrayList<>();
            try {
                if (arguments.length == 1) {
                    scriptPath = arguments[0].trim();
                    if (script.scriptPaths.contains(scriptPath)) throw new RecursiveCallException(scriptPath);
                    else script.putScript(scriptPath);
                } else throw new IllegalArgumentException();

                File ioFile = new File(scriptPath);
                if (!ioFile.canWrite() || ioFile.isDirectory() || !ioFile.isFile()) throw new IOException();

                FileInputStream fileInputStream = new FileInputStream(scriptPath);
                InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
                Scanner scanner = new Scanner(inputStreamReader);
                userIO = new UserIO(scanner);
                CommandInvoker commandInvoker = new CommandInvoker(userIO, dragonFieldsReader, script);

                super.result.add(scriptPath);//добавлен адрес, из которого была исполнена команда execute_script

                PrintStream nullStream = (new PrintStream(new OutputStream() {
                    public void write(int b) {
                        //DO NOTHING
                    }
                }));

                while (scanner.hasNext()) {
                    if (commandInvoker.executeClient(scanner.nextLine(), nullStream)) {
                        super.result.add(commandInvoker.getLastCommandContainer());
                    }
                }
                script.removeScript(scriptPath);
                return;
            } catch (FileNotFoundException ex) {
                printStream.println("Файл скрипта не найден");
            } catch (NullPointerException ex) {
                printStream.println("Не выбран файл, из которого читать скрипт");
            } catch (IOException ex) {
                printStream.println("Доступ к файлу невозможен");
            } catch (IllegalArgumentException ex) {
                printStream.println("скрипт не передан в качестве аргумента команды, либо кол-во агрументов больше 1");
            } catch (RecursiveCallException ex) {
                printStream.println("Скрипт " + scriptPath + " уже существует (Рекурсивный вызов)");
            }
            script.removeScript(scriptPath);
            throw new CannotExecuteCommandException("Принудительное завершение работы команды execute_script");
        } else if (invocationEnum.equals(InvocationStatus.SERVER)) {
            printStream.println("Файл, который исполняется скриптом: " + this.getResult().get(0));
            Object[] arr = result.toArray();
            arr = Arrays.copyOfRange(arr, 1, arr.length);
            CommandContainer[] containerArray = Arrays.copyOf(arr, arr.length, CommandContainer[].class);

            CommandInvoker commandInvoker = new CommandInvoker(collectionManager);
            for (CommandContainer command : containerArray) {
                commandInvoker.executeServer(command.getName(), command.getResult(), printStream);
            }
        }
    }

    /**
     * @return Описание команды execute_script.
     *
     * @see HelpCommand
     */
    @Override
    public String getDescription() {
        return "выполняет команды, описанные в скрипте";
    }

    /**
     * Статический класс, в котором хранится коллекция адресов скриптов.
     */
    static class Script {

        /**
         * Коллекция, в которой хранятся адреса запущенных скриптов.
         */
        private final ArrayList<String> scriptPaths = new ArrayList<>();

        /**
         * Метод, добавляющий скрипт в коллекцию.
         *
         * @param scriptPath адрес скрипта, требующий добавляения в коллекцию.
         */
        public void putScript(String scriptPath) {
            scriptPaths.add(scriptPath);
        }

        /**
         * Метод, убирающий скрипт из коллекции.
         *
         * @param scriptPath адрес скрипта, требующий удаления из коллекции.
         */
        public void removeScript(String scriptPath) {
            scriptPaths.remove(scriptPath);
        }
    }
}
