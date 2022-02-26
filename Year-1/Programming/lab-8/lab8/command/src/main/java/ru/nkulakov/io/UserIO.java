package ru.nkulakov.io;

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
     * Конструктор класса со сканером в качестве параметра. Необходим для корректного чтения-вывода данных на-из консоли.
     *
     * @param scanner сканнер, необходимый для чтения и вывода.
     */
    public UserIO(Scanner scanner) {
        this.scanner = scanner;
    }

    /**
     * Метод, производящий чтение строки из места, на которое указывает объект, на который ссылается поле scanner данного объекта.
     *
     * @return возвращает прочитанную строку
     */
    public String readLine() {
        return scanner.nextLine();
    }

}
