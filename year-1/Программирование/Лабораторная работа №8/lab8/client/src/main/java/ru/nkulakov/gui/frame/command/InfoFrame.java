package ru.nkulakov.gui.frame.command;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class InfoFrame extends CommandFrame {
    private JPanel container;
    private JTextArea jTextArea;
    private JScrollPane jScrollPane;

    public InfoFrame(String option) {
        super(option);
        this.initAndSet();
        this.setLocalization();
        this.locate();

        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        this.pack();
        this.setResizable(false);
    }

    private void initAndSet() {
        Font font = new Font("SansSerif", Font.PLAIN, 16);

        container = new JPanel();
        jTextArea = new JTextArea(20, 50);
        jTextArea.setLineWrap(true);
        jTextArea.setWrapStyleWord(true);
        jTextArea.setFont(font);
        jTextArea.setEditable(false);
        jScrollPane = new JScrollPane(jTextArea);
    }

    private void locate() {
        GridLayout layout = new GridLayout(1, 1, 0, 0);
        container.setLayout(layout);
        container.add(jScrollPane);
        this.add(container);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
    }

    private void setLocalization() {
        jTextArea.setText("Здесь будет находиться информация о коллекции, но я сделаю это потом.");
    }

    public void setText(String text) {
        jTextArea.setText(text);
        this.revalidate();
    }
}
