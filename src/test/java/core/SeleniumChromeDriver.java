package core;

import java.time.Duration;

import org.openqa.selenium.Dimension;
import org.openqa.selenium.Point;
import org.openqa.selenium.chrome.ChromeDriver;

import io.github.bonigarcia.wdm.WebDriverManager;

public class SeleniumChromeDriver extends SeleniumDriver {

  public static final String VERSION = "111.0.5563.64";

  @Override
  public void setup() {
    WebDriverManager.chromedriver().driverVersion(VERSION).setup();
    driver = new ChromeDriver();
    driver.manage().window().setSize(new Dimension(1024, 768));
    driver.manage().window().setPosition(new Point(0, 0));
    driver.manage().timeouts().pageLoadTimeout(Duration.ofSeconds(10));
    driver.manage().timeouts().implicitlyWait(Duration.ofSeconds(10));
  }

  @Override
  public void tear() {
    driver.close();
    driver.quit();
  }

}
