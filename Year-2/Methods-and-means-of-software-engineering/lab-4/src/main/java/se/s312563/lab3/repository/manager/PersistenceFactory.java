package se.s312563.lab3.repository.manager;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.io.Serializable;

@ManagedBean(name = "persistenceFactory")
@SessionScoped
public class PersistenceFactory implements Serializable {

    private EntityManagerFactory emf;

    @PostConstruct
    public void init() {
        try {
            emf = Persistence.createEntityManagerFactory("PointUnit");
        } catch (Exception exc){
            exc.printStackTrace();
        }
    }

    public EntityManagerFactory getEntityManagerFactory() {
        return emf;
    }
}
