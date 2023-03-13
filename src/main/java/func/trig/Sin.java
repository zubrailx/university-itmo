package func.trig;

import func.NumRFunc;

/**
 * Sin
 */
public class Sin implements NumRFunc {

  private Cos cos;

  public Sin(int lastTailor) {
    this.cos = new Cos(lastTailor);
  }

  @Override
  public Double calc(Double x) {
    return cos.calc(Math.PI / 2 - x);
  }
}
