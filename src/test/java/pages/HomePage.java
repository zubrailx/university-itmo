package pages;

import java.time.Duration;
import java.time.temporal.ChronoUnit;

import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.FluentWait;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HeaderSearchComponent;
import components.HomeLogoComponent;
import helpers.PageUrl;

public class HomePage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;
  private final HeaderSearchComponent headerSearchComponent;

  @FindBy(xpath = "//a[contains(@href, '/chats')]")
  private WebElement messagesLink;

  @FindBy(xpath = "//a[contains(@href, 'you')]")
  private WebElement youLink;

  @FindBy(xpath = "//a[contains(@href, '/jobs')]")
  private WebElement jobsLink;

  @FindBy(xpath = "//li/a[contains(@href, '/premium')]")
  private WebElement premiumLink;

  @FindBy(xpath = "//li/a[contains(@href, '/network')]")
  private WebElement networkLink;

  @FindBy(xpath = "//li/a[contains(@href, '/news')]")
  private WebElement newsLink;

  @FindBy(xpath = "//li/a[contains(@href, '/companies')]")
  private WebElement companiesLink;

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

  public HomePage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.HOME_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
    headerSearchComponent = new HeaderSearchComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }

  public HeaderSearchComponent getHeaderSearchComponent() {
    return headerSearchComponent;
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

    // send char by char to fix result for firefox webdriver
    for (var key : data.toCharArray()) {
      element.sendKeys(String.valueOf(key));
    }

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

  // Links (Can be managed in separate component)
  public MessagesPage clickMessagesLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(messagesLink)).click();
    return new MessagesPage(driver, true);
  }

  public YouPage clickYouLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(youLink)).click();
    return new YouPage(driver, true);
  }

  public JobsPage clickJobsLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(jobsLink)).click();
    return new JobsPage(driver, true);
  }

  public PremiumPage clickPremiumLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(premiumLink)).click();
    return new PremiumPage(driver, true);
  }

  public NetworkPage clickNetworkLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(networkLink)).click();
    return new NetworkPage(driver, true);
  }

  public NewsPage clickNewsLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(newsLink)).click();
    return new NewsPage(driver, true);
  }

  public CompaniesPage clickCompaniesLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(companiesLink)).click();
    return new CompaniesPage(driver, true);
  }

}
