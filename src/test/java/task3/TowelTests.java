package task3;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

@TestInstance(Lifecycle.PER_CLASS)
public class TowelTests {

  @Test
  public void Constructor_NoExcept() {
    Towel towel = new Towel("aboba", 50);
  }

  @Test
  public void GetBrand_AssertEquals() {
    Towel towel = new Towel("guchi", 50);
    assertEquals("guchi", towel.getBrand());
  }

  @Test
  public void GetRep_AssertEquals() {
    Towel towel = new Towel("guchi", 50);
    assertEquals(50, towel.getRep());
  }
}
