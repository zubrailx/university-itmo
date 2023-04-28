package components;

import java.time.Duration;

import org.openqa.selenium.WebDriver;

/**
 * Component
 */
public class Component {

  protected WebDriver driver;

  public Component(WebDriver driver) {
    this.driver = driver;
  }

  public final Duration getWaitTimeout() {
    return driver.manage().timeouts().getImplicitWaitTimeout();
  }
}
