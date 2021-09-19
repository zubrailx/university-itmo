package ru.nkulakov.gui.panel;

import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.setting.TableModel;

import javax.swing.*;
import java.awt.*;

public class TablePanel extends JPanel {
    private final MainFrame mainFrame;

    private JTable dragonTable;
    private JScrollPane scrollPane;

    public TablePanel(MainFrame mainFrame) {
        super(new GridLayout(1, 1, 0, 0));
        this.mainFrame = mainFrame;
        initAndSet();
        setLocalization();
        locate();
    }

    public void initAndSet() {
        dragonTable = new JTable();
        dragonTable.setAutoCreateRowSorter(true);  //TODO сделать сортировку с помощью стримапи
        scrollPane = new JScrollPane(dragonTable);

        Color white = new Color(255, 255, 255);
        dragonTable.setOpaque(true);
        dragonTable.setFillsViewportHeight(true);
        dragonTable.setBackground(white);
    }

    public void locate() {

        add(scrollPane);
    }

    public void setLocalization() {
        TableModel tableModel = new TableModel(mainFrame.getGuiManager().getClientCollection());
        dragonTable.setModel(tableModel);
        dragonTable.revalidate();
    }
}

