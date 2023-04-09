package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import components.HomeLogoComponent;
/**
 * SearchPage
 */
import helpers.PageUrl;

public class SearchPage extends XingPage {

  private HomeLogoComponent homeLogoComponent;

  public SearchPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.SEARCH_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }
}
