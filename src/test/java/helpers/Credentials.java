package helpers;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

/**
 * Credentials
 */
public class Credentials {

  private String login;
  private String password;

  public Credentials() {
    try (InputStream inputStream = new FileInputStream("credentials.properties")) {
      Properties props = new Properties();
      props.load(inputStream);

      login = props.getProperty("login");
      password = props.getProperty("password");

    } catch (IOException ex) {
      ex.printStackTrace();
    }
  }

  public String getLogin() {
    return login;
  }

  public String getPassword() {
    return password;
  }

}
