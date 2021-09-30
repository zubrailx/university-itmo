package commands;

/**
 * Класс команды, которая завершает работу программы.
 */
public class ExitCommand implements Command {
    /**
     * Конструктор класса.
     */
    public ExitCommand() {
    }

    /**
     * Метод, завершающий работу программы. При завершении выводит соответствующее сообщение.
     */
    @Override
    public void execute() {
        System.out.println("Завершение работы программы.");
        System.exit(0);
    }

    /**
     * @return Метод, возвращающий строку описания программы.
     * @see Command
     */
    @Override
    public String getDescription() {
        return "завершает работу программы";
    }
}
