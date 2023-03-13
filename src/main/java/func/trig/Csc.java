package func.trig;

import func.NumRFunc;

/**
 * Csc
 */
public class Csc implements NumRFunc {

  private Sin sin;

  public Csc(int lastLailor) {
    this.sin = new Sin(lastLailor);
  }

  @Override
  public Double calc(Double x) {
    return 1 / sin.calc(x);
  }
}
