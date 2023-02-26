package task1;

public class Secans {

  public static int calc(double x, int n) {
    int [] arr = getBoustrophedonCoef(n);
    return 0;
  }


  // https://math.stackexchange.com/questions/2616561/taylor-series-for-secx
  private static int[] getBoustrophedonCoef(int n) {
    int[] prev = { 1 };
    int[] cur;

    for (int i = 1; i <= n; ++i) {
      cur = new int[prev.length + 1];

      for (int j = 0; j < prev.length; ++j) {
        if (i % 2 != 0) {
          cur[cur.length - 1 - j] = sumSubarr(prev, 0, prev.length - j);
        } else {
          cur[j] = sumSubarr(prev, j, prev.length); 
        }
      }

      prev = cur;
    }
    return prev;
  }

  private static int sumSubarr(int[] arr, int s, int e) {
    int result = 0;
    for (; s < e; ++s) {
      result += arr[s];
    }
    return result;
  }
}
