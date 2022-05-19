package se.s312563.lab3.repository.manager;

import org.hibernate.service.spi.ServiceException;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.io.File;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;

@ManagedBean(name = "persistenceFactory")
@SessionScoped
public class PersistenceFactory implements Serializable {
    private static final String LOGIN_AND_PASSWORD_FILE = "credentials.txt";
    private static final String JDBC_URL_LOCALHOST = "jdbc:postgresql://localhost:5432/studs";
    private static final String JDBC_URL_HELIOS = "jdbc:postgresql://pg:5432/studs";

    private static final String JDBC_URL_DOCKER = "jdbc:postgresql://localhost:5432/postgres";
    private static final String JDBC_DOCKER_USER = "postgres";
    private static final String JDBC_DOCKER_PASSWORD = "postgres";

    private static String jdbcUser;
    private static String jdbcPassword;


    private EntityManagerFactory emf;


    public void init() {
        Map<String, String> result = new HashMap<>();

        if (jdbcUser == null && jdbcPassword == null) {
            readUserAndPasswordFromFile();
        }
        try {
//            result.put("javax.persistence.jdbc.user", jdbcUser);
//            result.put("javax.persistence.jdbc.password", jdbcPassword);
//            result.put("javax.persistence.jdbc.url", JDBC_URL_LOCALHOST);
            result.put("javax.persistence.jdbc.user", JDBC_DOCKER_USER);
            result.put("javax.persistence.jdbc.password", JDBC_DOCKER_PASSWORD);
            result.put("javax.persistence.jdbc.url", JDBC_URL_DOCKER);
            emf = Persistence.createEntityManagerFactory("PointUnit", result);
        } catch (IllegalStateException | ServiceException  exc){
            System.out.println("Trying to connect using localhost...");
            try {
                result.put("javax.persistence.jdbc.url", JDBC_URL_HELIOS);
                emf = Persistence.createEntityManagerFactory("PointUnit", result);
            } catch (IllegalStateException | ServiceException ex) {
                ex.printStackTrace();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public EntityManagerFactory getEntityManagerFactory() {
        if (emf == null) {
            init();
        }
        return emf;
    }


    private static void readUserAndPasswordFromFile() {
        try {
            File file = new File(LOGIN_AND_PASSWORD_FILE);
            System.out.println("Expected file location: " + file.getAbsolutePath());
            Scanner scanner = new Scanner(file);
            jdbcUser = scanner.nextLine().trim();
            jdbcPassword = scanner.nextLine().trim();
        } catch (NoSuchElementException ex) {
            System.err.println("Credentials file is not found.");
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
