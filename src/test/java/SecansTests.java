import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

import task1.Secans;

public class SecansTests {
  @Test
  public void Calc_Zero_ReturnsResult() {
    assertEquals(0, Secans.calc(10, 5));
  }
}
