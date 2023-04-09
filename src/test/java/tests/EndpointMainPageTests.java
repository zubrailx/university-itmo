package tests;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
import helpers.PageUrl;
import pages.LanguagePage;
import pages.MainPage;

/**
 * MainPageTest
 */
public class EndpointMainPageTests {

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void getTitle_Equals(SeleniumDriver driver) {
    driver.setup();

    MainPage main = new MainPage(driver.getDriver(), false);

    assertEquals("XING - Find the right job for you", main.getTitle());
    assertEquals(PageUrl.MAIN.getUrl(), main.driverGetUrl());

    driver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickCookie_NotThrowsException(SeleniumDriver driver) {
    driver.setup();

    MainPage main = new MainPage(driver.getDriver(), false);

    assertDoesNotThrow(main::clickCookiePopup);

    driver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void getJobSearchTitle_AssertEquals(SeleniumDriver driver) {
    driver.setup();

    MainPage main = new MainPage(driver.getDriver(), false)
        .acceptCookiesIfClickable();

    for (var checkbox : MainPage.JobCheckbox.values()) {
      assertEquals(checkbox.getText(), main.getJobsSearchTitle(checkbox));
    }

    driver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void selectLanguage_English(SeleniumDriver driver) {
    driver.setup();

    MainPage mainPage = new MainPage(driver.getDriver(), false)
        .acceptCookiesIfClickable()
        .clickLanguageBtn()
        .selectLanguage(LanguagePage.Language.ENGLISH)
            ;

    assertEquals(PageUrl.MAIN_EN.getUrl(), mainPage.driverGetUrl());

    driver.tear();
  }

}
