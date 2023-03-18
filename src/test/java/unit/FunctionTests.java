package unit;

import java.io.IOException;
import java.util.HashMap;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;
import org.junit.jupiter.params.provider.CsvSource;
import org.mockito.Mockito;

import csv.CSVFuncReader;
import func.Function;
import func.NumRFunc;
import func.NumRFuncBase;
import func.log.Ln;
import func.log.Log;
import func.trig.Cos;
import func.trig.Cot;
import func.trig.Csc;
import func.trig.Sec;
import func.trig.Sin;
import func.trig.Tan;

import static org.junit.jupiter.api.Assertions.*;

/**
 * FunctionTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class FunctionTests {

  double epsilon = 0.001;

  final double TOP_BOUND = Math.pow(10, 20);
  final double LOWER_BOUND = - Math.pow(10, 20);

  int lnN = 10;
  int trigN = 1000;

  Class<?>[] numRFuncCls = { Ln.class, Cos.class, Cot.class, Csc.class, Sec.class, Sin.class, Tan.class };
  Class<?>[] numRFuncBaseCls = { Log.class };

  // Modules
  HashMap<Class<?>, Object> clsModules;
  // Mocks
  HashMap<Class<?>, Object> clsIntervalMocks;
  HashMap<Class<?>, Object> clsCriticalMocks;

  @BeforeAll
  public void init() {
    clsModules = new HashMap<>();
    initModules(clsModules);

//    clsIntervalMocks = new HashMap<>();
//    initIntervalMocks(clsIntervalMocks, clsModules);

    clsCriticalMocks = new HashMap<>();
    initCriticalMocks(clsCriticalMocks, clsModules);
  }

  private void initModules(HashMap<Class<?>, Object> clsModules) {
    clsModules.put(Ln.class, new Ln(lnN));
    clsModules.put(Log.class, new Log(lnN));
    clsModules.put(Cos.class, new Cos(trigN));
    clsModules.put(Cot.class, new Cot(trigN));
    clsModules.put(Csc.class, new Csc(trigN));
    clsModules.put(Sec.class, new Sec(trigN));
    clsModules.put(Sin.class, new Sin(trigN));
    clsModules.put(Tan.class, new Tan(trigN));
  }

  private void initIntervalMocks(HashMap<Class<?>, Object> clsMocks, HashMap<Class<?>, Object> clsModules) {
    for (var key : clsModules.keySet()) {
      clsMocks.put(key, Mockito.mock(key));
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/mock/interval");

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

    for (var cls : numRFuncBaseCls) {
      try {
        var records = reader.getNumRFuncBaseRecords(cls, null);
        for (var record : records) {
          Double base = Double.parseDouble(record.get(0));
          Double x = Double.parseDouble(record.get(1));
          Double y = Double.parseDouble(record.get(2));
          Mockito.when(((NumRFuncBase) clsMocks.get(cls)).calc(base, x)).thenReturn(y);
        }

      } catch (IOException e) {
        System.err.println(e.getMessage());
      }
    }
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

    for (var cls : numRFuncBaseCls) {
      try {
        var records = reader.getNumRFuncBaseRecords(cls, null);
        for (var record : records) {
          Double base = Double.parseDouble(record.get(0));
          Double x = Double.parseDouble(record.get(1));
          Double y = Double.parseDouble(record.get(2));
          Mockito.when(((NumRFuncBase) clsMocks.get(cls)).calc(base, x)).thenReturn(y);
        }

      } catch (IOException e) {
        System.err.println(e.getMessage());
      }
    }
  }

  /* Interval */
  @ParameterizedTest
  @Disabled
  @CsvFileSource(files = "src/test/data/unit/interval/func.Function.csv", numLinesToSkip = 1)
  public void Calc_IntervalAllMocks_EqualsDouble(double x, double y) {
    Function function = new Function(
        (Sin) clsIntervalMocks.get(Sin.class),
        (Tan) clsIntervalMocks.get(Tan.class),
        (Cot) clsIntervalMocks.get(Cot.class),
        (Csc) clsIntervalMocks.get(Csc.class),
        (Sec) clsIntervalMocks.get(Sec.class),
        (Cos) clsIntervalMocks.get(Cos.class),
        (Log) clsIntervalMocks.get(Log.class));

    assertEquals(y, function.calc(x));
  }

  /* Critical */
  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/critical/NaN-func.Function.csv", numLinesToSkip = 1)
  public void Calc_CriticalAllMocks_NaNPoints(Double x, Double y) {
    Function function = new Function(
        (Sin) clsCriticalMocks.get(Sin.class),
        (Tan) clsCriticalMocks.get(Tan.class),
        (Cot) clsCriticalMocks.get(Cot.class),
        (Csc) clsCriticalMocks.get(Csc.class),
        (Sec) clsCriticalMocks.get(Sec.class),
        (Cos) clsCriticalMocks.get(Cos.class),
        (Log) clsCriticalMocks.get(Log.class));

    Double res = function.calc(x);
    assertTrue(Double.isNaN(res) || res > TOP_BOUND || res < LOWER_BOUND);
  }

  @Test
  public void Calc_CriticalAllMocks_ExtremumPoints() {
    Function function = new Function(
        (Sin) clsCriticalMocks.get(Sin.class),
        (Tan) clsCriticalMocks.get(Tan.class),
        (Cot) clsCriticalMocks.get(Cot.class),
        (Csc) clsCriticalMocks.get(Csc.class),
        (Sec) clsCriticalMocks.get(Sec.class),
        (Cos) clsCriticalMocks.get(Cos.class),
        (Log) clsCriticalMocks.get(Log.class));

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
    Function function = new Function(
        (Sin) clsCriticalMocks.get(Sin.class),
        (Tan) clsCriticalMocks.get(Tan.class),
        (Cot) clsCriticalMocks.get(Cot.class),
        (Csc) clsCriticalMocks.get(Csc.class),
        (Sec) clsCriticalMocks.get(Sec.class),
        (Cos) clsCriticalMocks.get(Cos.class),
        (Log) clsCriticalMocks.get(Log.class));

    assertEquals(y, function.calc(x));
  }

  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/critical/periodic-func.Function.csv", numLinesToSkip = 1)
  public void Calc_CriticalAllMocks_PeriodicPoints(Double x, Double y) {
    Function function = new Function(
        (Sin) clsCriticalMocks.get(Sin.class),
        (Tan) clsCriticalMocks.get(Tan.class),
        (Cot) clsCriticalMocks.get(Cot.class),
        (Csc) clsCriticalMocks.get(Csc.class),
        (Sec) clsCriticalMocks.get(Sec.class),
        (Cos) clsCriticalMocks.get(Cos.class),
        (Log) clsCriticalMocks.get(Log.class));

    assertEquals(y, function.calc(x));
  }
}
