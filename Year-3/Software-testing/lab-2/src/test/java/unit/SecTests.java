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
import func.trig.Sec;

/**
 * SecTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class SecTests {

  int trigN = 10;
  double epsilon = 0.001;

  Class<?>[] numRFuncCls = { Cos.class };

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
      "-0.4,1.0857044283832387",
      "-0.2,1.0203388449411928",
      "-0.1,1.0050209184004553",
      "0.0,1.0",
      "0.1,1.0050209184004553",
      "0.2,1.0203388449411928",
      "0.4,1.0857044283832387",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Smooth_EqualsDouble(double x, double expected) {
    Sec sec = new Sec(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sec.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-1.4707963267948965,10.016686131634762",
      "-1.3707963267948966,5.033489547672343",
      "-1.2707963267948965,3.3838633618241216",
      "-1.1707963267948966,2.5679324555477834",
      "-1.0707963267948966,2.0858296429334877",
      "1.4707963267948965,10.016686131634762",
      "1.3707963267948964,5.033489547672339",
      "1.2707963267948967,3.3838633618241234",
      "1.1707963267948966,2.5679324555477834",
      "1.0707963267948966,2.0858296429334877",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Rapid_EqualsDouble(double x, double expected) {
    Sec sec = new Sec(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sec.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "3.741592653589793,-1.2116283145123166",
      "3.3415926535897933,-1.0203388449411928",
      "4.5415926535897935,-5.883490084827353",
      "16.307963267948967,-1.2116283145123172",
      "15.907963267948965,-1.0203388449411925",
      "17.107963267948964,-5.883490084827275",
      "32.01592653589793,1.2116283145123168",
      "31.61592653589793,1.0203388449411923",
      "32.81592653589793,5.883490084827255",
      "63.431853071795864,1.211628314512316",
      "63.031853071795865,1.0203388449411928",
      "64.23185307179587,5.8834900848274545",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    Sec sec = new Sec(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sec.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    Sec sec = new Sec(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sec.calc(x), epsilon);
  }

}
