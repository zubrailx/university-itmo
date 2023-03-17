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


  public Sin(Cos cos) {
    this.cos = cos;
  }


  @Override
  public Double calc(Double x) {
    return cos.calc(Math.PI / 2 - x);
  }
}
