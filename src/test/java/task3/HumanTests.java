package task3;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

@TestInstance(Lifecycle.PER_CLASS)
class HumanTests {

  @Test
  public void Constructor_NoExcept() {
    new Human("aboba", "abobich");
  }

  @Test
  public void GetRep_AssertEquals() {
    Human human = new Human("aboba", "abobich");
    assertEquals(HumanImportance.DEFAULT.getRep(), human.getRep());
  }

  @Test
  public void GetName_AssertEquals() {
    Human human = new Human("aboba", "abobich");
    assertEquals("aboba", human.getName());

  }
  @Test
  public void GetSurname_AssertEquals() {
    Human human = new Human("aboba", "abobich");
    assertEquals("abobich", human.getSurname());
  }
}
