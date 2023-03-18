package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.IOException;
import java.util.HashMap;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Disabled;
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

/**
 * FunctionTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class FunctionTests {

  double epsilon = 0.001;

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

    clsIntervalMocks = new HashMap<>();
    initIntervalMocks(clsIntervalMocks, clsModules);

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

  @ParameterizedTest
  @CsvSource(value = {
      "-6.283185307179586,NaN",
  })
  public void Calc_CriticalAllMocks_EqualsDouble(Double x, Double y) {
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
