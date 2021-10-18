package se.s312563.lab3.service;

import se.s312563.lab3.entity.Point;
import se.s312563.lab3.repository.PointRepository;

import javax.enterprise.context.SessionScoped;
import javax.inject.Inject;
import javax.inject.Named;
import java.io.Serializable;
import java.util.List;

@Named("service.pointService")
@SessionScoped
public class PointServiceJPA implements PointService, Serializable {

    @Inject
    @Named("repository.pointRepository")
    PointRepository pointRepository;

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

