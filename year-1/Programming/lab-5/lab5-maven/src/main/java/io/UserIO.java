package io;

import java.util.Scanner;

public class UserIO {
    /**
     * хранит ссылку на Scanner, производящий чтение данных из указанного места
     */
    Scanner scanner;

    /**
     * Конструктор класса без параметров. При вызове в поле scanner присваивается Scanner, производящий чтение из стандартного потока ввода с кодировкой UTF-8
     */
    public UserIO() {
        scanner = new Scanner(System.in, "UTF-8");
    }

    /**
     * Конструктор класса с параметрами
     *
     * @param scanner хранит ссылку на объект типа Scanner
     */
    public UserIO(Scanner scanner) {
        this.scanner = scanner;
    }

    /**
     * Метод, производящий чтение данных из места, на которое указывает объект, на который ссылается поле scanner данного объекта
     *
     * @return возвращает прочитанную строку
     */
    public String readLine() {
        return scanner.nextLine();
    }

    /**
     * Метод, выводящий текст, принадлежащий команде в стандартный поток вывода
     *
     * @param str строка, которую следует вывести в стандартный поток вывода
     */
    public void printCommandText(String str) {
        System.out.print(str);
    }

    /**
     * Метод, выводящий текст ошибки, принадлежащий команде в стандартный поток вывода ошибок
     *
     * @param str строка, которую следует вывести в стандартный поток вывода ошибок
     */
    public void printCommandError(String str) {
        System.err.println(str);
    }

    /**
     * Метод, выводящий символ стрелки вправо перед запросом ввода команды
     */
    public void printPreamble() {
        System.out.print(">");
    }

}
