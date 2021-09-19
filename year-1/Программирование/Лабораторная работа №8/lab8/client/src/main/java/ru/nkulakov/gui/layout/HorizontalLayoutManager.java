package ru.nkulakov.gui.layout;

import java.awt.*;

public class HorizontalLayoutManager implements LayoutManager {

    private final int horizontalMargin;
    private final int firstMargin;
    private final int lastMargin;

    public HorizontalLayoutManager(int horizontalMargin, int firstMargin, int lastMargin) {
        this.horizontalMargin = horizontalMargin;
        this.firstMargin = firstMargin;
        this.lastMargin = lastMargin;
    }

    @Override
    public void addLayoutComponent(String name, Component comp) {
    }

    @Override
    public void removeLayoutComponent(Component comp) {
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        return calculateBestSize(parent);
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return calculateBestSize(parent);
    }

    @Override
    public void layoutContainer(Container parent) {
        Component[] list = parent.getComponents();
        int currentX = firstMargin;
        for (Component component : list) {
            // Определение предпочтительного размера компонента
            Dimension pref = component.getPreferredSize();
            // Размещение компонента на экране
            component.setBounds(currentX, 0, pref.width, pref.height);
            // Смещаем вертикальную позицию компонента (отступ между n пикселей)
            currentX += pref.width + horizontalMargin;
        }
    }

    private Dimension calculateBestSize(Container c) {
        Component[] list = c.getComponents();
        int maxHeight = 0;
        int totalWidth = 0;

        for (Component component : list) {
            int height = component.getPreferredSize().height;
            if (height > maxHeight) maxHeight = height;
            totalWidth += component.getPreferredSize().width + horizontalMargin;
        }
        return new Dimension(totalWidth + lastMargin, maxHeight);
    }
}
