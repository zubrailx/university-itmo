package task3;

public abstract class AbstrHuman implements IRep {
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

  public int getRep() {
    return importance.getRep();
  }

  public String getName() {
    return name;
  }

  public String getSurname() {
    return surname;
  }
}
