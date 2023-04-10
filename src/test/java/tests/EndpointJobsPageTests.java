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
public class EndpointJobsPageTests {

  Credentials credentials;

  @BeforeAll
  public void init() {
    credentials = new Credentials();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickLink_FromHome_EqualsPath(SeleniumDriver selDriver) {
    selDriver.setup();

    var jobsPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .authenticate(credentials.getLogin(), credentials.getPassword())
      .clickJobsLink();

    assertEquals(true, WaitHelpers.waitStringEqual(
            selDriver.getDriver(), jobsPage.getWaitTimeout(),
            PageUrl.JOBS_PAGE.getUrl()));

    selDriver.close();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void saveAndUnsaveJob_CheckSaveJobBtnAndJobName(SeleniumDriver selDriver) {
    selDriver.setup();

    var jobsPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .authenticate(credentials.getLogin(), credentials.getPassword())
      .clickJobsLink();

    jobsPage.searchKeywords("software developer");

    String saveJob = "Save job";
    String saved = "Saved";

    // save job or unsave
    String firstRecJobName = jobsPage.getFirstRecommendedJobNameText();

    boolean eqSaveJob = jobsPage.getFirstRecommendedJobNameText().equals("Save job");
    if (eqSaveJob) {
      assertEquals(saveJob, jobsPage.getFirstRecommendedJobSaveBtnText());
    } else {
      assertEquals(saved, jobsPage.getFirstRecommendedJobSaveBtnText());
    }

    jobsPage.clickFirstRecommendedJobSaveBtn();
    if (eqSaveJob) {
      assertEquals(saved, jobsPage.getFirstRecommendedJobSaveBtnText());
    } else {
      assertEquals(saveJob, jobsPage.getFirstRecommendedJobSaveBtnText());
    }

    // refresh and repeat again but inverted
    jobsPage.refreshPage();
    assertEquals(firstRecJobName, jobsPage.getFirstRecommendedJobNameText());

    if (eqSaveJob) {
      assertEquals(saved, jobsPage.getFirstRecommendedJobSaveBtnText());
    } else {
      assertEquals(saveJob, jobsPage.getFirstRecommendedJobSaveBtnText());
    }

    jobsPage.clickFirstRecommendedJobSaveBtn();
    if (eqSaveJob) {
      assertEquals(saveJob, jobsPage.getFirstRecommendedJobSaveBtnText());
    } else {
      assertEquals(saved, jobsPage.getFirstRecommendedJobSaveBtnText());
    }

    selDriver.close();
  }
  
}
