package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.IOException;
import java.util.HashMap;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;
import org.mockito.Mockito;

import csv.CSVFuncReader;
import func.NumRFunc;
import func.trig.Cos;
import func.trig.Sin;
import func.trig.Tan;

/**
 * TanTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class TanTests {

  double epsilon = 0.001;

  int trigN = 10;
  Class<?>[] numRFuncCls = { Cos.class, Sin.class, };

  // Modules
  HashMap<Class<?>, Object> clsModules;
  // Mocks
  HashMap<Class<?>, Object> clsCriticalMocks;

  @BeforeAll
  public void init() {
    clsModules = new HashMap<>();
    initModules(clsModules);

    clsCriticalMocks = new HashMap<>();
    initCriticalMocks(clsCriticalMocks, clsModules);
  }

  private void initModules(HashMap<Class<?>, Object> clsModules) {
    clsModules.put(Cos.class, new Cos(trigN));
    clsModules.put(Sin.class, new Sin(trigN));
  }

  private void initCriticalMocks(HashMap<Class<?>, Object> clsMocks, HashMap<Class<?>, Object> clsModules) {
    for (var key : clsModules.keySet()) {
      clsMocks.put(key, Mockito.mock(key));
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/mock/critical");
    // Read mockito data
    for (var cls : numRFuncCls) {
      try {
        var records = reader.getNumRFuncRecords(cls, null);
        for (var record : records) {
          Double x = Double.parseDouble(record.get(0));
          Double y = Double.parseDouble(record.get(1));
          Mockito.when(((NumRFunc) clsMocks.get(cls)).calc(x)).thenReturn(y);
        }

      } catch (IOException e) {
        System.err.println(e.getMessage());
      }
    }
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-0.4,-0.4227932187381618",
      "-0.2,-0.20271003550867248",
      "-0.1,-0.10033467208545054",
      "0.0,0.0",
      "0.1,0.10033467208545054",
      "0.2,0.20271003550867248",
      "0.4,0.4227932187381618",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Smooth_EqualsDouble(double x, double expected) {
    Tan tan = new Tan(
        (Cos) clsCriticalMocks.get(Cos.class),
        (Sin) clsCriticalMocks.get(Sin.class));
    assertEquals(expected, tan.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-0.6853981633974483,-0.8176288094325203",
      "-0.5853981633974483,-0.662911209645077",
      "-0.4853981633974483,-0.5274915061706243",
      "-0.38539816339744826,-0.40568564262187573",
      "-0.2853981633974483,-0.2934079930260234",
      "2.2561944901923447,-1.2230488804498656",
      "2.1561944901923447,-1.508497647121401",
      "2.056194490192345,-1.8957651228540087",
      "1.956194490192345,-2.464962756722604",
      "1.8561944901923448,-3.408223442335829",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Rapid_EqualsDouble(double x, double expected) {
    Tan tan = new Tan(
        (Cos) clsCriticalMocks.get(Cos.class),
        (Sin) clsCriticalMocks.get(Sin.class));
    assertEquals(expected, tan.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "3.741592653589793,0.6841368083416923",
      "3.3415926535897933,0.20271003550867253",
      "4.5415926535897935,5.797883715482898",
      "16.307963267948967,0.6841368083416934",
      "15.907963267948965,0.20271003550867112",
      "17.107963267948964,5.7978837154828184",
      "32.01592653589793,0.6841368083416925",
      "31.61592653589793,0.20271003550867048",
      "32.81592653589793,5.797883715482798",
      "63.431853071795864,0.6841368083416908",
      "63.031853071795865,0.2027100355086729",
      "64.23185307179587,5.797883715483001",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    Tan tan = new Tan(
        (Cos) clsCriticalMocks.get(Cos.class),
        (Sin) clsCriticalMocks.get(Sin.class));
    assertEquals(expected, tan.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    Tan tan = new Tan(
        (Cos) clsCriticalMocks.get(Cos.class),
        (Sin) clsCriticalMocks.get(Sin.class));
    assertEquals(expected, tan.calc(x), epsilon);
  }
}