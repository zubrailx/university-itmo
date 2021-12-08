package commands;

import collection.CollectionManager;
import exceptions.RecoursiveCallException;
import file.DragonFieldsReader;
import io.UserIO;

import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class ExecuteScriptCommand implements CommandWithArguments {
    /**
     * Массив, хранящий аргументы команды.
     */
    private String[] commandArguments;
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
     * Конструктор класса.
     *
     * @param collectionManager  Хранит ссылку на созданный в объекте Application объект CollectionManager.
     * @param dragonFieldsReader Хранит ссылку на объект, осуществляющий чтение полей из указанного в userIO потока ввода.
     * @param script             Хранит объект класса ExecuteScript.Script, из которого нам следует получить список адресов скриптов.
     */
    public ExecuteScriptCommand(CollectionManager collectionManager, DragonFieldsReader dragonFieldsReader, Script script) {
        this.collectionManager = collectionManager;
        this.dragonFieldsReader = dragonFieldsReader;
        this.script = script;
    }

    /**
     * Метод, исполняющий команду. В коллекцию scripts при начале исполнения добавляется адрес скрипта, далее идет само его исполнение, в конце адрес файла удаляется. В случае ошибки выводится соответствующее сообщение.
     */
    @Override
    public void execute() {
        try {
            if (commandArguments.length == 1) {
                scriptPath = commandArguments[0];
                if (script.scriptPaths.contains(scriptPath)) throw new RecoursiveCallException();
                else script.putScript(scriptPath);
            } else throw new IllegalArgumentException();

            File ioFile = new File(scriptPath);
            if (!ioFile.canWrite() || ioFile.isDirectory() || !ioFile.isFile()) throw new IOException();

            FileInputStream fileInputStream = new FileInputStream(scriptPath);
            InputStreamReader inputStreamReader = new InputStreamReader(fileInputStream);
            Scanner scanner = new Scanner(inputStreamReader);
            userIO = new UserIO(scanner);
            CommandInvoker commandInvoker = new CommandInvoker(collectionManager, userIO, dragonFieldsReader, script);

            while (scanner.hasNext()) {
                commandInvoker.execute(scanner.nextLine());
            }
        } catch (FileNotFoundException ex) {
            System.err.println("Файл скрипта не найден");
        } catch (NullPointerException ex) {
            System.err.println("Не выбран файл, из которого читать скрипт");
        } catch(IOException ex){
            System.err.println("Доступ к файлу невозможен"+ ex.getMessage());
        }  catch (IllegalArgumentException ex) {
            System.err.println("скрипт не передан в качестве аргумента команды, либо кол-во агрументов больше 1");
        } catch (RecoursiveCallException ex) {
            System.err.println("Скрипт " + scriptPath + " уже существует (Рекурсивный вызов)");
        }
        script.removeScript(scriptPath);

    }

    /**
     * @param commandArguments Список аргументов команды.
     * @see CommandWithArguments
     */
    @Override
    public void getCommandArguments(String[] commandArguments) {
        this.commandArguments = commandArguments;
    }

    /**
     * @return Описание команды execute_script.
     * @see Command
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
        private ArrayList<String> scriptPaths = new ArrayList<String>();

        /**
         * Метод, добавляющий скрипт в коллекцию.
         *
         * @param scriptPath Адрес скрипта, требующий добавляения в коллекцию.
         */
        public void putScript(String scriptPath) {
            scriptPaths.add(scriptPath);
        }

        /**
         * Метод, убирающий скрипт из коллекции.
         *
         * @param scriptPath Адрес скрипта, требующий удаления из коллекции.
         */
        public void removeScript(String scriptPath) {
            scriptPaths.remove(scriptPath);
        }
    }
}
