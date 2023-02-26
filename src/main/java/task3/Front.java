package task3;

public class Front extends AbstrHuman {

  private Towel towel;

  public Front(String name, String surname) {
    super(HumanImportance.FRONT, name, surname);
    this.towel = null;
  }

  public void setTowel(Towel towel) {
    this.towel = towel;
  }

  @Override
  public int getRep() {
    int rep = super.getRep();
    if (towel != null) {
      rep += towel.getRep();
    }
    return rep;
  }
}
