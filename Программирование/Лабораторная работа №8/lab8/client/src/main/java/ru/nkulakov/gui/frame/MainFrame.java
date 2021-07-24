package ru.nkulakov.gui.frame;

import ru.nkulakov.gui.GuiManager;

import javax.swing.*;

public class MainFrame extends JFrame {

    private final GuiManager guiManager;

    public MainFrame(GuiManager guiManager) {
        super();
        this.setLayout(null);
        this.setTitle("Client Application.");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.guiManager = guiManager;
    }

    public GuiManager getGuiManager() {
        return guiManager;
    }
}