package old.unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import func.log.Ln;

/**
 * LnTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class LnTests {

  int taylorN = 1000;
  double epsilon = 0.01;
  Ln ln;

  @BeforeAll
  public void init() {
    ln = new Ln(taylorN);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,Infinity",
      "0.0,-Infinity",
      "-0.1,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    assertEquals(expected, ln.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "0.1,-2.3025850929940455",
      "0.2,-1.6094379124341003",
      "0.3,-1.2039728043259361",
      "0.4,-0.916290731874155",
      "0.5,-0.6931471805599453",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearZero_EqualsDouble(double x, double expected) {
    assertEquals(expected, ln.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "0.5,-0.6931471805599453",
      "0.8,-0.2231435513142097",
      "1.1,0.09531017980432493",
      "1.4,0.3364722366212129",
      "1.7,0.5306282510621704",
      "2.0,0.6931471805599453",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearTransition_EqualsDouble(double x, double expected) {
    assertEquals(expected, ln.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "2.0,0.6931471805599453",
      "5.0,1.6094379124341003",
      "10.0,2.302585092994046",
      "100.0,4.605170185988092",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearSmooth_EqualsDouble(double x, double expected) {
    assertEquals(expected, ln.calc(x), epsilon);
  }

}
