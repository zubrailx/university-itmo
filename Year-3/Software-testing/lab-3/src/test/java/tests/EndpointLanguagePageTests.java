package tests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
import pages.LanguagePage;
import pages.MainPage;

public class EndpointLanguagePageTests {

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void getLanguage_AssertEquals(SeleniumDriver selDriver) {
    selDriver.setup();

    LanguagePage languagePage = new MainPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .clickLanguageBtn();

    for (var language : LanguagePage.Language.values()) {
      assertEquals(language.getText(), languagePage.getLanguage(language));
    }

    selDriver.close();
  }
}
