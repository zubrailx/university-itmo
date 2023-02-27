package task3;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

@TestInstance(Lifecycle.PER_CLASS)
public class FrontTests {

  @Test
  public void Constructor_NoExcept() {
    Front front = new Front("aboba", "abobich");
  }

  @Test
  public void SetTowel_AssertEquals() {
    Front front = new Front("aboba", "abobich");
    Towel towel = new Towel("guchi", 50);
    front.setTowel(towel);
    assertEquals(towel, front.getTowel());
  }

  @Test
  public void GetRep_AssertEquals_TowelNotNull() {
    Front front = new Front("aboba", "abobich");
    Towel towel = new Towel("guchi", 50);
    front.setTowel(towel);
    assertEquals(HumanImportance.FRONT.getRep() + towel.getRep(), front.getRep());
  }

  @Test
  public void GetRep_AssertEquals_TowelNull() {
    Front front = new Front("aboba", "abobich");
    assertEquals(HumanImportance.FRONT.getRep(), front.getRep());
  }

}
