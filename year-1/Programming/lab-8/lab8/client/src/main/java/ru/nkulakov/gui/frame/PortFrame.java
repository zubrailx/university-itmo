package ru.nkulakov.gui.frame;

import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.ResourceBundle;

public class PortFrame extends JFrame implements ActionListener {

    private final MainFrame mainFrame;
    private JLabel label;
    private JPanel container;
    private JTextField portField;
    private JButton okButton;

    public PortFrame(MainFrame mainFrame) {
        this.mainFrame = mainFrame;
        this.initAndSet();
        this.addActionEvent();
        this.setLocalization();
        this.locate();

        this.pack();
        this.setResizable(false);
    }

    private void initAndSet() {
        container = new JPanel();
        Font font = new Font("SansSerif", Font.PLAIN, 14);

        label = new JLabel();
        label.setFont(font);
        portField = new JTextField(15);
        portField.setFont(font);
        portField.setText(mainFrame.getGuiManager().getClientPort().toString());
        okButton = new JButton();
        okButton.setFont(font);

        portField.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent ke) {
                portField.setEditable(ke.getKeyChar() >= '0' && ke.getKeyChar() <= '9' ||
                        ke.getKeyCode() == KeyEvent.VK_BACK_SPACE);
            }
        });
    }

    private void locate() {
        container.setSize(300, 170);
        Border border = BorderFactory.createEmptyBorder(10, 30, 0, 30);
        GridBagLayout layout = new GridBagLayout();
        container.setLayout(layout);
        container.setBorder(border);

        okButton.setPreferredSize(new Dimension(100, 25));
        portField.setPreferredSize(new Dimension(150, 25));

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.gridy = 0;
        container.add(label, gbc);
        gbc.gridy = 1;

        container.add(portField, gbc);
        gbc.gridy = 2;
        container.add(okButton, gbc);
        this.add(container);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(okButton)) {
            int port;
            try {
                port = Integer.parseInt(portField.getText());
            } catch (NumberFormatException ex) {
                port = 0;
            }
            mainFrame.getGuiManager().setClientPort(port);
            this.dispose();
        }
    }

    private void addActionEvent() {
        okButton.addActionListener(this);
    }

    private void setLocalization() {
        ResourceBundle rb = Localization.getCurrentResourceBundle();
        label.setText(rb.getString("portLabel"));
        okButton.setText(rb.getString("okButton"));
    }
}
