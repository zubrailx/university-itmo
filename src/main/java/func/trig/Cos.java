package func.trig;

import func.NumRFunc;

/**
 * Cos
 */
public class Cos implements NumRFunc {

  private int lastTailor;

  public Cos(int lastTailor) {
    this.lastTailor = lastTailor;
  }

  @Override
  public Double calc(Double x) {
    if (x == Double.POSITIVE_INFINITY || x == Double.NEGATIVE_INFINITY
        || x == Double.NaN) {
      return Double.NaN;
    }
    // normalize to minimize error
    while (x > 0 && x > Math.PI) {
      x -= 2 * Math.PI;
    }
    while (x < 0 && x < -Math.PI) {
      x += 2 * Math.PI;
    }
    
    double res = 0;
    for (int i = 0; i <= lastTailor; ++i) {
      res += (i % 2 == 0 ? 1 : -1) * Math.pow(x, 2 * i) / fact(2 * i);
    }
    return res;
  }

  private static long fact(int n) {
    long fact = 1;
    for (int i = 2; i <= n; i++) {
      fact = fact * i;
    }
    return fact;
  }
}
