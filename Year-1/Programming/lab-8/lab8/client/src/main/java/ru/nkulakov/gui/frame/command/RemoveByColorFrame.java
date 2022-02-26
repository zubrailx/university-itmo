package ru.nkulakov.gui.frame.command;

import ru.nkulakov.gui.locale.Localization;
import ru.nkulakov.gui.panel.MainPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.ResourceBundle;

public class RemoveByColorFrame extends CommandFrame {
    private final MainPanel mainPanel;
    private final String option;
    private final String[] arr = {"GREEN", "YELLOW", "ORANGE", "BLUE"};
    private JPanel container;
    private JButton greenButton;
    private JButton yellowButton;
    private JButton orangeButton;
    private JButton blueButton;

    public RemoveByColorFrame(String option, MainPanel mainPanel) {
        super(option);
        this.option = option;
        this.mainPanel = mainPanel;
        this.initAndSet();
        this.setLocalization();
        this.locate();
        this.addActionEvent();

        this.pack();
        this.setResizable(false);
        this.setVisible(true);
    }

    private void initAndSet() {
        Font font = new Font("SansSerif", Font.PLAIN, 14);
        container = new JPanel(new FlowLayout());

        greenButton = new JButton();
        greenButton.setFont(font);
        yellowButton = new JButton();
        yellowButton.setFont(font);
        orangeButton = new JButton();
        orangeButton.setFont(font);
        blueButton = new JButton();
        blueButton.setFont(font);
    }

    private void locate() {
        GridLayout gridLayout = new GridLayout(4, 1, 5, 5);
        container.setLayout(gridLayout);

        container.add(greenButton);
        container.add(yellowButton);
        container.add(orangeButton);
        container.add(blueButton);

        this.add(container);
        container.setSize(new Dimension(200, 120));
    }

    private void addActionEvent() {
        greenButton.addActionListener(this);
        yellowButton.addActionListener(this);
        orangeButton.addActionListener(this);
        blueButton.addActionListener(this);

    }

    private void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();
        greenButton.setText(currentBundle.getString(arr[0]));
        yellowButton.setText(currentBundle.getString(arr[1]));
        orangeButton.setText(currentBundle.getString(arr[2]));
        blueButton.setText(currentBundle.getString(arr[3]));
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String color = null;
        if (e.getSource().equals(greenButton)) color = arr[0];
        if (e.getSource().equals(yellowButton)) color = arr[1];
        if (e.getSource().equals(orangeButton)) color = arr[2];
        if (e.getSource().equals(blueButton)) color = arr[3];

        if (mainPanel.getMainFrame().getGuiManager().executeCommand(option + " " + color)) {
            mainPanel.updateData();
            this.dispose();
        }
    }
}