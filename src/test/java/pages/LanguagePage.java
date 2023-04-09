package pages;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import helpers.PageUrl;

/**
 * LanguagePage
 */
public class LanguagePage extends XingPage {

  public enum Language {
    GERMAN("de", "Deutsch"),
    ENGLISH("en", "English"),
    SPANISH("es", "Español"),
    FRENCH("fr", "Français"),
    ITALIAN("it", "Italiano");

    private final String lang, text;

    private Language(String lang, String text) {
      this.lang = lang;
      this.text = text;
    }

    public String getLang() {
      return lang;
    }

    public String getText() {
      return text;
    }
  }

  public LanguagePage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.LANGUAGE);
    PageFactory.initElements(driver, this);
  }

  public String getLanguage(Language lang) {
    String xpath = String.format("//div[@language='%s']/a", lang.getLang());
    return driver.findElement(By.xpath(xpath)).getText();
  }

  public MainPage selectLanguage(Language lang) {
    String xpath = String.format("//div[@language='%s']/a", lang.getLang());
    driver.findElement(By.xpath(xpath)).click();
    return new MainPage(driver, true);
  }
  
  @Override
  public LanguagePage acceptCookiesIfClickable() {
    return (LanguagePage) super.acceptCookiesIfClickable();
  }
}
