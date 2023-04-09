package pages;

import helpers.PageUrl;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

public class HomePage extends XingPage {

  public HomePage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.HOME_PAGE);
    PageFactory.initElements(driver, this);
  }
}
