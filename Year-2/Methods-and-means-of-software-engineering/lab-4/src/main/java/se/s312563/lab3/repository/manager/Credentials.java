package se.s312563.lab3.repository.manager;

import javax.annotation.PostConstruct;
import javax.enterprise.context.ApplicationScoped;
import java.io.File;
import java.util.NoSuchElementException;
import java.util.Scanner;

@ApplicationScoped
public class Credentials {
    private String username = "";
    private String password = "";

    @PostConstruct
    public void init() {
        try {
            String path = "credentials.txt";
            File file = new File(path);

            String paath = file.getAbsolutePath();
            System.out.println(file.getAbsolutePath());
            Scanner scanner = new Scanner(file);
            username = scanner.nextLine().trim();
            password = scanner.nextLine().trim();
        } catch (NoSuchElementException ex) {
            System.err.println("Credentials file is incorrect.");
        } catch (Exception ex) {
            System.err.println("Exception in " + this.getClass().getName());
        }
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }
}
