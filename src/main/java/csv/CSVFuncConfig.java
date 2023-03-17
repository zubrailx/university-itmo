package csv;

import java.nio.file.Path;
import java.nio.file.Paths;

/**
 * CSVFuncConfig
 */
public class CSVFuncConfig {

  private static final String[] baseHeaders = { "base", "x", "y" };
  private static final String[] headers = { "x", "y" };

  public static String[] getBaseHeaders() {
    return baseHeaders;
  }

  public static String[] getHeaders() {
    return headers;
  }

  static Path getFilePath(String testPath, Class<?> cls, String suffix) {
    if (suffix == null) {
      return Paths.get(testPath, cls.getName() + ".csv");
    } else {
      return Paths.get(testPath, cls.getName() + "." + suffix + ".csv");
    }
  }

}
