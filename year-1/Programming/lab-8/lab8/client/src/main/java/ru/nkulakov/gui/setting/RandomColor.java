package ru.nkulakov.gui.setting;

import java.awt.*;
import java.util.Random;

public class RandomColor {

    public static Color getRandomColor() {
        Random rand = new Random();
        float r = rand.nextFloat();
        float g = rand.nextFloat();
        float b = rand.nextFloat();
        return new Color(r, g, b);
    }
}
