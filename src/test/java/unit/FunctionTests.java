package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;

import func.Function;

/**
 * FunctionTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class FunctionTests {

  int taylorTrigN = 10;
  int taylorLogN = 1000;
  double epsilon = 0.001;
  Function function;

  @BeforeAll
  public void init() {
    function = new Function(taylorTrigN, taylorLogN);
  }
  
  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/func.Function.csv", numLinesToSkip = 1)
  public void Calc_Interval_EqualsDouble(double x, double y) {
    assertEquals(y, function.calc(x));
  }
}
