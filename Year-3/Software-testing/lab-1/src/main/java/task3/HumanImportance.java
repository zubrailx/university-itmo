package task3;

public enum HumanImportance {
  DEFAULT(0),
  FRONT(10),
  HIPEL(100);

  private final int importance;
  
  HumanImportance(int importance) {
    this.importance = importance;
  }

  public int getRep() {
    return importance;
  }
}
