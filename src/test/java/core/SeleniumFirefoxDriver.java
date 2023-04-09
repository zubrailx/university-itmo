package core;

import java.time.Duration;

import org.openqa.selenium.Dimension;
import org.openqa.selenium.Point;
import org.openqa.selenium.firefox.FirefoxDriver;
import org.openqa.selenium.firefox.FirefoxOptions;

import io.github.bonigarcia.wdm.WebDriverManager;

public class SeleniumFirefoxDriver extends SeleniumDriver {


  public static final String VERSION = "0.32.0";
  public static final String FIREFOX_BINARY = "/usr/bin/firefox-bin";

  @Override
  public void setup() {
    FirefoxOptions options = new FirefoxOptions();
    options.setBinary(FIREFOX_BINARY);

    WebDriverManager.firefoxdriver().setup();

    driver = new FirefoxDriver(options);
    driver.manage().window().setSize(new Dimension(1024, 768));
    driver.manage().window().setPosition(new Point(0, 0));
    driver.manage().timeouts().pageLoadTimeout(Duration.ofSeconds(10));
    driver.manage().timeouts().implicitlyWait(Duration.ofSeconds(10));
  }

  @Override
  public void tear() {
    driver.close();
  }

}
