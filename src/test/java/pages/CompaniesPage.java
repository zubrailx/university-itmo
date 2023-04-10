package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class CompaniesPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  @FindBy(xpath = "//a[@copy='Recommended']")
  private WebElement recommendedBtn;

  @FindBy(xpath = "//a[@copy='Following']")
  private WebElement followingBtn;

  @FindBy(xpath = "//div[contains(@class, 'container-company-card')]/descendant::div[@data-xds='Card']")
  private WebElement firstCard;

  @FindBy(xpath = "//div[contains(@class, 'container-company-card')]/descendant::div[@data-xds='Card']/descendant::strong")
  private WebElement firstCompanyName;

  @FindBy(xpath = "//div[contains(@class, 'container-company-card')]/descendant::div[@data-xds='Card']/descendant::button[2]")
  private WebElement firstCompanyFollowBnt;

  @FindBy(xpath = "//div[contains(@class, 'container-company-card')]/descendant::div[@data-xds='Card']/descendant::button")
  private WebElement firstFollowingUnfollowBtn;


  public CompaniesPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.COMPANIES_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }

  public CompaniesPage clickRecommendedBtn() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(recommendedBtn)).click();
    return this;
  }

  public CompaniesPage clickFollowingBtn() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(followingBtn)).click();
    return this;
  }
  
  public String getFirstCompanyNameText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.visibilityOf(firstCompanyName)).getText();
  }

  public CompaniesPage clickRecommendedFirstCompanyFollowBtn() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstCompanyFollowBnt)).click();
    return this;
  }

  public String getRecommendedFirstCompanyFollowBtnText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstCompanyFollowBnt)).getText();
  }

  public CompaniesPage clickFollowingFirstCompanyUnfollowBtn() {
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstFollowingUnfollowBtn)).click();
    return this;
  }

  public String getFollowingFirstCompanyUnfollowBtnText() {
    return new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(firstFollowingUnfollowBtn)).getText();
  }
}
