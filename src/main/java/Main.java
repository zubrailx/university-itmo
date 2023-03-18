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

import java.io.IOException;

import csv.CSVFuncReader;
import csv.CSVFuncWriter;

public class Main {

  static final double leftBound = -6.29; // slightly above 2PI
  static final double rightBound = 4;
  static final double interval = 0.01;

  public static Double trigFunc(Double x) {
    return Math.sin(x) / Math.cos(x);
  }

  public static Double logFunc(Double x, double base) {
    return Math.log(x) / Math.log(base);
  }

  public static void main(String[] argv) {
    // getCriticalPoints();
    getOtherCriticalPoints();
    // getCriticalLogPoints();
    // funcGraphGen();
    // mockDataGenWithInterval();
    // trigTestGen();
    // lnTestGen();
    // logTestGen();
  }

  public static void getCriticalPoints() {
    var writer = new CSVFuncWriter("src/test/data/mock/critical");

    var xNaNs = new Double[] {
        -2 * Math.PI,
        -1.5 * Math.PI,
        -1 * Math.PI,
        -0.5 * Math.PI,
        -0 * Math.PI,
    };

    var xExtrs = new Double[] {
        -5.501,
        -5.5,
        -5.499,
        -3.896,
        -3.895,
        -3.894,
        -2.665,
        -2.664,
        -2.663,
        -0.362,
        -0.361,
        -0.360
    };

    var xOthers = new Double[] {
        0.2,
        0.5,
        1.,
        3.,
        10.,
        40.
    };

    var xPers = new Double[] {
        -5.8,
        -5.94,
        -5.,
        -4.9,
        -4.5,
        -4.2,
        -3.5,
        -2.9,
        -2.1,
        -1.9,
        -1.3,
        -1.15,
        -0.6,
        -0.2,
        -0.150
    };

    var periods = new Double[] {
        0 * Math.PI,
        -2 * Math.PI,
        -10 * Math.PI,
        -100 * Math.PI
    };

    Function func = new Function(10, 1000);

    try (var printer = writer.getNumRFuncPrinter(Function.class, "add")) {
      // for (var x : xNaNs) {
      // printer.printRecord(x, func.calc(x)); // can be NaN
      // }

      // for (var x : xExtrs) {
      // printer.printRecord(x, func.calc(x));
      // }

      for (var x : xOthers) {
        printer.printRecord(x, func.calc(x));
      }

      // for (var period : periods) {
      // for (var x : xPers) {
      // printer.printRecord(x + period, func.calc(x + period));
      // }
      // }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "extremum")) {
    // for (var x : xExtrs) {
    // printer.printRecord(x, func.calc(x));
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }

    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "other")) {
    // for (var x : xOthers) {
    // printer.printRecord(x, func.calc(x));
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }

    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "periodic")) {
    // for (var period : periods) {
    // for (var x : xPers) {
    // printer.printRecord(x + period, func.calc(x + period));
    // }
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }
  }

  public static void getCriticalLogPoints() {
    var writer = new CSVFuncWriter("src/test/data/mock/critical");

    var xNaNs = new Double[] {
        -1000.,
        -1.,
        0.,
        0.2,
        0.5,
        1.,
        3.,
        10.,
        40.
    };

    var bases = new Double[] {
        2.,
        3.,
        5.,
        10.,
    };

    Ln func = new Ln(1000);

    try (var printer = writer.getNumRFuncPrinter(Ln.class, "additional")) {
      // for (var base: bases) {
      for (var x : xNaNs) {
        printer.printRecord(x, func.calc(x));
      }
      // }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "extremum")) {
    // for (var x : xExtrs) {
    // printer.printRecord(x, func.calc(x));
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }

    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "other")) {
    // for (var x : xOthers) {
    // printer.printRecord(x, func.calc(x));
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }

    // try (var printer = writer.getNumRFuncPrinter(Sin.class, "periodic")) {
    // for (var period : periods) {
    // for (var x : xPers) {
    // printer.printRecord(x + period, func.calc(x + period));
    // }
    // }
    // } catch (IOException e) {
    // System.err.println(e.getMessage());
    // }
  }

  public static void getOtherCriticalPoints() {

    var objects = new NumRFunc[] {
        new Cos(10),
        new Sin(10)
    };

    var xNaNs = new Double[] {
        -0.38539816339744826
    };

    var writer = new CSVFuncWriter("src/test/data/mock/additional");

    for (var function : objects) {
      try (var printer = writer.getNumRFuncPrinter(function.getClass(), null)) {
        for (var x : xNaNs) {
          printer.printRecord(x, function.calc(x));
        }
      } catch (IOException e) {
        System.err.println(e.getMessage());
      }
    }

  }

  /*
   * Generate data for graph
   */
  public static void funcGraphGen() {
    var func = new Function(10, 1000);
    var writer = new CSVFuncWriter("src/test/data/graph");

    try (var printer = writer.getNumRFuncPrinter(Function.class, null)) {
      for (double i = leftBound; i < rightBound + interval; i += interval) {
        printer.printRecord(i, func.calc(i));
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/graph");
    try {
      var records = reader.getNumRFuncRecords(Function.class, "interval");
      for (var record : records) {
        // System.out.println(record);
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }
}
