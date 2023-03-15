package unit;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

import func.Function;

/**
 * FunctionTests
 */
@TestInstance(Lifecycle.PER_CLASS)
public class FunctionTests {

  int taylorN = 10;
  double epsilon = 0.001;
  Function function;

  @BeforeAll
  public void init() {
    function = new Function(taylorN);
  }
  
}
