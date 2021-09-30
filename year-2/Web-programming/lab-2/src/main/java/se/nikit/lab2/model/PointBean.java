package se.nikit.lab2.model;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class PointBean implements Serializable {
    private static final long serialVersionUID = 1L;

    private final List<Point> pointList;

    public PointBean(){
        pointList = new CopyOnWriteArrayList<>();
    }

    public void addFirst(Point p){ pointList.add(0, p); }

    public List<Point> getList(){ return pointList; }

    public void clearList(){
        pointList.clear();
    }
}
