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

  @Override
  public Double calc(Double x) {
    return 1 / cos.calc(x);
  }
}
