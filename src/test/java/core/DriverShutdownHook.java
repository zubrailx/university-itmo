package core;

import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.openqa.selenium.WebDriver;

/**
 * ShutdownHook
 */
public class DriverShutdownHook {

  @Test
  @Disabled
  public void addShutdownHook() {
    Runtime.getRuntime().addShutdownHook(new Thread(() -> {
      SeleniumFirefoxDriver firefoxDriver = new SeleniumFirefoxDriver();
      firefoxDriver.setup();
      firefoxDriver.getDriver().quit();

      SeleniumChromeDriver chromeDriver = new SeleniumChromeDriver();
      chromeDriver.setup();
      chromeDriver.getDriver().quit();

      System.out.println("FINISHED SHUTDOWN HOOK");
    }));
  }
}
