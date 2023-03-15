package func.log;

import func.NumRFunc;

/**
 * Ln
 */
public class Ln implements NumRFunc {

  private int lastTailor;

  public Ln(int lastTailor) {
    this.lastTailor = lastTailor;
  }

  public Double calc(Double x) {
    double res = 0;
    // bound checks
    if (Double.isNaN(x) || x < 0)
      return Double.NaN;
    if (x == 0)
      return Double.NEGATIVE_INFINITY;
    if (x == Double.POSITIVE_INFINITY)
      return Double.POSITIVE_INFINITY;

    // if (0 < x <= 2) { search as ln(1 + x) series with substitution }
    // else res = -ln(1/x) with substitution, where 1/x <= 2 ==> x >= 0.5
    boolean substInv = (x > 2);
    if (substInv) {
      x = 1 / x - 1;
    } else {
      x = x - 1;
    }

    int sign = 1;
    for (int i = 1; i <= lastTailor; ++i) {
      res += sign * Math.pow(x, i) / i;
      sign = -sign;
    }
    return substInv ? -res : res;
  }
}
