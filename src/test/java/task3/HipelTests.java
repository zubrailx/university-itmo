package task3;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

@TestInstance(Lifecycle.PER_CLASS)
public class HipelTests {

  @Test
  public void Constructor_NoExcept() {
    Hipel hipel = new Hipel("aboba", "abobich");
  }
}
