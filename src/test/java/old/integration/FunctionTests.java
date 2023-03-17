package old.integration;

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
public class FunctionTests {

  int taylorN = 10;
  int taylorLogN = 1000;
  double epsilon = 0.001;
  Function function;

  @BeforeAll
  public void init() {
    HashMap<Class<?>, Object> clsMocks = new HashMap<>();

    // Create mockito objects
    var numrFuncClasses = new Class<?>[] { Ln.class, Cos.class, Cot.class, Csc.class, Sec.class, Sin.class, Tan.class };
    var numrFuncBaseClasses = new Class<?>[] { Log.class };

    // Put mockito objects
    for (var n : numrFuncClasses) {
      clsMocks.put(n, Mockito.mock(n));
    }
    for (var n : numrFuncBaseClasses) {
      clsMocks.put(n, Mockito.mock(n));
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/mock");

    // Read mockito data
    for (var cls : numrFuncClasses) {
      try {
        var records = reader.getNumRFuncRecords(cls);
        for (var record : records) {
          Double x = Double.parseDouble(record.get(0));
          Double y = Double.parseDouble(record.get(1));
          Mockito.when(((NumRFunc) clsMocks.get(cls)).calc(x)).thenReturn(y);
        }

      } catch (IOException e) {
        System.err.println(e.getMessage());
      }
    }

    for (var cls : numrFuncBaseClasses) {
      try {
        var records = reader.getNumRFuncBaseRecords(cls);
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

    // Construct function that uses mockito objects
    function = new Function(
        (Sin) clsMocks.get(Sin.class),
        (Tan) clsMocks.get(Tan.class),
        (Cot) clsMocks.get(Cot.class),
        (Csc) clsMocks.get(Csc.class),
        (Sec) clsMocks.get(Sec.class),
        (Cos) clsMocks.get(Cos.class),
        (Log) clsMocks.get(Log.class));
  }

  @ParameterizedTest
  @CsvFileSource(files = "src/test/data/unit/func.Function.csv", numLinesToSkip = 1)
  public void Calc_Interval_EqualsDouble(double x, double y) {
    assertEquals(y, function.calc(x));
  }
}
