package ru.nkulakov.backend;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.backend.client.Client;
import ru.nkulakov.backend.client.ClientImpl;
import ru.nkulakov.backend.command.CommandProcessor;
import ru.nkulakov.backend.command.CommandProcessorImpl;
import ru.nkulakov.backend.connection.ClientConnection;
import ru.nkulakov.backend.connection.ClientConnectionImpl;
import ru.nkulakov.backend.reader.RequestReader;
import ru.nkulakov.backend.reader.RequestReaderImpl;
import ru.nkulakov.backend.sender.ResponseSender;
import ru.nkulakov.backend.sender.ResponseSenderImpl;
import ru.nkulakov.commands.CommandInvoker;
import ru.nkulakov.commands.CommandInvokerImpl;
import ru.nkulakov.file.DragonFieldsReader;
import ru.nkulakov.gui.GuiManagerImpl;
import ru.nkulakov.io.UserIO;

import java.io.IOException;

/**
 * Класс, через который производится подготовка к запуску клиента.
 */
public class Application {

    private static final Logger rootLogger = LogManager.getRootLogger();

    /**
     * Метод, выполняющий запуск программы. Через него происходит работа всей программы.
     */
    public Application() {
        rootLogger.info("Конструктор класса backend.Application был загружен.");
    }

    public void start() {

        UserIO userIO = new UserIO();
        DragonFieldsReader dragonFieldsReader = new DragonFieldsReader(userIO);

        CommandInvoker commandInvoker = new CommandInvokerImpl(userIO, dragonFieldsReader);

        try {
            ClientConnection clientConnection = new ClientConnectionImpl();

            ResponseSender responseSender = new ResponseSenderImpl(clientConnection.getClientChannel());
            RequestReader requestReader = new RequestReaderImpl(clientConnection.getClientChannel());
            CommandProcessor commandProcessor = new CommandProcessorImpl(commandInvoker);

            Client client = new ClientImpl(requestReader, responseSender, commandProcessor);
            new GuiManagerImpl(client);

        } catch (IOException ex) {
            rootLogger.error("Не удалось запустить клиента из-за ошибки подключения к порту.");
        }
    }
}