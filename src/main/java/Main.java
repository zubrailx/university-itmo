import func.log.Log;
import func.trig.Cos;
import func.trig.Sin;

import java.io.IOException;

import csv.CSVFuncReader;
import csv.CSVFuncWriter;

public class Main {

  public static void main(String[] argv) {
    // System.out.println(new Cos(10).calc(Math.PI));
    var writer = new CSVFuncWriter("src/test/data/out");
    try (var printer = writer.getNumRFuncBasePrinter(Log.class)) {
      printer.printRecord(10, 5, Double.NaN);
      printer.printRecord(10, 5, 12);
      printer.printRecord(10, 5, 12);
      printer.printRecord(10, 5, 12);
      printer.printRecord(10, 5, 12);
      printer.printRecord(10, 5, 12);
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }

    CSVFuncReader reader = new CSVFuncReader("src/test/data/out");
    try {
      var records = reader.getNumRFuncBaseRecords(Log.class);
      for (var record : records) {
        System.out.println(Double.parseDouble(record.get("y")));
      }
    } catch (IOException e) {
      System.err.println(e.getMessage());
    }
  }
}
