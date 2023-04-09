package tests;

import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
import helpers.Credentials;
import helpers.PageUrl;
import helpers.WaitHelpers;
import pages.LoginPage;

/**
 * EndpointCompaniesPageTests
 */
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class EndpointMessagesPageTests {

  Credentials credentials;

  @BeforeAll
  public void init() {
    credentials = new Credentials();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickLink_FromHome_EqualsPath(SeleniumDriver selDriver) {
    selDriver.setup();

    var messagesPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .authenticate(credentials.getLogin(), credentials.getPassword())
      .clickMessagesLink();

    assertTrue(WaitHelpers.waitStringEqual(
           selDriver.getDriver(), messagesPage.getWaitTimeout(),
           PageUrl.MESSAGES_PAGE.getUrl()));

    selDriver.close();
  }
  
}
