package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;

import helpers.Credentials;
import helpers.PageUrl;

/**
 * LoginPage
 */
public class LoginPage extends XingPage {

  @FindBy(xpath = "//input[@type='checkbox']")
  private WebElement rememberCheckbox;

  private Credentials credentials;

  public LoginPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.LOGIN);
    PageFactory.initElements(driver, this);

    credentials = new Credentials();
    System.out.println(credentials.getLogin());
    System.out.println(credentials.getPassword());
  }

  public boolean isRememberSel() {
    return rememberCheckbox.isSelected();
  }

  public LoginPage clickRemember() {
    rememberCheckbox.click();
    return this;
  }

  @Override
  public LoginPage acceptCookiesIfClickable() {
    return (LoginPage) super.acceptCookiesIfClickable();
  }
}
