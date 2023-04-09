package tests;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import core.SeleniumDriver;
import pages.MainPage;

/**
 * MainPageTest
 */
public class MainPageTest {

  @ParameterizedTest
  @MethodSource("helpers.DriverSources#provideDrivers")
  public void first(SeleniumDriver driver) {
    driver.setup();

    MainPage main = new MainPage(driver.getDriver());

    System.out.println(main);
    driver.tear();
  }

}
