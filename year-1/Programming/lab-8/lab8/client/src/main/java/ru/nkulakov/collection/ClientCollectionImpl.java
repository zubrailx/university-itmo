package ru.nkulakov.collection;

import ru.nkulakov.gui.locale.Localization;
import ru.nkulakov.gui.setting.DragonOnMap;

import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.time.format.FormatStyle;
import java.util.ArrayList;

public class ClientCollectionImpl implements ClientCollection {

    private final ArrayList<DragonOnMap> dragonsOnMap = new ArrayList<>();
    private ArrayList<Dragon> clientArrayList;

    public ArrayList<DragonOnMap> getDragonsOnMap() {
        return dragonsOnMap;
    }

    public ArrayList<Dragon> getClientArrayList() {
        return clientArrayList;
    }

    public void setClientArrayList(ArrayList<Dragon> clientArrayList) {
        this.clientArrayList = clientArrayList;
    }

    public Object getClientField(Integer rowIndex, Integer columnIndex) {
        switch (columnIndex) {
            case 0: {
                return clientArrayList.get(rowIndex).getId();
            }
            case 1: {
                return clientArrayList.get(rowIndex).getName();
            }
            case 2: {
                return clientArrayList.get(rowIndex).getCoordinates().getX();
            }
            case 3: {
                return clientArrayList.get(rowIndex).getCoordinates().getY();
            }
            case 4: {
                ZonedDateTime zdt = clientArrayList.get(rowIndex).getCreationDate().plusHours(3);
                DateTimeFormatter dtf = DateTimeFormatter.ofLocalizedDateTime(FormatStyle.MEDIUM).withLocale(Localization.getCurrentResourceBundle().getLocale());
                return zdt.format(dtf);

            }
            case 5: {
                return clientArrayList.get(rowIndex).getAge();
            }
            case 6: {
                return clientArrayList.get(rowIndex).getColor();
            }
            case 7: {
                return clientArrayList.get(rowIndex).getType();
            }
            case 8: {
                return clientArrayList.get(rowIndex).getCharacter();
            }
            case 9: {
                return clientArrayList.get(rowIndex).getCave().getDepth();
            }
            case 10: {
                return clientArrayList.get(rowIndex).getOwner();
            }
            default: {
                return "";
            }
        }
    }
}
