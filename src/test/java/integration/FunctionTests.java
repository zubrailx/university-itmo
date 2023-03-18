package integration;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.junit.jupiter.api.Assertions.fail;

import java.io.IOException;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;

import csv.CSVFuncReader;
import func.Function;

/**
 * FunctionTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class FunctionTests {

  double epsilon = 0.001;

  final double TOP_BOUND = Math.pow(10, 20);
  final double LOWER_BOUND = -Math.pow(10, 20);

  int lnN = 1000;
  int trigN = 10;

  Function function;

  @BeforeAll
  public void init() {
    function = new Function(10, 1000);
  }

  /* Interval */
  @ParameterizedTest
  @Disabled
  @CsvFileSource(files = "src/test/data/unit/interval/func.Function.csv", numLinesToSkip = 1)
  public void Calc_IntervalAllMocks_EqualsDouble(double x, double y) {
    assertEquals(y, function.calc(x));
  }

  /* Critical */
  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/critical/NaN-func.Function.csv", numLinesToSkip = 1)
  public void Calc_CriticalAllMocks_NaNPoints(Double x, Double y) {
    Double res = function.calc(x);
    assertTrue(Double.isNaN(res) || res > TOP_BOUND || res < LOWER_BOUND);
  }

  @Test
  public void Calc_CriticalAllMocks_ExtremumPoints() {
    CSVFuncReader reader = new CSVFuncReader("src/test/data/unit/critical");
    try {
      var records = reader.getNumRFuncRecords(Function.class, "extremum");
      var recIter = records.iterator();
      while (recIter.hasNext()) {
        var left = recIter.next();
        var crit = recIter.next();
        var right = recIter.next();

        var lx = Double.parseDouble(left.get(0));
        var ly = Double.parseDouble(left.get(1));
        var cx = Double.parseDouble(crit.get(0));
        var cy = Double.parseDouble(crit.get(1));
        var rx = Double.parseDouble(right.get(0));
        var ry = Double.parseDouble(right.get(1));

        // Check in critical triple points
        assertEquals(ly, function.calc(lx), epsilon);
        assertEquals(cy, function.calc(cx), epsilon);
        assertEquals(ry, function.calc(rx), epsilon);
      }
    } catch (IOException ex) {
      fail();
    }
  }

  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/critical/other-func.Function.csv", numLinesToSkip = 1)
  public void Calc_CriticalAllMocks_OtherPoints(Double x, Double y) {
    assertEquals(y, function.calc(x));
  }

  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/critical/periodic-func.Function.csv", numLinesToSkip = 1)
  public void Calc_CriticalAllMocks_PeriodicPoints(Double x, Double y) {
    assertEquals(y, function.calc(x));
  }
}
