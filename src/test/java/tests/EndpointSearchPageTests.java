package tests;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
import helpers.Credentials;
import helpers.StringModifier;
import pages.LoginPage;

/**
 * EndpointSearchPageTests
 */
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class EndpointSearchPageTests {

  Credentials credentials;

  @BeforeAll
  public void init() {
    credentials = new Credentials();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void search_and_getFirstResult_compareWithUsername(SeleniumDriver seleniumDriver) {
    seleniumDriver.setup();


    var homePage = new LoginPage(seleniumDriver.getDriver(), false)
        .acceptCookiesIfClickable()
        .authenticate(credentials.getLogin(), credentials.getPassword());

    var searchPage = homePage.getHeaderSearchComponent().writeEnterSearch(
      "software engineer"
    );

    var searchedUsername = searchPage.getFirstSearchResultUsername();

    assertEquals("Nikita Kulakov", searchedUsername);

    seleniumDriver.close();
  }

}
