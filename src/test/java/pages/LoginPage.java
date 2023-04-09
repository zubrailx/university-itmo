package pages;

import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;

import helpers.PageUrl;

/**
 * LoginPage
 */
public class LoginPage extends XingPage {

  @FindBy(xpath = "//input[@type='checkbox']")
  private WebElement rememberCheckbox;

  @FindBy(xpath = "//input[@id='username']")
  private WebElement loginInput;

  @FindBy(xpath = "//input[@id='password']")
  private WebElement passwordInput;

  public LoginPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.LOGIN);
    PageFactory.initElements(driver, this);
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

  public LoginPage setLogin(String login) {
    loginInput.sendKeys(login);
    return this;
  }

  public LoginPage setPassword(String password) {
    passwordInput.sendKeys(password);
    return this;
  }

  public HomePage authenticate(String login, String password) {
    setLogin(login);
    setPassword(password);
    passwordInput.sendKeys(Keys.ENTER);
    return new HomePage(driver, true);
  }
}
