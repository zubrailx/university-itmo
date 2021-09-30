package ru.nkulakov.gui.panel;

import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.layout.HorizontalLayoutManager;
import ru.nkulakov.gui.layout.VerticalLayoutManager;
import ru.nkulakov.gui.locale.LocaleEnum;
import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class LoginPanel extends JPanel implements ActionListener {

    private final MainFrame mainFrame;
    private final String[] signInMessages = {"Error", "Connection", "Exception", "LoginLength", "PasswordLength", "UserAlreadyExists"};
    private final ArrayList<String> signInLocalizedMessages = new ArrayList<>();
    private JPanel container;
    private JPanel loginAndPassPanel;
    private JPanel loginPanel;
    private JLabel loginLabel;
    private JTextField loginText;
    private JPanel passPanel;
    private JLabel passLabel;
    private JPasswordField passText;
    private JPanel bottomPanel;
    private JPanel authenticationPanel;
    private JButton registerButton;
    private JButton okButton;
    private JMenuBar menuBar;
    private JMenu locale;
    private JMenuItem lanRU;
    private JMenuItem lanSL;
    private JMenuItem lanGC;
    private JMenuItem lanSP;
    private JPanel emptyBottom;
    private JMenu settings;
    private JMenuItem portItem;

    public LoginPanel(MainFrame mainFrame) {
        super(new BorderLayout());
        this.initAndSet();
        this.setLocalization();
        this.locate();
        this.addActionEvent();
        this.mainFrame = mainFrame;
    }

    private void initAndSet() {
        container = new JPanel(new BorderLayout());
        Font font = new Font("SansSerif", Font.PLAIN, 14);

        loginAndPassPanel = new JPanel(new VerticalLayoutManager(10, 10, 10));
        loginPanel = new JPanel(new HorizontalLayoutManager(5, 10, 10));
        loginLabel = new JLabel();
        loginLabel.setFont(font);
        loginText = new JTextField(20);
        loginText.setFont(font);
        passPanel = new JPanel(new HorizontalLayoutManager(5, 10, 10));
        passLabel = new JLabel();
        passLabel.setFont(font);
        passText = new JPasswordField(20);
        passText.setEchoChar("*".charAt(0));
        passText.setFont(font);

        bottomPanel = new JPanel(new BorderLayout());
        authenticationPanel = new JPanel(new HorizontalLayoutManager(5, 10, 10));
        registerButton = new JButton();
        registerButton.setFont(font);
        okButton = new JButton();
        okButton.setFont(font);
        emptyBottom = new JPanel();

        menuBar = new JMenuBar();
        locale = new JMenu();
        locale.setFont(font);
        lanRU = new JMenuItem();
        lanSL = new JMenuItem();
        lanGC = new JMenuItem();
        lanSP = new JMenuItem();
        settings = new JMenu();
        settings.setFont(font);
        portItem = new JMenuItem();
    }

    private void locate() {
        Border border = BorderFactory.createEmptyBorder(10, 10, 10, 10);

        loginLabel.setPreferredSize(new Dimension(120, 30));
        loginPanel.add(loginLabel);
        loginText.setPreferredSize(new Dimension(300, 30));
        loginPanel.add(loginText);
        passLabel.setPreferredSize(new Dimension(120, 30));
        passPanel.add(passLabel);
        passText.setPreferredSize(new Dimension(300, 30));
        passPanel.add(passText);
        loginAndPassPanel.add(loginPanel);
        loginAndPassPanel.add(passPanel);
        loginAndPassPanel.setBorder(border);

        emptyBottom.setPreferredSize(new Dimension(120, 30));
        authenticationPanel.add(emptyBottom);
        registerButton.setPreferredSize(new Dimension(100, 30));
        authenticationPanel.add(registerButton);
        okButton.setPreferredSize(new Dimension(100, 30));
        authenticationPanel.add(okButton);
        bottomPanel.add(authenticationPanel, BorderLayout.NORTH);
        bottomPanel.setBorder(border);

        menuBar.setPreferredSize(new Dimension(100, 30));
        locale.add(lanRU);
        locale.addSeparator();
        locale.add(lanSL);
        locale.addSeparator();
        locale.add(lanGC);
        locale.addSeparator();
        locale.add(lanSP);
        menuBar.add(locale);

        settings.add(portItem);
        menuBar.add(settings);

        this.add(menuBar, BorderLayout.NORTH);
        this.add(loginAndPassPanel, BorderLayout.CENTER);
        this.add(bottomPanel, BorderLayout.PAGE_END);

        this.setLocation(500, 300);
        this.setMinimumSize(new Dimension(container.getWidth(), container.getHeight() + 40)); //не считаются отступы от border
        this.setMaximumSize(new Dimension(600, 600));

    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String login;
        String password;
        if (e.getSource().equals(registerButton)) {
            login = loginText.getText().trim();
            password = new String(passText.getPassword()).trim();
            if (login.length() < 6) {
                JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(3));
            } else if (password.length() < 6) JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(4));
            else {
                switch (mainFrame.getGuiManager().authenticate(login, password, true)) {
                    case 1: {
                        JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(1));
                        break;
                    }
                    case 0: {
                        JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(5));
                        break;
                    }
                }
            }
        }
        if (e.getSource().equals(okButton)) {
            login = loginText.getText().trim();
            password = new String(passText.getPassword()).trim();

            switch (mainFrame.getGuiManager().authenticate(login, password, false)) {
                case 1: {
                    JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(1));
                    break;
                }
                case 0: {
                    JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(0));
                    break;
                }
                case -1: {
                    JOptionPane.showMessageDialog(this, signInLocalizedMessages.get(2));
                    break;
                }
            }
        }
        if (e.getSource().equals(lanRU)) {
            ResourceBundle currentBundle = Localization.getBundle(LocaleEnum.ru_RU);
            Localization.setCurrentResourceBundle(currentBundle);
            setLocalization();
        }
        if (e.getSource().equals(lanGC)) {
            ResourceBundle currentBundle = Localization.getBundle(LocaleEnum.el_GR);
            Localization.setCurrentResourceBundle(currentBundle);
            setLocalization();
        }
        if (e.getSource().equals(lanSL)) {
            ResourceBundle currentBundle = Localization.getBundle(LocaleEnum.sl_SI);
            Localization.setCurrentResourceBundle(currentBundle);
            setLocalization();
        }
        if (e.getSource().equals(lanSP)) {
            ResourceBundle currentBundle = Localization.getBundle(LocaleEnum.es_PR);
            Localization.setCurrentResourceBundle(currentBundle);
            setLocalization();
        }
        if (e.getSource().equals(portItem)) {
            mainFrame.getGuiManager().createPortFrame();
        }
    }

    private void addActionEvent() {
        registerButton.addActionListener(this);
        okButton.addActionListener(this);
        lanRU.addActionListener(this);
        lanGC.addActionListener(this);
        lanSL.addActionListener(this);
        lanSP.addActionListener(this);
        portItem.addActionListener(this);
    }

    private void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();

        loginLabel.setText(currentBundle.getString("loginLabel"));
        passLabel.setText(currentBundle.getString("passLabel"));
        registerButton.setText(currentBundle.getString("registerButton"));
        okButton.setText(currentBundle.getString("okButton"));
        locale.setText(currentBundle.getString("localeMenu"));
        settings.setText(currentBundle.getString("settingsMenu"));
        portItem.setText(currentBundle.getString("portSettingsItem"));

        lanRU.setText("Русский");
        lanSL.setText("Slovenščina");
        lanGC.setText("Ελληνικά");
        lanSP.setText("Español");

        signInLocalizedMessages.clear();
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[0]));
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[1]));
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[2]));
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[3]));
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[4]));
        signInLocalizedMessages.add(currentBundle.getString(signInMessages[5]));
    }
}
