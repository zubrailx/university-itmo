package ru.nkulakov.gui.setting;

import java.awt.*;

public class DragonOnMap {
    private int id;
    private int coordinateX;
    private int coordinateY;
    private int age;
    private Color color;

    public Color getColor() {
        return color;
    }

    public void setColor(Color color) {
        this.color = color;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getCoordinateX() {
        return coordinateX;
    }

    public void setCoordinateX(int coordinateX) {
        this.coordinateX = coordinateX;
    }

    public int getCoordinateY() {
        return coordinateY;
    }

    public void setCoordinateY(int coordinateY) {
        this.coordinateY = coordinateY;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == this) {
            return true;
        }
        if (obj == null || obj.getClass() != this.getClass()) {
            return false;
        }
        DragonOnMap obj2 = (DragonOnMap) obj;
        return obj2.getCoordinateX() == this.coordinateX && obj2.getCoordinateY() == this.getCoordinateY() &&
                obj2.getId() == this.getId();
    }
}
