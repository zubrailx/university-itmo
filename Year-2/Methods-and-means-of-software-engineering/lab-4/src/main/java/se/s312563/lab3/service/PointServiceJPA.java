package se.s312563.lab3.service;

import se.s312563.lab3.entity.Point;
import se.s312563.lab3.repository.PointRepository;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.SessionScoped;
import java.io.Serializable;
import java.util.List;

@ManagedBean(name = "pointService")
@SessionScoped
public class PointServiceJPA implements PointService, Serializable {

    @ManagedProperty(value = "#{pointRepository}")
    PointRepository pointRepository;

    public PointRepository getPointRepository() {
        return pointRepository;
    }

    public void setPointRepository(PointRepository pointRepository) {
        this.pointRepository = pointRepository;
    }

    @Override
    public Point addEntity(Point p) {
        return pointRepository.addEntity(p);
    }

    @Override
    public List<Point> addEntityList(List<Point> pList) {
        return pointRepository.addEntityList(pList);
    }

    @Override
    public List<Point> getSessionEntityList() {
        return pointRepository.getSessionEntityList();
    }

    @Override
    public List<Point> deleteSessionEntityList() {
        return pointRepository.deleteSessionEntityList();
    }
}

