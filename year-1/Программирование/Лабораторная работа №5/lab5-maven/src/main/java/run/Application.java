package run;

import collection.CollectionManager;
import collection.Dragon;
import commands.CommandInvoker;
import file.DragonFieldsReader;
import file.FileManager;
import file.XmlParser;
import io.UserIO;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.NoSuchElementException;

/**
 * Класс, через который производится запуск данного приложения.
 */
public class Application {
    /**
     * менеджер коллекций
     */
    CollectionManager collectionManager;
    /**
     * менеджер файлов
     */
    FileManager fileManager;
    /**
     * XML парсер
     */
    XmlParser xmlParser;
    /**
     * хранит ссылку на объект, производящий чтение и вывод команд
     */
    UserIO userIO;
    /**
     * хранит ссылку на объект, который производит запуск выбранных команд
     */
    CommandInvoker commandInvoker;
    /**
     * хранит ссылку на объект, производящий чтение полей класса Dragon
     */
    DragonFieldsReader dragonFieldsReader;

    /**
     * Метод, выполняющий запуск программы. Через него происходит работа всей программы.
     *
     * @param inputFile имя файла, откуда следует читать объекты коллекции, представленные в формате XML
     */
    public void start(String inputFile) {

        collectionManager = new CollectionManager();
        fileManager = new FileManager();
        xmlParser = new XmlParser();
        userIO = new UserIO();

        dragonFieldsReader = new DragonFieldsReader(userIO);

        this.commandInvoker = new CommandInvoker(collectionManager, userIO, inputFile, dragonFieldsReader);

        try {

            File ioFile = new File(inputFile);
            if (!ioFile.canWrite() || ioFile.isDirectory() || !ioFile.isFile()) throw new IOException();
            String file = fileManager.readFromFile(inputFile);

            Dragon[] dragons = xmlParser.parseToCollection(new InputSource(new StringReader(file)));
            for (Dragon dragon : dragons) {
                collectionManager.insert(dragon.getId(), dragon);
            }
            userIO.printCommandText("Элементы коллекций из указанного файла были загружены\n");
        } catch (ParserConfigurationException | IOException | SAXException e) {
            System.err.println("По указанному адресу нет подходящего файла");
            System.exit(0);
        }
        try {
            cycle();
        } catch (NoSuchElementException ex) {
            System.err.println("Ctrl + D exception");
        }
    }

    /**
     * Метод, выполняющий циклическое чтение команд из строки ввода
     */
    public void cycle() {
        userIO.printCommandText("Программа была запущена.\n");
        while (true) {
            userIO.printCommandText("\nВведите название команды:\n");
            userIO.printPreamble();
            String line = userIO.readLine();
            commandInvoker.execute(line);
        }
    }
}
