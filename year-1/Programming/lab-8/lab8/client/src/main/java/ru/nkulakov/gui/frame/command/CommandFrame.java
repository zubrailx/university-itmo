package ru.nkulakov.gui.frame.command;

import javax.swing.*;
import java.awt.event.ActionListener;

public abstract class CommandFrame extends JFrame implements ActionListener { //TODO заменить option на Enum
    protected String option;

    public CommandFrame(String option) {
        super(option);
        this.option = option;
    }

    public String getOption() {
        return option;
    }
}
