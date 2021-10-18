package se.s312563.lab3.repository.manager;

import javax.annotation.PostConstruct;
import javax.enterprise.context.SessionScoped;
import javax.inject.Inject;
import javax.inject.Named;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

@Named("repository.persistenceFactory")
@SessionScoped
public class PersistenceFactory implements Serializable {

    private EntityManagerFactory emf;

    @Inject
    private Credentials credentials;

    @PostConstruct
    public void init() {
        Map<String, String> result = new HashMap<>();
        result.put("javax.persistence.jdbc.user", credentials.getUsername());
        result.put("javax.persistence.jdbc.password", credentials.getPassword());
        try {
            emf = Persistence.createEntityManagerFactory("Points", result);
        } catch (Exception ex) {
            System.err.println("Установить соединение на локальной машине не удалось, устанавливаем параметры для helios...");
            try{
                result.put("javax.persistence.jdbc.url","jdbc:postgresql://pg:5432/studs");
                emf = Persistence.createEntityManagerFactory("Points", result);
            } catch (Exception exc){
                exc.printStackTrace();
            }
        }
    }

    public EntityManagerFactory getEntityManagerFactory() {
        return emf;
    }
}
