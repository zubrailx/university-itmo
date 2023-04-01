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

/**
 * SinTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class SinTests {

  double epsilon = 0.001;

  int trigN = 10;
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
    Sin sin = new Sin(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sin.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    Sin sin = new Sin(
        (Cos) clsCriticalMocks.get(Cos.class));
    assertEquals(expected, sin.calc(x), epsilon);
  }

}
