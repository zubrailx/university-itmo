package ru.nkulakov;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Main {

    private static final Logger rootLogger = LogManager.getRootLogger();

    public static void main(String[] args) {

        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        Application application = new Application();
        if (args.length > 0) {
            try {
                int port = Integer.parseInt(args[0]);
                if (port <= 0) {
                    throw new IllegalArgumentException();
                } else {
                    System.out.println("Порт установлен: " + port);
                    application.start(port);
                }
            } catch (NumberFormatException ex) {
                rootLogger.error("Введенный порт должен быть числом. Завершение работы сервера.");
            } catch (IllegalArgumentException ex) {
                rootLogger.error("Порт должен быть больше нуля.");
            }
        } else {
            rootLogger.error("Должен быть указан порт, к которому подключаться");
        }
    }
}
