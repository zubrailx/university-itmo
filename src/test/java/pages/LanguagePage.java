package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import helpers.PageUrl;

/**
 * LanguagePage
 */
public class LanguagePage extends Page {

  public LanguagePage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.LANGUAGE);
    PageFactory.initElements(driver, this);
  }
  
}
