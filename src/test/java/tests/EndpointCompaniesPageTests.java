package tests;

import static org.junit.jupiter.api.Assertions.assertEquals;

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
public class EndpointCompaniesPageTests {

  Credentials credentials;

  @BeforeAll
  public void init() {
    credentials = new Credentials();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickLink_FromHome_EqualsPath(SeleniumDriver selDriver) {
    selDriver.setup();

    var companiesPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .authenticate(credentials.getLogin(), credentials.getPassword())
      .clickCompaniesLink();

    assertEquals(true, WaitHelpers.waitStringEqual(
            selDriver.getDriver(), companiesPage.getWaitTimeout(),
            PageUrl.COMPANIES_PAGE.getUrl()));

    selDriver.close();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void followUnfollowAndCheckName_EqualsWith(SeleniumDriver selDriver) {
    selDriver.setup();

    var companiesPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .authenticate(credentials.getLogin(), credentials.getPassword())
      .clickCompaniesLink();

    String firstCompanyName = companiesPage.getFirstCompanyNameText();

    // company is not followed
    assertEquals("Follow", companiesPage.getRecommendedFirstCompanyFollowBtnText());
    companiesPage.clickRecommendedFirstCompanyFollowBtn();

    companiesPage.clickFollowingBtn();
    assertEquals("Unfollow", companiesPage.getFollowingFirstCompanyUnfollowBtnText());
    companiesPage.clickFollowingFirstCompanyUnfollowBtn();

    assertEquals(firstCompanyName, companiesPage.getFirstCompanyNameText());

    companiesPage.clickRecommendedBtn();

    selDriver.close();
  }

}
