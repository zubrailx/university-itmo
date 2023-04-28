package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;

import helpers.PageUrl;

/**
 * MainPage
 */
public class MainPage extends XingPage {

  @FindBy(xpath = "//child::*[contains(@href, 'language')]")
  private WebElement languageBtn;

  @FindBy(xpath = "//a[contains(@href, 'login')]")
  private WebElement loginBtn;

  String xpathInputContainer = "//form/div[contains(@class, 'Filter')]/descendant::";

  public enum JobCheckbox {
    REMOTE("Remote", "remote"),
    PART_TIME("Part-time", "partTime"),
    FLEXITIME("Flexitime", "flexible"),
    DOGS_WELCOME("Dogs welcome", "dogsFriendly");

    private final String text, name;

    JobCheckbox(String text, String name) {
      this.text = text;
      this.name = name;
    }

    public String getText() {
      return text;
    }

    public String getName() {
      return name;
    }
  };

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

  public MainPage clickJobsSearch(JobCheckbox name) {
    String xpath = String.format(xpathInputContainer + "input[@name='%s']", name.getName());
    driver.findElement(By.xpath(xpath)).click();
    return this;
  }

  public String getJobsSearchTitle(JobCheckbox name) {
    String xpath = String.format(
        "//form/div[contains(@class, 'Filter')]/descendant::input[@name='%s']/../..", name.getName());
    return driver.findElement(By.xpath(xpath)).getText();
  }

  public LanguagePage clickLanguageBtn() {
    languageBtn.click();
    return new LanguagePage(driver, true);
  }

  public LoginPage clickLoginBtn() {
    loginBtn.click();
    return new LoginPage(driver, true);
  }

  @Override
  public MainPage acceptCookiesIfClickable() {
    return (MainPage) super.acceptCookiesIfClickable();
  }

}
