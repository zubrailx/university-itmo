package csv;

import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * CSVFuncConfig
 */
public class CSVFuncConfig {

  private static final String[] baseHeaders = { "base", "x", "y" };
  private static final String[] headers = { "x", "y" };

  static String[] getBaseHeaders() {
    return baseHeaders;
  }

  static String[] getHeaders() {
    return headers;
  }

  static Path getFilePath(String testPath, Class<?> cls) {
    return Paths.get(testPath, cls.getName() + ".csv");
  }

}
