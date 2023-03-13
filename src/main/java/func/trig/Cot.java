package func.trig;

import func.NumRFunc;

/**
 * Cot
 */
public class Cot implements NumRFunc {

  private Cos cos;
  private Sin sin;

  public Cot(int lastTailor) {
    this.cos = new Cos(lastTailor);
    this.sin = new Sin(lastTailor);
  }

  @Override
  public Double calc(Double x) {
    return cos.calc(x) / sin.calc(x);
  }
}
