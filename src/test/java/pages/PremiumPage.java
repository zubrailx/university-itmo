package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class PremiumPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  @FindBy(xpath = "//a[contains(@href, 'premium_offer')]")
  private WebElement goPremiumNow;

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

  public String getGoPremiumNowText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(goPremiumNow)).getText();
  }

  public PurchasePremiumPage clickPurcasePremium() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(goPremiumNow)).click();
    return new PurchasePremiumPage(driver, true);
  }
}
