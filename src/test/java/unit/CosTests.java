package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import func.trig.Cos;

/**
 * CosTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class CosTests {

  int taylorN = 10;
  double epsilon = 0.001;
  Cos cos;

  @BeforeAll
  public void init() {
    cos = new Cos(taylorN);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-0.4,0.9210609940028851",
      "-0.2,0.9800665778412416",
      "-0.1,0.9950041652780258",
      "0.0,1.0",
      "0.2,0.9800665778412416",
      "0.4,0.9210609940028851",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearZero_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-1.5707963267948966,6.123233995736766E-17",
      "-1.2,0.3623577544766736",
      "-1.0,0.5403023058681398",
      "-0.8,0.6967067093471654",
      "-0.6,0.8253356149096783",
      "-0.5,0.8775825618903728",
      "0.5,0.8775825618903728",
      "0.6,0.8253356149096783",
      "0.8,0.6967067093471654",
      "1.0,0.5403023058681398",
      "1.2,0.3623577544766736",
      "3.141592653589793,-1.0",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearPIDiv2_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "2.741592653589793,-0.9210609940028851",
      "2.941592653589793,-0.9800665778412416",
      "3.041592653589793,-0.9950041652780257",
      "3.141592653589793,-1.0",
      "3.3415926535897933,-0.9800665778412416",
      "3.541592653589793,-0.9210609940028852",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NegNearZero_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "1.5707963267948966,6.123233995736766E-17",
      "1.9415926535897932,-0.3623577544766735",
      "2.141592653589793,-0.5403023058681397",
      "2.3415926535897933,-0.6967067093471655",
      "2.541592653589793,-0.8253356149096782",
      "2.641592653589793,-0.8775825618903726",
      "3.641592653589793,-0.8775825618903728",
      "3.741592653589793,-0.8253356149096783",
      "3.941592653589793,-0.6967067093471656",
      "4.141592653589793,-0.5403023058681398",
      "4.341592653589793,-0.3623577544766735",
      "6.283185307179586,1.0",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NegNearPIDiv2_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "2.541592653589793,-0.8253356149096782",
      "15.107963267948966,-0.8253356149096781",
      "30.81592653589793,0.8253356149096768",
      "62.23185307179586,0.8253356149096761",
      "3.3415926535897933,-0.9800665778412416",
      "15.907963267948965,-0.9800665778412418",
      "31.61592653589793,0.9800665778412421",
      "63.031853071795865,0.9800665778412415",
      "4.5415926535897935,-0.1699671429002407",
      "17.107963267948964,-0.16996714290024295",
      "32.81592653589793,0.16996714290024353",
      "64.23185307179587,0.16996714290023776",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    assertEquals(expected, cos.calc(x), epsilon);
  }

}
