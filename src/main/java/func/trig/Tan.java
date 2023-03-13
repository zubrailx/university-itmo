package func.trig;

import func.NumRFunc;

/**
 * Tan
 */
public class Tan implements NumRFunc {

  private Cos cos;
  private Sin sin;

  public Tan(int lastTailor) {
    this.cos = new Cos(lastTailor);
    this.sin = new Sin(lastTailor);
  }

  @Override
  public Double calc(Double x) {
    return sin.calc(x) / cos.calc(x);
  }
}
