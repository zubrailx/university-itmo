package pages;

import java.time.Duration;

import org.openqa.selenium.WebDriver;

import helpers.PageUrl;

/**
 * BasePage
 */
abstract public class Page {

  protected WebDriver driver;

  public Page(WebDriver driver) {
    this.driver = driver;
  }

  protected final void driverGet(PageUrl pageUrl) {
    driver.get(pageUrl.getUrl());
  }

  public final String driverGetUrl() {
    return driver.getCurrentUrl();
  }

  public final Duration getWaitTimeout() {
    return driver.manage().timeouts().getImplicitWaitTimeout();
  }

  public final void refreshPage() {
    driver.get(driver.getCurrentUrl());
  }
}
