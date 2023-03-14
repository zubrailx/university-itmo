package csv;

import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.file.Path;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVRecord;

/**
 * CSVFuncReader
 */
public class CSVFuncReader {

  private final String testPath;

  public CSVFuncReader(String testPath) {
    this.testPath = testPath;
  }

  public Iterable<CSVRecord> getNumRFuncBaseRecords(Class<?> cls) throws IOException {
    return getCSVReaderDepOnHeader(cls, CSVFuncConfig.getBaseHeaders());
  }

  public Iterable<CSVRecord> getNumRFuncRecords(Class<?> cls) throws IOException {
    return getCSVReaderDepOnHeader(cls, CSVFuncConfig.getHeaders());
  }

  private Iterable<CSVRecord> getCSVReaderDepOnHeader(Class<?> cls, String[] header) throws IOException {
    Path filePath = CSVFuncConfig.getFilePath(testPath, cls);
    Reader reader = new FileReader(filePath.toString());

    final CSVFormat csvFormat = CSVFormat.DEFAULT.builder()
        .setHeader(header)
        .setSkipHeaderRecord(true)
        .build();

    return csvFormat.parse(reader);
  }
}
