package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class PurchasePremiumPage extends XingPage {

  private HomeLogoComponent homeLogoComponent;

  public PurchasePremiumPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.PURCHASE_PREMIUM_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }
}
