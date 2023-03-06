package task1;

import java.util.ArrayList;

public class Secans {

  private Secans() {}

  public static double secans(double x, int n) {
    var table = getBoustrophedonCoef(n);

    // Normalize function cuz it is periodical
    // and very big error because of large power
    int sign = 1;
    {
      while (x <= -Math.PI) {
        x += Math.PI * 2;
      }

      while (x >= Math.PI) {
        x -= Math.PI * 2;
      }

      if (x <= -Math.PI / 2) {
        sign = -1;
        x += Math.PI;
      }

      if (x >= Math.PI / 2) {
        sign = -1;
        x -= Math.PI;
      }

    }

    double res = 0;
    for (int i = 0; i <= n; i += 2) {
      res += table.get(i)[0] * Math.pow(x, i) / factorial(i);
    }
    return sign * res;
  }

  private static long factorial(int n) {
    long fact = 1;
    for (int i = 2; i <= n; i++) {
      fact = fact * i;
    }
    return fact;
  }

  // https://math.stackexchange.com/questions/2616561/taylor-series-for-secx
  private static ArrayList<double[]> getBoustrophedonCoef(int n) {
    var table = new ArrayList<double[]>();

    double[] prev, cur, tmp = { 1 };
    table.add(tmp);

    for (int i = 1; i <= n; ++i) {
      prev = table.get(i - 1);
      cur = new double[prev.length + 1];

      for (int j = 0; j < prev.length; ++j) {
        if (i % 2 != 0) {
          cur[cur.length - 1 - j] = sumSubarr(prev, 0, prev.length - j);
        } else {
          cur[j] = sumSubarr(prev, j, prev.length);
        }
      }
      table.add(cur);
    }
    return table;
  }

  private static int sumSubarr(double[] arr, int s, int e) {
    int result = 0;
    for (; s < e; ++s) {
      result += arr[s];
    }
    return result;
  }
}
