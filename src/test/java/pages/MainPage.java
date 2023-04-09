package pages;

import org.openqa.selenium.WebDriver;

/**
 * MainPage
 */
public class MainPage extends Page {

  public MainPage(WebDriver driver) {
    super(driver);
    driver.get("https://www.xing.com/");
  }

}
