package func;

import func.log.Log;
import func.trig.Cos;
import func.trig.Cot;
import func.trig.Csc;
import func.trig.Sec;
import func.trig.Sin;
import func.trig.Tan;

/**
 * Function
 */
public class Function implements NumRFunc {

  private Sin sin;
  private Tan tan;
  private Cot cot;
  private Csc csc;
  private Sec sec;
  private Cos cos;
  private Log log;

  public Function(int n) {
    sin = new Sin(n);
    tan = new Tan(n);
    cot = new Cot(n);
    csc = new Csc(n);
    sec = new Sec(n);
    cos = new Cos(n);
    log = new Log(n);
  }

  @Override
  public Double calc(Double x) {
    if (x <= 0) {
      return (((((Math.pow((Math.pow((Math.pow((sin.calc(x) / tan.calc(x)), 3) + cot.calc(x)), 2) / csc.calc(x)), 3)
          * cot.calc(x)) - cos.calc(x))
          - ((sec.calc(x) - ((cot.calc(x) + (sin.calc(x) * sin.calc(x))) / sin.calc(x)))
              + (((cos.calc(x) - cos.calc(x)) + cot.calc(x))
                  - (cos.calc(x) * Math.pow((tan.calc(x) + tan.calc(x)), 3)))))
          - (csc.calc(x) * Math.pow(sec.calc(x), 2)))
          - ((Math.pow((sec.calc(x) - (tan.calc(x) - sin.calc(x))), 2) + (((Math.pow(csc.calc(x), 2)
              + (((cot.calc(x) / cos.calc(x)) / tan.calc(x)) - (sin.calc(x) - tan.calc(x)))) - cos.calc(x))
              - ((csc.calc(x) + Math.pow((csc.calc(x) * sec.calc(x)), 2)) - (sin.calc(x) / tan.calc(x)))))
              * ((Math.pow(
                  ((Math.pow(sin.calc(x), 3) + Math.pow(sin.calc(x), 3)) - ((cot.calc(x) - cot.calc(x)) * sin.calc(x))),
                  3) - (sec.calc(x) / (csc.calc(x) - csc.calc(x)))) + sec.calc(x))));
    } else {
      return (((((log.calc(3., x) - log.calc(2., x)) * log.calc(10., x))
          / ((log.calc(5., x) + log.calc(3., x)) - (log.calc(5., x) - log.calc(10., x))))
          + (log.calc(10., x) / (log.calc(5., x) - log.calc(10., x)))) / log.calc(2., x));
    }
  }
}
