package ru.nkulakov.gui.setting;

import ru.nkulakov.collection.ClientCollection;
import ru.nkulakov.collection.Color;
import ru.nkulakov.collection.DragonCharacter;
import ru.nkulakov.collection.DragonType;
import ru.nkulakov.gui.locale.Localization;

import javax.swing.table.AbstractTableModel;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;

public class TableModel extends AbstractTableModel {

    private final List<String> headerList;
    private final ClientCollection clientCollection;

    public TableModel(ClientCollection clientCollection) {
        headerList = new ArrayList<>();
        this.clientCollection = clientCollection;
        setHeaderLocalization();
    }

    @Override
    public int getRowCount() {
        return clientCollection.getClientArrayList().size();
    }

    @Override
    public int getColumnCount() {
        return headerList.size();
    }

    @Override
    public String getColumnName(int columnIndex) {
        return headerList.get(columnIndex);
    }

    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        return clientCollection.getClientField(rowIndex, columnIndex);
    }

    @Override
    public Class<?> getColumnClass(int columnIndex) {
        switch (columnIndex) {
            case 0:
            case 3:
                return Integer.class;
            case 2:
            case 9:
                return Double.class;
            case 5:
                return Long.class;
            case 6:
                return Color.class;
            case 7:
                return DragonType.class;
            case 8:
                return DragonCharacter.class;
            default:
                return String.class;
        }
    }

    public void setHeaderLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();

        headerList.clear();
        headerList.add(currentBundle.getString("id"));
        headerList.add(currentBundle.getString("name"));
        headerList.add(currentBundle.getString("coordinate_x"));
        headerList.add(currentBundle.getString("coordinate_y"));
        headerList.add(currentBundle.getString("creation_date"));
        headerList.add(currentBundle.getString("age"));
        headerList.add(currentBundle.getString("color"));
        headerList.add(currentBundle.getString("type"));
        headerList.add(currentBundle.getString("character"));
        headerList.add(currentBundle.getString("cave_depth"));
        headerList.add(currentBundle.getString("owner"));
    }
}