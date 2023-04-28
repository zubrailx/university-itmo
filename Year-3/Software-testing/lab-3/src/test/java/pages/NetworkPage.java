package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class NetworkPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  @FindBy(xpath = "//div[@data-testid='Tile']/descendant::img[contains(@src, 'contacts')]/following-sibling::div/p")
  private WebElement contacts;

  @FindBy(xpath = "//div[@data-testid='Tile']/descendant::img[contains(@src, 'birthday')]/following-sibling::div/p")
  private WebElement birthdays;

  @FindBy(xpath = "//div[@data-testid='TileContactRequest']/div/div/p")
  private WebElement contactRequests;


  public NetworkPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.NETWORK_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }

  public String getNoContactsString() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(contacts)).getText();
  }

  public String getNoBirthdaysString() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(birthdays)).getText();
  }

  public String getNoContactRequestsString() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(contactRequests)).getText();
  }
}
