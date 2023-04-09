package pages;

import helpers.PageUrl;
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.FluentWait;
import org.openqa.selenium.support.ui.WebDriverWait;

import java.time.Duration;
import java.time.temporal.ChronoUnit;

public class HomePage extends XingPage {

  @FindBy(xpath = "//a[@href='/' and @data-qa='logo']")
  private WebElement homeLink;

  @FindBy(xpath = "//button[@data-testid='default-commbox-entry-point']")
  private WebElement postBtn;

  @FindBy(xpath = "//div[@role='textbox']")
  private WebElement postTextbox;

  @FindBy(xpath = "//div[@data-xds='Card']/descendant-or-self::img[contains(@src,'users')]")
  private WebElement postAvatar;

  @FindBy(xpath = "//button[@data-testid='audienceSelectionButton']")
  private WebElement whoCanSeeBtn;

  @FindBy(xpath = "//div[@data-xds='PopOver']/ul/li[2]/button")
  private WebElement privatePostBtn;

  @FindBy(xpath = "//button[@data-testid='commboxPost']")
  private WebElement sendPostBtn;

  @FindBy(xpath = "//a[contains(@href, 'you')]")
  private WebElement youLink;

  public HomePage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.HOME_PAGE);
    PageFactory.initElements(driver, this);
  }

  public HomePage openPost() {
    var wait = new FluentWait<>(driver)
            .withTimeout(getWaitTimeout())
            .pollingEvery(Duration.of(1000, ChronoUnit.MILLIS));
    // wait until avatar is loaded
    wait.until( u -> ((JavascriptExecutor)driver).executeScript("return arguments[0].complete", postAvatar));
    var element = wait.until(ExpectedConditions.elementToBeClickable(postBtn));
    element.click();
    return this;
  }

  public HomePage writeToOpenedPost(String data) {
    var wait = new FluentWait<>(driver)
            .withTimeout(getWaitTimeout())
            .pollingEvery(Duration.of(1000, ChronoUnit.MILLIS));

    var element = wait.until(ExpectedConditions.elementToBeClickable(postTextbox));
    element.sendKeys(Keys.BACK_SPACE);
    element.sendKeys(data);
    return this;
  }

  public HomePage setPostPrivate() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(whoCanSeeBtn)).click();

    new WebDriverWait(driver, getWaitTimeout())
            .until(ExpectedConditions.elementToBeClickable(privatePostBtn)).click();
    return this;
  }

  public HomePage sendPost() {
    (new WebDriverWait(driver, getWaitTimeout()))
        .until(ExpectedConditions.elementToBeClickable(sendPostBtn)).click();
    return new HomePage(driver, true);
  }

  public HomePage clickHomeLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(homeLink)).click();
    return new HomePage(driver, true);
  }

  public YouPage clickYouLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(youLink)).click();
    return new YouPage(driver, true);
  }

}
