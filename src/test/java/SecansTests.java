import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;

import task1.Secans;

public class SecansTests {
  @Test
  public void Calc_Zero_ReturnsResult() {
    double delta = 0.0001;
    assertEquals(1, Secans.secans(0, 20), delta);
  }
}
