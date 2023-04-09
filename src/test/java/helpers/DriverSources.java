package helpers;

import java.util.stream.Stream;

import org.junit.jupiter.params.provider.Arguments;

import core.SeleniumChromeDriver;
import core.SeleniumFirefoxDriver;

/**
 * DriverSources
 */
public class DriverSources {

  static Stream<Arguments> provideDrivers() {
    return Stream.of(
      Arguments.of(new SeleniumChromeDriver()),
      Arguments.of(new SeleniumFirefoxDriver())
    );
  }
}
