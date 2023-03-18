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
import func.log.Ln;
import func.log.Log;

/**
 * LogTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class LogTests {

  double epsilon = 0.001;

  int lnN = 1000;
  Class<?>[] numRFuncCls = { Ln.class, };

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
    clsModules.put(Ln.class, new Ln(lnN));
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
      "NaN,0.5,NaN",
      "NaN,0.8,NaN",
      "NaN,1.1,NaN",
      "NaN,1.4,NaN",
      "NaN,1.7,NaN",
      "NaN,2.0,NaN",
      "-Infinity,0.5,NaN",
      "-Infinity,0.8,NaN",
      "-Infinity,1.1,NaN",
      "-Infinity,1.4,NaN",
      "-Infinity,1.7,NaN",
      "-Infinity,2.0,NaN",
      "Infinity,0.5,-0.0",
      "Infinity,0.8,-0.0",
      "Infinity,1.1,0.0",
      "Infinity,1.4,0.0",
      "Infinity,1.7,0.0",
      "Infinity,2.0,0.0",
      "-1.0,0.5,NaN",
      "-1.0,0.8,NaN",
      "-1.0,1.1,NaN",
      "-1.0,1.4,NaN",
      "-1.0,1.7,NaN",
      "-1.0,2.0,NaN",
      "0.0,0.5,0.0",
      "0.0,0.8,0.0",
      "0.0,1.1,-0.0",
      "0.0,1.4,-0.0",
      "0.0,1.7,-0.0",
      "0.0,2.0,-0.0",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_BaseBounds_EqualsDouble(double base, double x, double expected) {
    Log log = new Log(
        (Ln) clsCriticalMocks.get(Ln.class));
    assertEquals(expected, log.calc(base, x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "10,NaN,NaN",
      "10,-Infinity,NaN",
      "10,Infinity,Infinity",
      "10,0.0,-Infinity",
      "10,-0.1,NaN",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_Bounds_EqualsDouble(double base, double x, double expected) {
    Log log = new Log(
        (Ln) clsCriticalMocks.get(Ln.class));
    assertEquals(expected, log.calc(base, x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "10,0.1,-0.9999999999999998",
      "10,0.2,-0.6989700043360187",
      "10,0.3,-0.5228787452803376",
      "10,0.4,-0.39794000867203755",
      "10,0.5,-0.30102999566398114",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearZero_EqualsDouble(double base, double x, double expected) {
    Log log = new Log(
        (Ln) clsCriticalMocks.get(Ln.class));
    assertEquals(expected, log.calc(base, x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "10,0.5,-0.30102999566398114",
      "10,0.8,-0.09691001300805638",
      "10,1.1,0.04139268515822507",
      "10,1.4,0.14612803567823798",
      "10,1.7,0.23044892137827389",
      "10,2.0,0.30102999566398114",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearTransition_EqualsDouble(double base, double x, double expected) {
    Log log = new Log(
        (Ln) clsCriticalMocks.get(Ln.class));
    assertEquals(expected, log.calc(base, x), epsilon);
  }

  @ParameterizedTest
  @CsvSource(value = {
      "10,2.0,0.30102999566398114",
      "10,5.0,0.6989700043360187",
      "10,10.0,1.0",
      "10,100.0,2.0",
  }, ignoreLeadingAndTrailingWhitespace = true)
  void Calc_NearSmooth_EqualsDouble(double base, double x, double expected) {
    Log log = new Log(
        (Ln) clsCriticalMocks.get(Ln.class));
    assertEquals(expected, log.calc(base, x), epsilon);
  }

}
