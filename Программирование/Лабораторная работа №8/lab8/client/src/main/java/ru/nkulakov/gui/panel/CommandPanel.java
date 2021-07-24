package ru.nkulakov.gui.panel;

import ru.nkulakov.gui.GuiManagerImpl;
import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.frame.command.*;
import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ResourceBundle;

public class CommandPanel extends JPanel implements ActionListener {

    private final MainFrame mainFrame;
    private final MainPanel mainPanel;

    private JLabel commandLabel;
    private JButton averageAgeButton;
    private JButton clearButton;
    private JButton executeScriptButton;
    private JButton helpButton;
    private JButton infoButton;
    private JButton insertElementButton;
    private JButton maxElementByAgeButton;
    private JButton removeAnyByColorButton;
    private JButton removeElementButton;
    private JButton removeLowerKeysButton;
    private JButton replaceIfGreaterButton;
    private JButton replaceIfLowerButton;
    private JButton updateElementButton;

    public CommandPanel(MainFrame mainFrame, MainPanel mainPanel) {
        super();
        this.mainFrame = mainFrame;
        this.mainPanel = mainPanel;
        this.initAndSet();
        this.addActionEvent();
        this.locate();
    }

    private void initAndSet() {
        Font font = new Font("SansSerif", Font.PLAIN, 16);

        commandLabel = new JLabel();
        commandLabel.setFont(new Font("SansSerif", Font.ITALIC, 24));
        averageAgeButton = new JButton();
        averageAgeButton.setFont(font);
        clearButton = new JButton();
        clearButton.setFont(font);
        executeScriptButton = new JButton();
        executeScriptButton.setFont(font);
        helpButton = new JButton();
        helpButton.setFont(font);
        infoButton = new JButton();
        infoButton.setFont(font);
        insertElementButton = new JButton();
        insertElementButton.setFont(font);
        maxElementByAgeButton = new JButton();
        maxElementByAgeButton.setFont(font);
        removeAnyByColorButton = new JButton();
        removeAnyByColorButton.setFont(font);
        removeElementButton = new JButton();
        removeElementButton.setFont(font);
        removeLowerKeysButton = new JButton();
        removeLowerKeysButton.setFont(font);
        replaceIfGreaterButton = new JButton();
        replaceIfGreaterButton.setFont(font);
        replaceIfLowerButton = new JButton();
        replaceIfLowerButton.setFont(font);
        updateElementButton = new JButton();
        updateElementButton.setFont(font);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(averageAgeButton)) {
            if (mainFrame.getGuiManager().executeCommand("average_of_age")) {
                mainPanel.updateData();
                InfoFrame infoFrame = new InfoFrame("average_of_age");
                infoFrame.setText(mainFrame.getGuiManager().getCommandResult());
                GuiManagerImpl.centerFrame(infoFrame);
                infoFrame.setVisible(true);
            }
        }
        if (e.getSource().equals(clearButton)) {
            if (mainFrame.getGuiManager().executeCommand("clear")) {
                mainPanel.updateData();
                InfoFrame infoFrame = new InfoFrame("clear");
                infoFrame.setText(mainFrame.getGuiManager().getCommandResult());
                GuiManagerImpl.centerFrame(infoFrame);
                infoFrame.setVisible(true);
            }
        }
        if (e.getSource().equals(executeScriptButton)) {
            GuiManagerImpl.centerFrame(new ExecuteScriptFrame("execute_script", mainPanel));
        }
        if (e.getSource().equals(helpButton)) {
            InfoFrame infoFrame = new InfoFrame("help");
            infoFrame.setText(mainFrame.getGuiManager().executeHelpCommand());
            GuiManagerImpl.centerFrame(infoFrame);
            infoFrame.setVisible(true);
        }
        if (e.getSource().equals(infoButton)) {
            if (mainFrame.getGuiManager().executeCommand("info")) {
                mainPanel.updateData();
                InfoFrame infoFrame = new InfoFrame("info");
                infoFrame.setText(mainFrame.getGuiManager().getCommandResult());
                GuiManagerImpl.centerFrame(infoFrame);
                infoFrame.setVisible(true);
            }
        }
        if (e.getSource().equals(removeAnyByColorButton)) {
            GuiManagerImpl.centerFrame(new RemoveByColorFrame("remove_any_by_color", mainPanel));
        }
        if (e.getSource().equals(removeElementButton)) {
            GuiManagerImpl.centerFrame(new RemoveKeyFrame("remove_key", mainPanel));

        }
        if (e.getSource().equals(removeLowerKeysButton)) {
            GuiManagerImpl.centerFrame(new RemoveKeyFrame("remove_lower_key", mainPanel));

        }
        if (e.getSource().equals(maxElementByAgeButton)) {
            if (mainFrame.getGuiManager().executeCommand("max_by_age")) {
                mainPanel.updateData();
                InfoFrame infoFrame = new InfoFrame("max_by_age");
                infoFrame.setText(mainFrame.getGuiManager().getCommandResult());
                GuiManagerImpl.centerFrame(infoFrame);
                infoFrame.setVisible(true);
            }
        }
        if (e.getSource().equals(insertElementButton)) {
            GuiManagerImpl.centerFrame(new UpdateDragonFrame("insert", mainPanel));
        }
        if (e.getSource().equals(replaceIfGreaterButton)) {
            GuiManagerImpl.centerFrame(new UpdateDragonFrame("replace_if_greater", mainPanel));
        }
        if (e.getSource().equals(replaceIfLowerButton)) {
            GuiManagerImpl.centerFrame(new UpdateDragonFrame("replace_if_lower", mainPanel));
        }
        if (e.getSource().equals(updateElementButton)) {
            GuiManagerImpl.centerFrame(new UpdateDragonFrame("update", mainPanel));
        }
    }

    public void addActionEvent() {
        averageAgeButton.addActionListener(this);
        clearButton.addActionListener(this);
        executeScriptButton.addActionListener(this);
        helpButton.addActionListener(this);
        infoButton.addActionListener(this);
        insertElementButton.addActionListener(this);
        maxElementByAgeButton.addActionListener(this);
        removeAnyByColorButton.addActionListener(this);
        removeElementButton.addActionListener(this);
        removeLowerKeysButton.addActionListener(this);
        replaceIfGreaterButton.addActionListener(this);
        replaceIfLowerButton.addActionListener(this);
        updateElementButton.addActionListener(this);
    }

    public void locate() {
        this.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        commandLabel.setPreferredSize(new Dimension(0, 40));
        commandLabel.setHorizontalAlignment(SwingConstants.CENTER);
        commandLabel.setVerticalAlignment(SwingConstants.CENTER);

        gbc.fill = GridBagConstraints.BOTH;
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.weighty = 1;
        gbc.weightx = 1;
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.insets = new Insets(5, 5, 5, 5);
        this.add(commandLabel, gbc);
        gbc.gridy = 1;
        this.add(averageAgeButton, gbc);
        gbc.gridy = 2;
        this.add(clearButton, gbc);
        gbc.gridy = 3;
        this.add(executeScriptButton, gbc);
        gbc.gridy = 4;
        this.add(helpButton, gbc);
        gbc.gridy = 5;
        this.add(infoButton, gbc);
        gbc.gridy = 6;
        this.add(removeAnyByColorButton, gbc);
        gbc.gridy = 7;
        this.add(removeElementButton, gbc);
        gbc.gridy = 8;
        this.add(removeLowerKeysButton, gbc);
        gbc.gridy = 9;
        this.add(maxElementByAgeButton, gbc);
        gbc.gridy = 10;
        this.add(insertElementButton, gbc);
        gbc.gridy = 11;
        this.add(replaceIfGreaterButton, gbc);
        gbc.gridy = 12;
        this.add(replaceIfLowerButton, gbc);
        gbc.gridy = 13;
        this.add(updateElementButton, gbc);
    }

    public void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();

        commandLabel.setText(currentBundle.getString("commandLabel"));
        averageAgeButton.setText(currentBundle.getString("averageAgeButton"));
        clearButton.setText(currentBundle.getString("clearButton"));
        executeScriptButton.setText(currentBundle.getString("executeScriptButton"));
        helpButton.setText(currentBundle.getString("helpButton"));
        infoButton.setText(currentBundle.getString("infoButton"));
        insertElementButton.setText(currentBundle.getString("insertElementButton"));
        maxElementByAgeButton.setText(currentBundle.getString("maxElementByAgeButton"));
        removeAnyByColorButton.setText(currentBundle.getString("removeAnyByColorButton"));
        removeElementButton.setText(currentBundle.getString("removeElementButton"));
        removeLowerKeysButton.setText(currentBundle.getString("removeLowerKeysButton"));
        replaceIfGreaterButton.setText(currentBundle.getString("replaceIfGreaterButton"));
        replaceIfLowerButton.setText(currentBundle.getString("replaceIfLowerButton"));
        updateElementButton.setText(currentBundle.getString("updateElementButton"));

        this.revalidate();
        this.repaint();
    }

//    public MainFrame getMainFrame() {
//        return mainFrame;
//    }
}
