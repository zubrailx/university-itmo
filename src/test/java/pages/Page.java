package pages;

import org.openqa.selenium.WebDriver;

/**
 * BasePage
 */
abstract public class Page {

  protected WebDriver driver;

  public Page(WebDriver driver) {
    this.driver = driver;
  }
}
