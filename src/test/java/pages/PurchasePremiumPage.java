package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class PurchasePremiumPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  @FindBy(xpath = "//div[@data-testid='pricing-box']/p/span")
  private WebElement premiumPrice;

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

  public String getPremiumPrice() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(premiumPrice)).getText();
  }
}
