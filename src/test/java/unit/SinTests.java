package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import func.trig.Sin;

/**
 * SinTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class SinTests {

  int taylorN = 10;
  double epsilon = 0.001;
  Sin sin;

  @BeforeAll
  public void init() {
    sin = new Sin(taylorN);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "2.541592653589793,0.5646424733950355",
      "15.107963267948966,0.5646424733950356",
      "30.81592653589793,-0.5646424733950376",
      "62.23185307179586,-0.5646424733950386",
      "3.3415926535897933,-0.19866933079506127",
      "15.907963267948965,-0.1986693307950599",
      "31.61592653589793,0.19866933079505933",
      "63.031853071795865,0.1986693307950616",
      "4.5415926535897935,-0.9854497299884603",
      "17.107963267948964,-0.9854497299884598",
      "32.81592653589793,0.9854497299884597",
      "64.23185307179587,0.9854497299884607",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    assertEquals(expected, sin.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    assertEquals(expected, sin.calc(x), epsilon);
  }

}
