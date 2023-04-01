package func.trig;

import func.NumRFunc;

/**
 * Sec
 */
public class Sec implements NumRFunc {

  private Cos cos;

  public Sec(int lastTailor) {
    this.cos = new Cos(lastTailor);
  }

  public Sec(Cos cos) {
    this.cos = cos;
  }


  @Override
  public Double calc(Double x) {
    return 1 / cos.calc(x);
  }
}
