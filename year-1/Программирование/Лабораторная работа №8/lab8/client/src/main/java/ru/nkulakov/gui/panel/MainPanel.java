package ru.nkulakov.gui.panel;

import ru.nkulakov.gui.GuiManagerImpl;
import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.frame.ProfileFrame;
import ru.nkulakov.gui.locale.LocaleEnum;
import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ResourceBundle;

public class MainPanel extends JPanel implements ActionListener {

    private final MainFrame mainFrame;
    private final MainPanel mainPanel = this;

    private boolean isTablePanelActive;
    private boolean isVisualPanelActive;
    private TablePanel tablePanel;
    private VisualizationPanel visualizationPanel;
    private CommandPanel commandPanel;

    private JButton tableButton;
    private JButton visualButton;
    private JMenuBar menuBar;
    private JMenu locale;
    private JMenuItem lanRU;
    private JMenuItem lanSL;
    private JMenuItem lanGC;
    private JMenuItem lanSP;
    private JMenu settings;
    private JMenuItem portItem;
    private JMenu userInfo;
    private JMenuItem userProfile;

    public MainPanel(MainFrame mainFrame) {
        super(null);
        this.mainFrame = mainFrame;
        initAndSet();
        setLocalization();
        addActionEvent();
        locate();
    }

    public void initAndSet() {
        setLayout(new GridBagLayout());

        Font font = new Font("SansSerif", Font.PLAIN, 14);

        tableButton = new JGradientButton();
        tableButton.setFont(font);
        visualButton = new JGradientButton();
        visualButton.setFont(font);

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
        userInfo = new JMenu();
        userInfo.setFont(font);
        userProfile = new JMenuItem();

        tablePanel = new TablePanel(mainFrame);
        visualizationPanel = new VisualizationPanel(mainFrame, mainPanel);
        commandPanel = new CommandPanel(mainFrame, this);
        tablePanel.setVisible(false);
        visualizationPanel.setVisible(false);
    }

    private void locate() {
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
        userInfo.add(userProfile);
        menuBar.add(userInfo);

        visualButton.setPreferredSize(new Dimension(0, 15));
        tableButton.setPreferredSize(new Dimension(0, 15));

        this.add(menuBar, new GridBagConstraints(0, 0, 5, 1, 1, 0,
                GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL,
                new Insets(0, 0, 10, 0), 0, 0));

        this.add(tableButton, new GridBagConstraints(0, 1, 2, 1, 1, 0,
                GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL,
                new Insets(10, 10, 10, 0), 0, 15));

        this.add(visualButton, new GridBagConstraints(2, 1, 2, 1, 1, 0,
                GridBagConstraints.NORTHWEST, GridBagConstraints.HORIZONTAL,
                new Insets(10, 0, 10, 10), 0, 15));

        this.add(commandPanel, new GridBagConstraints(4, 1, 1, 2, 1, 1,
                GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH,
                new Insets(10, 10, 10, 10), 0, 0));
    }

    private void addActionEvent() {
        lanRU.addActionListener(this);
        lanGC.addActionListener(this);
        lanSL.addActionListener(this);
        lanSP.addActionListener(this);
        portItem.addActionListener(this);
        tableButton.addActionListener(this);
        visualButton.addActionListener(this);
        userProfile.addActionListener(this);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
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
        if (e.getSource().equals(tableButton)) {
            visualizationPanel.setVisible(false);   //TODO заменить бульки на статус кнопки.
            isTablePanelActive = true;
            isVisualPanelActive = false;
            updateTable();
            tablePanel.setVisible(true);
        }
        if (e.getSource().equals(visualButton)) {
            tablePanel.setVisible(false);
            isTablePanelActive = false;
            isVisualPanelActive = true;
            updateVisualization();
            visualizationPanel.setVisible(true);
        }
        if (e.getSource().equals(userProfile)) {
            GuiManagerImpl.centerFrame(new ProfileFrame(mainFrame));
        }
    }

    private void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();

        commandPanel.setLocalization();
        tablePanel.setLocalization();

        locale.setText(currentBundle.getString("localeMenu"));
        settings.setText(currentBundle.getString("settingsMenu"));
        portItem.setText(currentBundle.getString("portSettingsItem"));
        userInfo.setText(currentBundle.getString("userInfo"));
        userProfile.setText(currentBundle.getString("Profile"));
        lanRU.setText("Русский");
        lanSL.setText("Slovenščina");
        lanGC.setText("Ελληνικά");
        lanSP.setText("Español");

    }

    //TODO При изменении языка после исполнения команды панелька с таблицей закрывается
    public void updateData() {
        this.remove(tablePanel);
        this.remove(visualizationPanel);
        SwingUtilities.invokeLater(() -> {
            tablePanel = new TablePanel(mainFrame);
            visualizationPanel = new VisualizationPanel(mainFrame, mainPanel);
            if (isTablePanelActive) updateTable();
            if (isVisualPanelActive) updateVisualization();
        });

    }

    private void updateTable() {
        this.add(tablePanel, new GridBagConstraints(0, 2, 4, 1, 1, 1,
                GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH,
                new Insets(10, 10, 10, 10), 0, 0));
        this.revalidate();
        this.repaint();
    }

    private void updateVisualization() {
        this.add(visualizationPanel, new GridBagConstraints(0, 2, 4, 1, 1, 1,
                GridBagConstraints.NORTHWEST, GridBagConstraints.BOTH,
                new Insets(10, 10, 10, 10), 0, 0));
        visualizationPanel.revalidate();
        visualizationPanel.repaint();
//        visualizationPanel.updateUI();
    }

    public MainFrame getMainFrame() {
        return mainFrame;
    }

    private static class JGradientButton extends JButton {
        private JGradientButton() {
            super();
            setContentAreaFilled(false);
        }

        @Override
        protected void paintComponent(Graphics g) {
            final Graphics2D g2 = (Graphics2D) g.create();
            g2.setPaint(new GradientPaint(
                    new Point(0, 0),
                    Color.pink,
                    new Point(0, getHeight()),
                    Color.white));
            g2.fillRect(0, 0, getWidth(), getHeight());
            g2.dispose();

            super.paintComponent(g);
        }
    }
}
