package task3;

public class AbstrHuman {
  protected HumanImportance importance;

  protected String name;
  protected String surname;

  public AbstrHuman(HumanImportance importance, String name, String surname) {
    this.importance = importance;
    this.name = name;
    this.surname = surname;
  }

  public AbstrHuman(AbstrHuman human) {
    this(human.importance, human.name, human.surname);
  }
}
