package commands;

/**
 * Uнтерфейс, реализуемый командами с аргументами, записываемыми с новой строки.
 */
public interface CommandWithArguments extends Command {
    /**
     * Метод, получающий аргументы команды.
     *
     * @param arguments Аргументы команды.
     */
    void getCommandArguments(String[] arguments);
}
