package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

abstract public class XingPage extends Page {

  @FindBy(xpath = "//div/descendant::div[contains(@class, 'cookie')]/button[2 and contains(@id, 'accept')]")
  protected WebElement cookiePopup;

  public XingPage(WebDriver driver) {
    super(driver);
  }

  public XingPage acceptCookiesIfClickable() {
    (new WebDriverWait(driver, getWaitTimeout()))
        .until(ExpectedConditions.elementToBeClickable(cookiePopup)).click();
    return this;
  }

}
