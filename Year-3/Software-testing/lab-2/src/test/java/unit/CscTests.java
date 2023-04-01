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
import func.trig.Csc;
import func.trig.Sin;

/**
 * CscTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class CscTests {

  double epsilon = 0.001;

  int trigN = 10;
  Class<?>[] numRFuncCls = { Sin.class, };

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
      "1.1707963267948966,1.0857044283832387",
      "1.3707963267948966,1.0203388449411928",
      "1.4707963267948965,1.0050209184004555",
      "1.5707963267948966,1.0",
      "1.6707963267948966,1.0050209184004555",
      "1.7707963267948965,1.0203388449411928",
      "1.9707963267948965,1.0857044283832387",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Smooth_EqualsDouble(double x, double expected) {
    Csc csc = new Csc(
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, csc.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "0.1,10.016686131634776",
      "0.2,5.033489547672344",
      "0.3,3.383863361824123",
      "0.4,2.567932455547783",
      "0.5,2.085829642933488",
      "3.041592653589793,10.016686131634755",
      "2.941592653589793,5.033489547672336",
      "2.8415926535897933,3.383863361824123",
      "2.741592653589793,2.567932455547783",
      "2.641592653589793,2.0858296429334877",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Rapid_EqualsDouble(double x, double expected) {
    Csc csc = new Csc(
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, csc.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "3.741592653589793,-1.7710321966877254",
      "3.3415926535897933,-5.0334895476723425",
      "4.5415926535897935,-1.0147651062948793",
      "16.307963267948967,-1.7710321966877232",
      "15.907963267948965,-5.033489547672377",
      "17.107963267948964,-1.0147651062948797",
      "32.01592653589793,1.771032196687725",
      "31.61592653589793,5.033489547672392",
      "32.81592653589793,1.01476510629488",
      "63.431853071795864,1.771032196687728",
      "63.031853071795865,5.0334895476723345",
      "64.23185307179587,1.0147651062948788",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Periodic_EqualsDouble(double x, double expected) {
    Csc csc = new Csc(
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, csc.calc(x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "NaN,NaN",
      "-Infinity,NaN",
      "Infinity,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double x, double expected) {
    Csc csc = new Csc(
      (Sin) clsCriticalMocks.get(Sin.class)
    );
    assertEquals(expected, csc.calc(x), epsilon);
  }

}
