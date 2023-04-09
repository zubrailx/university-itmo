package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import helpers.PageUrl;

public class CompaniesPage extends XingPage {

  @FindBy(xpath = "//a[@href='/' and @data-qa='logo']")
  private WebElement homeLink;

  public CompaniesPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.COMPANIES_PAGE);
    PageFactory.initElements(driver, this);
  }

  public HomePage clickHomeLink() {
    (new WebDriverWait(driver, getWaitTimeout()))
        .until(ExpectedConditions.elementToBeClickable(homeLink)).click();
    return new HomePage(driver, true);
  }
}
