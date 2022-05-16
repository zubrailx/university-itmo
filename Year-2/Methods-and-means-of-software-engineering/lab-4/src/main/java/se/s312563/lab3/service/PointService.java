package se.s312563.lab3.service;

import se.s312563.lab3.entity.Point;

import java.util.List;

public interface PointService {

    Point addEntity(Point p);

    List<Point> addEntityList(List<Point> pList);

    List<Point> getSessionEntityList();

    List<Point> deleteSessionEntityList();

}
