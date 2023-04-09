package tests;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

import helpers.StringModifier;
import helpers.WaitHelpers;
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
import pages.HomePage;
import pages.MainPage;
import pages.YourPostsPage;

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
        .clickHomeLink();

    assertTrue(
        (new WebDriverWait(driver, homePage.getWaitTimeout()))
            .until((ExpectedCondition<Boolean>) (u) -> PageUrl.HOME_PAGE.getUrl().equals(homePage.driverGetUrl())));

    selDriver.tear();
  }

  // NOTE: firefox doesn't pass often, because it's driver writes wrong value in text input
  // so -> no such element
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
            .writeToOpenedPost(postContent)
            .setPostPrivate()
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
