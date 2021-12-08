import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class MainClient {

    private static final Logger rootLogger = LogManager.getRootLogger();

    public static void main(String[] args) {

        if (args.length != 0) {
            Application application = new Application(Integer.parseInt(args[0]));
            try {
                application.start();
            } catch (NumberFormatException ex) {
                rootLogger.warn("Значение порта должно быть целочисленным.\n Введенное значение: " + args[0]);
            }
        }
    }
}
