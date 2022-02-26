package se.s312563.lab4.config;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.autoconfigure.jdbc.DataSourceProperties;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;

import javax.sql.DataSource;
import java.io.File;
import java.util.NoSuchElementException;
import java.util.Scanner;

@Configuration
@Component
public class DataSourceConfigProperties {

    Logger log = LoggerFactory.getLogger(DataSourceConfigProperties.class);

    @Bean
    @Primary
    @ConfigurationProperties("spring.datasource")
    public DataSourceProperties dataSourceProperties() {
        log.info("Setting login and password for db connection...");
        Credentials cr = new Credentials();
        DataSourceProperties dsp = new DataSourceProperties();
        dsp.setUsername(cr.getUsername());
        dsp.setPassword(cr.getPassword());
        return dsp;
    }

    @Bean
    @ConfigurationProperties("app.datasource")
    public DataSource dataSource(DataSourceProperties properties) {
        log.info("Getting dataSource from properties...");
        return properties.initializeDataSourceBuilder().build();
    }
}

class Credentials {
    private String username = "";
    private String password = "";

    public Credentials() {
        try {
            String path = "credentials.txt";
            File file = new File(path);
            System.out.println("Path: " + file.getAbsolutePath());
            Scanner scanner = new Scanner(file);
            username = scanner.nextLine().trim();
            password = scanner.nextLine().trim();
        } catch (NoSuchElementException ex) {
            System.err.println("Credentials file is incorrect.");
        } catch (Exception ex) {
            System.err.println("Exception in " + this.getClass().getName());
            ex.printStackTrace();
        }
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }
}
