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
import func.trig.Cot;
import func.trig.Sin;

/**
 * CotTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class CotTests {

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
      "-1.9707963267948965,0.4227932187381616",
      "-1.7707963267948965,0.20271003550867236",
      "-1.6707963267948966,0.10033467208545058",
      "-1.5707963267948966,-6.123233995736766E-17",
      "-1.4707963267948965,-0.1003346720854507",
      "-1.3707963267948966,-0.2027100355086725",
      "-1.1707963267948966,-0.4227932187381617",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Smooth_EqualsDouble(double x, double expected) {
    Cot cot = new Cot(
      (Cos) clsCriticalMocks.get(Cos.class),
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, cot.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "-0.4,-2.3652224200391103",
      "-0.2,-4.933154875586894",
      "-0.1,-9.966644423259238",
      "0.1,9.966644423259238",
      "0.2,4.933154875586894",
      "0.4,2.3652224200391103",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Rapid_EqualsDouble(double x, double expected) {
    Cot cot = new Cot(
      (Cos) clsCriticalMocks.get(Cos.class),
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, cot.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "3.741592653589793,1.4616959470781021",
      "3.3415926535897933,4.933154875586892",
      "4.5415926535897935,0.1724767258317997",
      "16.307963267948967,1.4616959470780997",
      "15.907963267948965,4.9331548755869274",
      "17.107963267948964,0.17247672583180207",
      "32.01592653589793,1.4616959470781017",
      "31.61592653589793,4.9331548755869425",
      "32.81592653589793,0.17247672583180268",
      "63.431853071795864,1.4616959470781055",
      "63.031853071795865,4.933154875586883",
      "64.23185307179587,0.17247672583179663",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    Cot cot = new Cot(
      (Cos) clsCriticalMocks.get(Cos.class),
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, cot.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    Cot cot = new Cot(
      (Cos) clsCriticalMocks.get(Cos.class),
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, cot.calc(x), epsilon);
  }

}