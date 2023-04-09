package tests;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
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

    assertEquals("Remote", main.getJobsSearchTitle("remote"));
    assertEquals("Part-time", main.getJobsSearchTitle("partTime"));
    assertEquals("Flexitime", main.getJobsSearchTitle("flexible"));
    assertEquals("Dogs welcome", main.getJobsSearchTitle("dogsFriendly"));

    driver.tear();
  }

}
