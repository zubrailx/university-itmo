package ru.nkulakov.gui.panel;

import ru.nkulakov.collection.Dragon;
import ru.nkulakov.gui.GuiManagerImpl;
import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.frame.command.UpdateDragonFrame;
import ru.nkulakov.gui.setting.DragonOnMap;
import ru.nkulakov.gui.setting.RandomColor;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Optional;
import java.util.TreeMap;

public class VisualizationPanel extends JPanel {
    private static final TreeMap<String, Color> userColors = new TreeMap<>();
    private final MainFrame mainFrame;
    private final MainPanel mainPanel;
    private final VisualizationPanel vp = this;

    ArrayList<DragonOnMap> dragonOnMaps;
    ArrayList<Dragon> dragonArrayList;

    int fieldLength = 200;
    int count = 0;
    int maxObjSize = 25;
    int minObjSize = 4;
    int gridLineLength = 50;
    private Graphics2D graphics2D;

    public VisualizationPanel(MainFrame mainFrame, MainPanel mainPanel) {
        this.mainFrame = mainFrame;
        this.mainPanel = mainPanel;
        addMouseListener();
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        setBackground(Color.white);
        int x = (int) this.getSize().getWidth() - 1;
        int y = (int) this.getSize().getHeight() - 1;
        for (int i = 0; i <= gridLineLength; i++) {
            //vertical
            g.drawLine(i * x / gridLineLength, 0, i * x / gridLineLength, y);
            //horizontal
            g.drawLine(0, i * y / gridLineLength, x, i * y / gridLineLength);
            g.setColor(Color.LIGHT_GRAY);
        }
        graphics2D = (Graphics2D) g;
        graphics2D.setColor(Color.BLACK);
        graphics2D.setStroke(new BasicStroke(1));
        graphics2D.drawLine(x / 2, 0, x / 2, y);
        graphics2D.drawLine(0, y / 2, x, y / 2);

        dragonOnMaps = mainFrame.getGuiManager().getClientCollection().getDragonsOnMap();
        dragonArrayList = mainFrame.getGuiManager().getClientCollection().getClientArrayList();

        Dragon[] dragons = dragonArrayList.toArray(new Dragon[0]);
        count++;
        for (Dragon dragon : dragons) {
            if (!userColors.containsKey(dragon.getOwner()))
                userColors.put(dragon.getOwner(), RandomColor.getRandomColor());

            long age = dragon.getAge() == null ? 0 : (dragon.getAge()) / 4;
            age = age > maxObjSize ? maxObjSize : age;
            age = age < minObjSize ? minObjSize : age;

            DragonOnMap dragonOnMap = new DragonOnMap();
            dragonOnMap.setId(dragon.getId());
            double coord_x = dragon.getCoordinates().getX();
            int coord_y = dragon.getCoordinates().getY();
            dragonOnMap.setCoordinateX((int) coord_x);       //установка параметров для дракона на карте
            dragonOnMap.setCoordinateY(coord_y);
            dragonOnMap.setAge((int) age);
            dragonOnMap.setColor(userColors.get(dragon.getOwner()));

            Optional<DragonOnMap> dragon1 = dragonOnMaps.stream().filter(d -> d.getId() == dragonOnMap.getId()).findAny();
            if (dragon1.isPresent()) {
                addWithoutAnimation(dragonOnMap);
                dragon1.get().setCoordinateY(dragonOnMap.getCoordinateY());
                dragon1.get().setCoordinateX(dragonOnMap.getCoordinateX());
                dragon1.get().setAge(dragonOnMap.getAge());
            } else {
                if (count >= 2) {
                    dragonOnMaps.add(dragonOnMap);  //если
                    animationAdd(dragonOnMap);
                }
            }
        }
        DragonOnMap[] dragonOnMap = dragonOnMaps.toArray(new DragonOnMap[0]);
        for (DragonOnMap dragonM : dragonOnMap) {
            if (dragonArrayList.stream().noneMatch(d -> d.getId() == dragonM.getId())) {
                dragonOnMaps.remove(dragonM);
                animationRemove(dragonM);
            }
        }
    }

    private void addMouseListener() {
        this.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int x = e.getX();
                int y = e.getY();

                for (DragonOnMap dragonM : dragonOnMaps) {
                    int sizeX = vp.getWidth();
                    int sizeY = vp.getHeight();
                    int dragonX = dragonM.getCoordinateX();
                    int dragonY = dragonM.getCoordinateY();
                    int age = dragonM.getAge();
                    System.out.println((double) sizeX / 2 + (double) dragonX * sizeX / fieldLength - x);
                    System.out.println((double) sizeY / 2 - (double) dragonY * sizeY / fieldLength - y);

                    if (Math.abs((double) sizeX / 2 + (double) dragonX * sizeX / fieldLength - x) < (double) age * sizeY / fieldLength / 2 &&
                            (Math.abs((double) sizeY / 2 - (double) dragonY * sizeY / fieldLength - y) < (double) age * sizeY / fieldLength / 2)) {
                        UpdateDragonFrame udf = new UpdateDragonFrame("update", mainPanel);
                        udf.lockId();
                        udf.setVisible(false);
                        udf.setId(dragonM.getId());
                        GuiManagerImpl.centerFrame(udf);
                        udf.setVisible(true);
                        break;
                    }
                }
            }
        });
    }

    private void animationAdd(DragonOnMap dragonOnMap) {
        new Timer(10, new ActionListener() {
            final int size = dragonOnMap.getAge();
            int alpha = 0;
            int currentSize = 0;

            public void actionPerformed(ActionEvent e) {
                Graphics2D graphics2D = (Graphics2D) vp.getGraphics();
                if (!vp.isVisible()) return;
                int sizeX = vp.getWidth();
                int sizeY = vp.getHeight();
                int dragonX = dragonOnMap.getCoordinateX();
                int dragonY = dragonOnMap.getCoordinateY();

                currentSize = (int) (size * sizeY / fieldLength * Math.sin(Math.toRadians(alpha)));
                graphics2D.setColor(dragonOnMap.getColor());
                graphics2D.fillOval(sizeX / 2 + dragonX * sizeX / fieldLength - currentSize / 2, sizeY / 2 - dragonY * sizeY / fieldLength - currentSize / 2,
                        currentSize, currentSize);

                if (++alpha == 91) {
                    currentSize = (int) (size * sizeY / fieldLength * Math.sin(Math.toRadians(90)));
                    graphics2D.setColor(Color.black);
                    graphics2D.drawOval(sizeX / 2 + dragonX * sizeX / fieldLength - currentSize / 2, sizeY / 2 - dragonY * sizeY / fieldLength - currentSize / 2,
                            currentSize, currentSize);
                    ((Timer) e.getSource()).stop();
                }
            }
        }).start();
    }

    public void addWithoutAnimation(DragonOnMap dragonOnMap) {
        int sizeX = vp.getWidth();
        int sizeY = vp.getHeight();
        int dragonX = dragonOnMap.getCoordinateX();
        int dragonY = dragonOnMap.getCoordinateY();
        int age = dragonOnMap.getAge() * sizeY / fieldLength;

        graphics2D.setColor(dragonOnMap.getColor());
        graphics2D.fillOval(sizeX / 2 + dragonX * sizeX / fieldLength - age / 2, sizeY / 2 - dragonY * sizeY / fieldLength - age / 2,
                age, age);
        graphics2D.setColor(Color.black);
        graphics2D.drawOval(sizeX / 2 + dragonX * sizeX / fieldLength - age / 2, sizeY / 2 - dragonY * sizeY / fieldLength - age / 2,
                age, age);
    }

    public void animationRemove(DragonOnMap dragonOnMap) {
        new Timer(20, e -> {
            if (!vp.isVisible()) return;
            Graphics2D graphics2D = (Graphics2D) vp.getGraphics();
            if (!vp.isVisible()) return;
            int sizeX = vp.getWidth();
            int sizeY = vp.getHeight();
            int dragonX = dragonOnMap.getCoordinateX();
            int dragonY = dragonOnMap.getCoordinateY();
            int currentSize = dragonOnMap.getAge() * sizeY / fieldLength;

            graphics2D.setColor(dragonOnMap.getColor());
            graphics2D.fillOval(sizeX / 2 + dragonX * sizeX / fieldLength - currentSize / 2, sizeY / 2 - dragonY * sizeY / fieldLength - currentSize / 2,
                    currentSize, currentSize);
            repaint(new Rectangle(sizeX / 2 + dragonX * sizeX / fieldLength - currentSize / 2, sizeY / 2 - dragonY * sizeY / fieldLength - currentSize / 2,
                    currentSize, currentSize));
            dragonOnMap.setAge(dragonOnMap.getAge() - 1);
            if (currentSize <= 0)
                ((Timer) e.getSource()).stop();
        }).start();
    }
}