package task3;

public enum HumanImportance {
  DEFAULT(0),
  FRONT(10),
  HIPEL(100);

  private final int importance;
  
  private HumanImportance(int importance) {
    this.importance = importance;
  }

  public boolean isGreater(HumanImportance importance) {
    return this.importance > importance.importance;
  }
}
