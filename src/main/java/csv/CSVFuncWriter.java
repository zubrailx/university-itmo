package csv;

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVPrinter;

/**
 * CSVFuncWriter
 */
public class CSVFuncWriter {

  private final String testPath;

  public CSVFuncWriter(String testPath) {
    this.testPath = testPath;
  }

  public CSVPrinter getNumRFuncBasePrinter(Class<?> cls) throws IOException {
    return getCSVPrinterDepOnHeader(cls, CSVFuncConfig.getBaseHeaders());
  }

  public CSVPrinter getNumRFuncPrinter(Class<?> cls) throws IOException {
    return getCSVPrinterDepOnHeader(cls, CSVFuncConfig.getHeaders());
  }

  private CSVPrinter getCSVPrinterDepOnHeader(Class<?> cls, String[] header) throws IOException {
    Path filePath = CSVFuncConfig.getFilePath(testPath, cls);
    PrintWriter writer = new PrintWriter(filePath.toString(), StandardCharsets.UTF_8);
    final CSVFormat csvFormat = CSVFormat.DEFAULT.builder()
        .setHeader(header)
        .build();

    return new CSVPrinter(writer, csvFormat);
  }

}
