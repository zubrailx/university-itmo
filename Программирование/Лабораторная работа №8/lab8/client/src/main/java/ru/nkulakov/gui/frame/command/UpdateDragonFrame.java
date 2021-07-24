package ru.nkulakov.gui.frame.command;

import ru.nkulakov.gui.locale.Localization;
import ru.nkulakov.gui.panel.DragonPanel;
import ru.nkulakov.gui.panel.MainPanel;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.util.ResourceBundle;

public class UpdateDragonFrame extends CommandFrame {
    private final String option;
    private final MainPanel mainPanel;
    private JPanel container;

    private String fault;
    private DragonPanel dragonPanel;
    private JButton jButton;

    public UpdateDragonFrame(String option, MainPanel mainPanel) {
        super(option);
        this.mainPanel = mainPanel;
        this.option = option;
        this.initAndSet();
        this.setLocalization();
        this.addActionEvent();
        this.locate();

        pack();
        setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        this.setMinimumSize(new Dimension(260, 430));
        setVisible(true);
    }

    private void initAndSet() {
        Font font = new Font("SansSerif", Font.PLAIN, 14);

        container = new JPanel();
        dragonPanel = new DragonPanel(this, mainPanel.getMainFrame());
        jButton = new JButton();
        jButton.setFont(font);
    }

    private void locate() {
        GridBagLayout layout = new GridBagLayout();
        container.setLayout(layout);

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.anchor = GridBagConstraints.NORTH;
        gbc.insets = new Insets(20, 20, 10, 20);

        gbc.gridy = 0;
        container.add(dragonPanel, gbc);

        gbc.gridy = 1;
        gbc.weightx = 1;
        gbc.ipadx = 20;
        gbc.insets = new Insets(10, 20, 20, 20);
        jButton.setSize(new Dimension(150, 30));
        container.add(jButton, gbc);
        this.add(container);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource().equals(jButton)) {
            try {
                if (!option.equals("insert")) {
                    String result = dragonPanel.getDragonAdaptation();
                    mainPanel.getMainFrame().getGuiManager().executeCommand(result);
                    mainPanel.updateData();
                    this.dispose();
                }
                if (option.equals("insert")) {
                    String result = dragonPanel.getInsertDragonAdaptation();
                    if (result == null) {
                        JOptionPane.showMessageDialog(this, fault);
                    } else {
                        if (mainPanel.getMainFrame().getGuiManager().executeCommand(result)) {
                            mainPanel.updateData();
                            this.dispose();
                        }
                    }
                }
            } catch (NumberFormatException ex) {
                System.out.println("Number format ex");
                ex.printStackTrace();
            }
        }
    }

    private void addActionEvent() {
        jButton.addActionListener(this);
    }

    private void setLocalization() {
        ResourceBundle rb = Localization.getCurrentResourceBundle();
        jButton.setText(rb.getString("OK"));
        fault = rb.getString("InsertFault");
    }

    public void setId(Integer id) {
        dragonPanel.setIdText(id);
    }

    public void lockId() {
        dragonPanel.getIdT().setEditable(false);
    }
}
