package task3;

public class Towel implements IRep {

  private String brand;
  private int rep;

  public Towel(String brand, int rep) {
    this.brand = brand;
    this.rep = rep;
  }

  public String getBrand() {
    return brand;
  }

  @Override
  public int getRep() {
    return rep;
  }
}
