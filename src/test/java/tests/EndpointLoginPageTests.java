package tests;

import core.SeleniumDriver;
import pages.LoginPage;
import pages.MainPage;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

public class EndpointLoginPageTests {

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void isRememberSel_EqualsTrue(SeleniumDriver selDriver) {
    selDriver.setup();

    LoginPage loginPage = new LoginPage(selDriver.getDriver(), false)
            .acceptCookiesIfClickable();

    assertTrue(loginPage.isRememberSel());

    selDriver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickRemember_After_EqualsFalse(SeleniumDriver selDriver) {
    selDriver.setup();

    LoginPage loginPage = new LoginPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .clickRemember();

    assertFalse(loginPage.isRememberSel());

    selDriver.tear();
  }

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void clickRemember_After_EqualsFalse_FromMainPage(SeleniumDriver selDriver) {
    selDriver.setup();

    LoginPage loginPage = new MainPage(selDriver.getDriver(), false)
      .acceptCookiesIfClickable()
      .clickLoginBtn()
      .clickRemember();

    assertFalse(loginPage.isRememberSel());

    selDriver.tear();
  }
}
