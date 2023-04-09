package core;

import org.openqa.selenium.WebDriver;

public abstract class SeleniumDriver {

  protected WebDriver driver;

  public final WebDriver getDriver() {
    return driver;
  }

  public abstract void setup();

  public abstract void tear();

}
