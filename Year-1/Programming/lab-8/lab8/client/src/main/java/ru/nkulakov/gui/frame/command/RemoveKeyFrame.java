package ru.nkulakov.gui.frame.command;

import ru.nkulakov.gui.locale.Localization;
import ru.nkulakov.gui.panel.MainPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ResourceBundle;

public class RemoveKeyFrame extends CommandFrame {

    private final MainPanel mainPanel;

    private final String option;
    private JPanel container;
    private JLabel jLabel;
    private JButton jButton;
    private JTextField jTextField;

    public RemoveKeyFrame(String option, MainPanel mainPanel) {
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
        container = new JPanel();

        jLabel = new JLabel();
        jLabel.setFont(font);
        jButton = new JButton();
        jButton.setFont(font);
        jTextField = new JTextField(30);
        jTextField.setFont(font);

        jTextField.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent ke) {
                jTextField.setEditable(ke.getKeyChar() >= '0' && ke.getKeyChar() <= '9' ||
                        ke.getKeyCode() == KeyEvent.VK_BACK_SPACE);
            }
        });
    }

    private void locate() {
        GridBagLayout gridBagLayout = new GridBagLayout();
        container.setLayout(gridBagLayout);

        jLabel.setHorizontalAlignment(SwingConstants.CENTER);
        jLabel.setVerticalAlignment(SwingConstants.CENTER);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(10, 20, 10, 20);
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.weightx = 0;
        gbc.weighty = 0;

        gbc.gridy = 0;
        gbc.ipady = 10;
        gbc.ipadx = 50;
        container.add(jLabel, gbc);
        gbc.gridy = 1;
        gbc.ipady = 10;
        container.add(jTextField, gbc);
        gbc.gridy = 2;
        gbc.ipadx = 45;
        gbc.ipady = 5;
        gbc.insets = new Insets(5, 20, 20, 20);
        container.add(jButton, gbc);
        this.setContentPane(container);
    }

    private void addActionEvent() {
        jButton.addActionListener(this);
    }

    private void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();
        jLabel.setText(currentBundle.getString("EnterKey"));
        jButton.setText(currentBundle.getString("OK"));
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(jButton)) {
            String value = jTextField.getText();
            if (mainPanel.getMainFrame().getGuiManager().executeCommand(option + " " + value)) {
                mainPanel.updateData();
            }
            this.dispose();
        }
    }
}
