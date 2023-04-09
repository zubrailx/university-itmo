package pages;

/**
 * YouPage
 */
import helpers.PageUrl;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

public class YourPostsPage extends XingPage {

  @FindBy(xpath = "//button[@data-qa='dot-menu-item-delete']")
  private WebElement delBtn;

  @FindBy(xpath = "//div[@data-xds='Dialog']/descendant::span[text()='Delete post']/ancestor::button")
  private WebElement popupDelBtn;

  @FindBy(xpath = "//a[@href='/' and @data-qa='logo']")
  private WebElement homeLink;

  public YourPostsPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.YOUR_POSTS);
    PageFactory.initElements(driver, this);
  }

  private Boolean hasFoundPostByContent(String content) {
    String xpath = String.format("//span[text()='%s']", content);
    var elements = driver.findElements(By.xpath(xpath));
    return elements.isEmpty();
  }

  public YourPostsPage deletePostByContent(String content) {
    String menuButtonPattern = "//span[text()='%s']/ancestor::article/descendant::button[@data-qa='dot" +
            "-menu-button']";

    String xpathDelBtn = String .format(menuButtonPattern, content);
    WebElement selBtn = driver.findElement(By.xpath(xpathDelBtn));
    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(selBtn)).click();

    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(delBtn)).click();

    new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(popupDelBtn)).click();

    return this;
  }

  public HomePage clickHomeLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
            .until(ExpectedConditions.elementToBeClickable(homeLink)).click();
    return new HomePage(driver, true);
  }

}
