package tests;

import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

import core.SeleniumDriver;
import helpers.Credentials;
import helpers.PageUrl;
import pages.HomePage;
import pages.MainPage;

public class EndpointHomePageTests {

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void authenticate_ShouldBeGood(SeleniumDriver selDriver) {
    selDriver.setup();
    WebDriver driver = selDriver.getDriver();

    Credentials credentials = new Credentials();

    HomePage homePage = new MainPage(driver, false)
        .acceptCookiesIfClickable()
        .clickLoginBtn()
        .authenticate(credentials.getLogin(), credentials.getPassword());

    assertTrue(
        (new WebDriverWait(driver, driver.manage().timeouts().getImplicitWaitTimeout()))
            .until((ExpectedCondition<Boolean>) (u) -> PageUrl.HOME_PAGE.getUrl().equals(homePage.driverGetUrl())));

    selDriver.tear();
  }
}
