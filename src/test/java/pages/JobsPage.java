package pages;

import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class JobsPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  @FindBy(xpath = "//input[@id='Keywords-input']")
  private WebElement keywordInput;

  @FindBy(xpath = "//a[contains(@class, 'list-item-job')]/descendant::h3")
  private WebElement firstRecommendedJobName;

  @FindBy(xpath = "//a[contains(@class, 'list-item-job')]/descendant::button")
  private WebElement firstRecommendedJobSaveBtn;

  public JobsPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.JOBS_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }

  public JobsPage searchKeywords(String data) {
    var element = new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(keywordInput));
    element.sendKeys(data, Keys.ENTER);
    return new JobsPage(driver, true);
  }

  public String getFirstRecommendedJobNameText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstRecommendedJobName)).getText();
  }

  public JobsPage clickFirstRecommendedJobSaveBtn() {
    // scroll until element is visible
    JavascriptExecutor js = (JavascriptExecutor) driver;

    var element = new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstRecommendedJobSaveBtn));

    js.executeScript("scroll(0, 500)", "");
    element.click();

    return this;
  }

  public String getFirstRecommendedJobSaveBtnText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstRecommendedJobSaveBtn)).getText();
  }
}
