package pages;

import java.time.Duration;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import helpers.PageUrl;

/**
 * MainPage
 */
public class MainPage extends Page {

  @FindBy(xpath = "//div/descendant::div[contains(@class, 'cookie')]/button[2 and contains(@id, 'accept')]")
  private WebElement cookiePopup;

  String xpathInputContainer = "//form/div[contains(@class, 'Filter')]/descendant::";

  public MainPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.MAIN);

    PageFactory.initElements(driver, this);
  }

  public String getTitle() {
    return driver.getTitle();
  }

  public MainPage clickCookiePopup() {
    cookiePopup.click();
    return this;
  }

  public MainPage acceptCookiesIfClickable() {
    WebDriverWait wait = new WebDriverWait(driver, Duration.ofSeconds(10));
    wait.until(ExpectedConditions.elementToBeClickable(cookiePopup)).click();
    return this;
  }

  public MainPage clickJobsSearch(String name) {
    String xpath = String.format(xpathInputContainer + "input[@name='%s']", name);
    driver.findElement(By.xpath(xpath)).click();
    return this;
  }

  public String getJobsSearchTitle(String name) {
    String xpath = String.format(
        "//form/div[contains(@class, 'Filter')]/descendant::input[@name='%s']/../..", name);
    return driver.findElement(By.xpath(xpath)).getText();
  }

}
