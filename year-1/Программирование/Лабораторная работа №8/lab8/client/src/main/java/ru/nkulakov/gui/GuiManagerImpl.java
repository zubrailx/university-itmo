package ru.nkulakov.gui;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import ru.nkulakov.backend.client.Client;
import ru.nkulakov.collection.ClientCollection;
import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.frame.PortFrame;
import ru.nkulakov.gui.locale.Localization;
import ru.nkulakov.gui.panel.LoginPanel;
import ru.nkulakov.gui.panel.MainPanel;

import javax.swing.*;
import java.awt.*;

public class GuiManagerImpl implements GuiManager {

    private static final Logger rootLogger = LogManager.getRootLogger();

    private final Client client;
    private final MainFrame mainFrame;

    public GuiManagerImpl(Client client) {
        this.client = client;

        mainFrame = new MainFrame(this);
        LoginPanel loginPanel = new LoginPanel(mainFrame);
        mainFrame.setContentPane(loginPanel);
        mainFrame.pack();
        centerFrame(mainFrame);
        mainFrame.setResizable(false);
        mainFrame.setVisible(true);
    }

    public static void centerFrame(JFrame frame) {
        Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
        int x = (int) (dim.getWidth() - frame.getWidth()) / 2;
        int y = (int) (dim.getHeight() - frame.getHeight()) / 2;
        frame.setLocation(x, y);
    }

    public int authenticate(String login, String password, boolean bool) {
        int situation = client.authenticate(login, password, bool);
        if (situation == 1) {
            rootLogger.info("Подключение клиента установлено.");
            mainFrame.setVisible(false);
            MainPanel mainPanel = new MainPanel(mainFrame);
            mainFrame.setContentPane(mainPanel);
            mainFrame.setSize(new Dimension(1000, 800));
            mainFrame.setMinimumSize(new Dimension(900, 700));
            mainFrame.setResizable(true);
            centerFrame(mainFrame);
            mainFrame.setVisible(true);
        } else {
            rootLogger.info("Подключение клиента не установлено.");
        }
        return situation;
    }

    public boolean executeCommand(String commandAdaptation) {
        switch (client.executeCommand(commandAdaptation)) {
            case 1: {
                rootLogger.info("Команда была исполнена корректно.");
                return true;
            }
            case 0: {
                rootLogger.warn("Команда НЕ была исполнена корректно.");
                JOptionPane.showMessageDialog(new PortFrame(mainFrame), Localization.getCurrentResourceBundle().getString("CommandExecutionError"));
                return false;
            }
            case -1: {
                rootLogger.warn("Ошибка подключения к серверу.");
                JOptionPane.showMessageDialog(new PortFrame(mainFrame), Localization.getCurrentResourceBundle().getString("ServerConnectionError"));
                return false;
            }
        }
        return false;
    }

    public Integer getClientPort() {
        return client.getPort();
    }

    public void setClientPort(int port) {
        client.setPort(port);
    }

    public void createPortFrame() {
        PortFrame portFrame = new PortFrame(mainFrame);
        centerFrame(portFrame);
        portFrame.setVisible(true);

    }

    public ClientCollection getClientCollection() {
        return client.getClientCollection();
    }

    public String getUserLogin() {
        return client.getUserData().getLogin();
    }

    public String getCommandResult() {
        return client.getCommandResult();
    }

    public String executeHelpCommand() {
        return client.executeHelpCommand();
    }
}
