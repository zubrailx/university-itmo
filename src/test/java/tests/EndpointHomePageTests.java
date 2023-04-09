package tests;

import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

import core.SeleniumDriver;
import helpers.Credentials;
import helpers.PageUrl;
import helpers.StringModifier;
import helpers.WaitHelpers;
import pages.HomePage;
import pages.MainPage;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class EndpointHomePageTests {

  Credentials credentials;

  @BeforeAll
  public void init() {
    credentials = new Credentials();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void authenticate_ShouldBeGood(SeleniumDriver selDriver) {
    selDriver.setup();
    WebDriver driver = selDriver.getDriver();

    HomePage homePage = new MainPage(driver, false)
        .acceptCookiesIfClickable()
        .clickLoginBtn()
        .authenticate(credentials.getLogin(), credentials.getPassword());

    assertTrue(
        (new WebDriverWait(driver, homePage.getWaitTimeout()))
            .until((ExpectedCondition<Boolean>) (u) -> PageUrl.HOME_PAGE.getUrl().equals(homePage.driverGetUrl())));

    selDriver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void redirectToHome_AssertPath(SeleniumDriver selDriver) {
    selDriver.setup();
    WebDriver driver = selDriver.getDriver();


    HomePage homePage = new MainPage(driver, false)
        .acceptCookiesIfClickable()
        .clickLoginBtn()
        .authenticate(credentials.getLogin(), credentials.getPassword())
        .getHomeLogoComponent()
        .clickHomeLink();

    assertTrue(
        (new WebDriverWait(driver, homePage.getWaitTimeout()))
            .until((ExpectedCondition<Boolean>) (u) -> PageUrl.HOME_PAGE.getUrl().equals(homePage.driverGetUrl())));

    selDriver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void createAndDeletePost_WithURLAndPostContentChecks(SeleniumDriver selDriver) {
    selDriver.setup();
    WebDriver driver = selDriver.getDriver();

    String postContent = StringModifier.getUniqueString("post");

    HomePage homePage = new MainPage(driver, false)
            .acceptCookiesIfClickable()
            .clickLoginBtn()
            .authenticate(credentials.getLogin(), credentials.getPassword())
            .openPost()
            .setPostPrivate()
            .writeToOpenedPost(postContent)
            .sendPost();

    // validate that got redirected to home page
    assertTrue(WaitHelpers.waitStringEqual(
            driver, homePage.getWaitTimeout(),
            PageUrl.HOME_PAGE.getUrl()));

    homePage = homePage
            .clickYouLink()
            .clickYourPostLink()
            .deletePostByContent(postContent)
            .clickHomeLink();

    // validate again
    assertTrue(WaitHelpers.waitStringEqual(
           driver, homePage.getWaitTimeout(),
           PageUrl.HOME_PAGE.getUrl()));

    selDriver.tear();
  }
}
