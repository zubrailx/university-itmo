package ru.nkulakov.collection;

import ru.nkulakov.gui.setting.DragonOnMap;

import java.util.ArrayList;

public interface ClientCollection {
    ArrayList<DragonOnMap> getDragonsOnMap();

    ArrayList<Dragon> getClientArrayList();

    void setClientArrayList(ArrayList<Dragon> clientArrayList);

    Object getClientField(Integer rowIndex, Integer columnIndex);
}
