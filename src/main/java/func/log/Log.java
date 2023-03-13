package func.log;

import func.NumRFuncBase;

/**
 * Log
 */
public class Log implements NumRFuncBase {

  private Ln ln;

  public Log(int lastTailor) {
    this.ln = new Ln(lastTailor);
  }

  @Override
  public Double calc(Double base, Double x) {
    return ln.calc(x) / ln.calc(base);
  }
}
