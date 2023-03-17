package unit;

import static org.junit.jupiter.api.Assertions.assertEquals;

import java.io.IOException;
import java.util.HashMap;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvFileSource;
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
public class FunctionIntervalTests {

  double epsilon = 0.001;

  int lnN = 10;
  int trigN = 1000;
  Class<?>[] numRFuncCls = { Ln.class, Cos.class, Cot.class, Csc.class, Sec.class, Sin.class, Tan.class };
  Class<?>[] numRFuncBaseCls = { Log.class };
  HashMap<Class<?>, Object> clsMocks;
  HashMap<Class<?>, Object> clsModules; // inited modules

  private void initModules() {
    clsModules.put(Ln.class, new Ln(lnN));
    clsModules.put(Log.class, new Log(lnN));
    clsModules.put(Cos.class, new Cos(trigN));
    clsModules.put(Cot.class, new Cot(trigN));
    clsModules.put(Csc.class, new Csc(trigN));
    clsModules.put(Sec.class, new Sec(trigN));
    clsModules.put(Sin.class, new Sin(trigN));
    clsModules.put(Tan.class, new Tan(trigN));
  }

  private void initMocks(HashMap<Class<?>, Object> clsModules) {
    for (var key : clsModules.keySet()) {
      clsMocks.put(key.getClass(), Mockito.mock(key.getClass()));
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/mock");

    // Read mockito data
    for (var cls : numRFuncCls) {
      try {
        var records = reader.getNumRFuncRecords(cls, "interval");
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
        var records = reader.getNumRFuncBaseRecords(cls, "interval");
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

  @BeforeAll
  public void init() {
    clsModules = new HashMap<>();
    initModules();

    clsMocks = new HashMap<>();
    initMocks(clsModules);

  }

  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/func.Function.csv", numLinesToSkip = 1)
  public void Calc_IntervalAllMocks_EqualsDouble(double x, double y) {
    Function function = new Function(
        (Sin) clsMocks.get(Sin.class),
        (Tan) clsMocks.get(Tan.class),
        (Cot) clsMocks.get(Cot.class),
        (Csc) clsMocks.get(Csc.class),
        (Sec) clsMocks.get(Sec.class),
        (Cos) clsMocks.get(Cos.class),
        (Log) clsMocks.get(Log.class));
    assertEquals(y, function.calc(x));
  }
}
