package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

/**
 * YouPage
 */
import helpers.PageUrl;

public class YouPage extends XingPage {

  @FindBy(xpath = "//a[contains(@href, 'your-posts')]")
  private WebElement yourPostLink;

  public YouPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.YOU_PAGE);
    PageFactory.initElements(driver, this);
  }

  public YourPostsPage clickYourPostLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
                .until(ExpectedConditions.elementToBeClickable(yourPostLink)).click();
    return new YourPostsPage(driver, true);
  }
}
