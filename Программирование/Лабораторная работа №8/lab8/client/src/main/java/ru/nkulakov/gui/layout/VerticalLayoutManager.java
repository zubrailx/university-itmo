package ru.nkulakov.gui.layout;

import java.awt.*;

public class VerticalLayoutManager implements LayoutManager {

    private final int verticalMargin;
    private final int firstMargin;
    private final int lastMargin;

    public VerticalLayoutManager(int verticalMargin, int firstMargin, int lastMargin) {
        this.verticalMargin = verticalMargin;
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
        int currentY = firstMargin;
        for (Component component : list) {
            // Определение предпочтительного размера компонента
            Dimension pref = component.getPreferredSize();
            // Размещение компонента на экране
            component.setBounds(0, currentY, pref.width, pref.height);
            // Смещаем вертикальную позицию компонента (отступ между 5 пикселей)
            currentY += pref.height + verticalMargin;
        }
    }

    private Dimension calculateBestSize(Container c) {
        Component[] list = c.getComponents();
        int maxWidth = 0;
        int totalHeight = verticalMargin;

        for (Component component : list) {
            int width = component.getPreferredSize().width;
            if (width > maxWidth) maxWidth = width;
            totalHeight += component.getPreferredSize().getHeight();
        }
        return new Dimension(maxWidth, totalHeight + lastMargin);
    }
}
