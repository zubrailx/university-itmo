package se.s312563.lab3.repository;

import lombok.Getter;
import lombok.Setter;
import se.s312563.lab3.entity.Point;
import se.s312563.lab3.repository.manager.PersistenceFactory;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.SessionScoped;
import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

@ManagedBean(name = "pointRepository")
@SessionScoped
@Getter
@Setter
public class PointRepositoryDAO implements Serializable, PointRepository {

    @ManagedProperty(value = "#{persistenceFactory}")
    private PersistenceFactory persistenceFactory;

    @Override
    public Point addEntity(Point p) {
        EntityManager entityManager = persistenceFactory.getEntityManagerFactory().createEntityManager();
        EntityTransaction entityTransaction = entityManager.getTransaction();
        try {
            entityTransaction.begin();
            entityManager.persist(p);
            entityTransaction.commit();

            entityManager.close();
            return p;
        } catch (Exception ex) {
            try {
                System.err.println("Exception at addEntity.");
                entityTransaction.rollback();
            } catch (Exception exc) {
                exc.printStackTrace();
            }
        }
        entityManager.close();
        return null;
    }

    @Override
    public List<Point> addEntityList(List<Point> pList) {
        List<Point> returnList = new ArrayList<>();
        for (Point p : pList) {
            Point ret = addEntity(p);
            if (ret != null) {
                returnList.add(ret);
            }
        }
        return returnList;
    }

    @Override
    public List<Point> getSessionEntityList() {
        EntityManager entityManager = persistenceFactory.getEntityManagerFactory().createEntityManager();
        List<Point> pList = null;
        try {
            pList = entityManager.createQuery("SELECT p FROM Point p", Point.class).getResultList();
        } catch (Exception ex) {
            System.err.println("Exception at getSessionEntityList.");
            ex.printStackTrace();
        }
        entityManager.close();
        return pList;
    }

    @Override
    public List<Point> deleteSessionEntityList() {
        List<Point> pList = getSessionEntityList();

        if (pList != null) {
            EntityManager entityManager = persistenceFactory.getEntityManagerFactory().createEntityManager();
            EntityTransaction entityTransaction = entityManager.getTransaction();
            try {
                entityTransaction.begin();
                for (Point p : pList) {
                    entityManager.remove(entityManager.contains(p) ? p : entityManager.merge(p));
                }
                entityTransaction.commit();
            } catch (Exception ex) {
                try {
                    System.err.println("Exception at deleteSessionEntityList.");
                    entityTransaction.rollback();
                } catch (Exception exc) {
                    exc.printStackTrace();
                }
            }
            entityManager.close();
        }
        return pList;
    }
}
