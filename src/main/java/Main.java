import func.Function;
import func.log.Ln;
import func.log.Log;
import func.trig.Cos;
import func.trig.Cot;
import func.trig.Sin;

import java.io.IOException;

import csv.CSVFuncReader;
import csv.CSVFuncWriter;

public class Main {

  public static Double trigFunc(Double x) {
    return Math.sin(x) / Math.cos(x);
  }

  public static Double logFunc(Double x, double base) {
    return Math.log(x) / Math.log(base);
  }

  public static void main(String[] argv) {
    // Cot cot = new Cot(10);
    // System.out.println(cot.calc(0.));
    // trigGenTests();
    // lnTestGen();
    // logTestGen();
    funcTestGen();
  }

  public static void trigGenTests() {
    var writer = new CSVFuncWriter("src/test/data/unit");

    double[] bounds = new double[] { Double.NaN, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY };
    double[] nearZero = new double[] { -0.4, -0.2, -0.1, 0, 0.1, 0.2, 0.4 };
    double[] nearTransition = new double[] { 0.1, 0.2, 0.3, 0.4, 0.5, Math.PI - 0.1, Math.PI - 0.2, Math.PI - 0.3,
        Math.PI - 0.4, Math.PI - 0.5 };

    double[] periodicValues = new double[] { 0.6, 0.2, 1.4 };
    double[] periodic = new double[] { Math.PI, 5 * Math.PI, 10 * Math.PI, 20 * Math.PI };

    try (var printer = writer.getNumRFuncPrinter(Cot.class)) {
      // bounds
      for (var value : bounds) {
        printer.printRecord(value, trigFunc(value));
      }
      printer.printRecord(0, 0);
      // nearZero
      for (var value : nearZero) {
        printer.printRecord(value, trigFunc(value));
      }
      printer.printRecord(0, 0);
      // highRaise
      // for (var value : nearZero) {
      // printer.printRecord(value, func(value));
      // }
      // printer.printRecord(0, 0);
      // nearTransition
      for (var value : nearTransition) {
        printer.printRecord(value - Math.PI / 4, trigFunc(value - Math.PI / 4));
      }
      printer.printRecord(0, 0);
      // periodic
      for (var period : periodic) {
        for (var value : periodicValues) {
          printer.printRecord(value + period, trigFunc(value + period));
        }
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/unit");
    try {
      var records = reader.getNumRFuncRecords(Cot.class);
      for (var record : records) {
        System.out.println(record);
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }

  public static void lnTestGen() {
    var writer = new CSVFuncWriter("src/test/data/unit");

    double[] bounds = new double[] { Double.NaN, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY, 0, -0.1 };
    double[] nearZero = new double[] { 0.1, 0.2, 0.3, 0.4, 0.5 };
    double[] nearTransition = new double[] { 0.5, 0.8, 1.1, 1.4, 1.7, 2.0 };
    double[] nearSmooth = new double[] { 2, 5, 10, 100 };

    try (var printer = writer.getNumRFuncPrinter(Ln.class)) {
      // bounds
      for (var value : bounds) {
        printer.printRecord(value, logFunc(value, Math.E));
      }
      printer.printRecord(0, 0);
      // nearZero
      for (var value : nearZero) {
        printer.printRecord(value, logFunc(value, Math.E));
      }
      printer.printRecord(0, 0);
      // nearTransition
      for (var value : nearTransition) {
        printer.printRecord(value, logFunc(value, Math.E));
      }
      printer.printRecord(0, 0);
      // nearSmooth
      for (var value : nearSmooth) {
        printer.printRecord(value, logFunc(value, Math.E));
      }
      printer.printRecord(0, 0);
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/unit");
    try {
      var records = reader.getNumRFuncRecords(Ln.class);
      for (var record : records) {
        System.out.println(record);
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }

  public static void logTestGen() {
    var writer = new CSVFuncWriter("src/test/data/unit");

    double[] bounds = new double[] { Double.NaN, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY, 0, -0.1 };
    double[] nearZero = new double[] { 0.1, 0.2, 0.3, 0.4, 0.5 };
    double[] nearTransition = new double[] { 0.5, 0.8, 1.1, 1.4, 1.7, 2.0 };
    double[] nearSmooth = new double[] { 2, 5, 10, 100 };

    double[] bases = new double[] { Double.NaN, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY, -1, 0 };

    try (var printer = writer.getNumRFuncBasePrinter(Log.class)) {
      // testBases
      for (var base : bases) {
        for (var value : nearTransition) {
          printer.printRecord(base, value, logFunc(value, base));
        }
      }
      printer.printRecord(0, 0, 0);
      // bounds
      for (var value : bounds) {
        printer.printRecord(10, value, logFunc(value, 10));
      }
      printer.printRecord(0, 0, 0);
      // nearZero
      for (var value : nearZero) {
        printer.printRecord(10, value, logFunc(value, 10));
      }
      printer.printRecord(0, 0, 0);
      // nearTransition
      for (var value : nearTransition) {
        printer.printRecord(10, value, logFunc(value, 10));
      }
      printer.printRecord(0, 0, 0);
      // nearSmooth
      for (var value : nearSmooth) {
        printer.printRecord(10, value, logFunc(value, 10));
      }
      printer.printRecord(0, 0, 0);
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/unit");
    try {
      var records = reader.getNumRFuncBaseRecords(Log.class);
      for (var record : records) {
        System.out.println(record);
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }

  public static void funcTestGen() {
    var func = new Function(10, 1000);
    var writer = new CSVFuncWriter("src/test/data/unit");

    double left = -4;
    double right = 4;
    double interval = 0.04;

    try (var printer = writer.getNumRFuncPrinter(Function.class)) {
      for (double i = left; i < right + interval; i += interval) {
        printer.printRecord(i, func.calc(i));
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/unit");
    try {
      var records = reader.getNumRFuncRecords(Function.class);
      for (var record : records) {
        System.out.println(record);
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }
}
