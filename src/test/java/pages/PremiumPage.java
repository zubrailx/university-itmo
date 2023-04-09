package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class PremiumPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  public PremiumPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.PREMIUM_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }
}
