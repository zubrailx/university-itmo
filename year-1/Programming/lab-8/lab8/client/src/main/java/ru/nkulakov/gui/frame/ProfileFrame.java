package ru.nkulakov.gui.frame;

import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.util.ResourceBundle;

public class ProfileFrame extends JFrame {
    private final MainFrame mainFrame;

    private JPanel container;

    private JLabel userLabel;
    private JLabel profileLabel;
    private JTextField userTextField;

    public ProfileFrame(MainFrame mainFrame) {
        this.mainFrame = mainFrame;
        initAndSet();
        setLocalization();
        setLocation();

        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        pack();
        setVisible(true);
    }

    private void initAndSet() {
        container = new JPanel();
        Font font = new Font("SansSerif", Font.PLAIN, 14);

        profileLabel = new JLabel();
        profileLabel.setFont(new Font("SansSerif", Font.BOLD, 18));
        userLabel = new JLabel();
        userLabel.setFont(font);
        userTextField = new JTextField(20);
        userTextField.setFont(font);
        userTextField.setText(mainFrame.getGuiManager().getUserLogin());
        userTextField.setEditable(false);
    }

    private void setLocation() {
        container.setSize(250, 100);
        Border border = BorderFactory.createEmptyBorder(10, 30, 0, 30);
//        Border border2 = BorderFactory.createLineBorder(Color.green, 3);
        GridBagLayout layout = new GridBagLayout();
        container.setLayout(layout);
        container.setBorder(border);

        profileLabel.setHorizontalAlignment(SwingConstants.CENTER);
        profileLabel.setVerticalAlignment(SwingConstants.CENTER);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.insets = new Insets(10, 10, 10, 10);
        gbc.gridy = 0;
        gbc.gridx = 0;
        gbc.gridwidth = 2;
//        profileLabel.setBorder(border2);
        container.add(profileLabel, gbc);
        gbc.gridwidth = 1;
        gbc.gridy = 1;
        gbc.gridx = 0;
        gbc.insets = new Insets(10, 10, 10, 5);
        container.add(userLabel, gbc);
        gbc.gridx = 1;
        gbc.insets = new Insets(10, 5, 10, 10);
        container.add(userTextField, gbc);
        this.add(container);
    }

    private void setLocalization() {
        ResourceBundle rb = Localization.getCurrentResourceBundle();

        profileLabel.setText(rb.getString("Profile"));
        userLabel.setText(rb.getString("UserLogin"));
    }
}
