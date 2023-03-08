package task2;

import java.util.ArrayList;
import java.util.LinkedList;

public class BucketSort {

  private BucketSort() {
  }

  public static int[] sorted(int[] input, int buckCount) {
    buckCount = buckCount <= 0 ? 1 : buckCount;
    int[] output = new int[input.length];

    var buckets = new ArrayList<LinkedList<Integer>>();
    for (int i = 0; i < buckCount; ++i) {
      buckets.add(i, new LinkedList<>());
    }

    fillBuckets(buckets, input);

    int pos = 0;
    for (var bucket : buckets) {
      for (var elem : bucket) {
        output[pos++] = elem;
      }
    }
    return output;
  }

  private static Pair getPair(int[] input) {
    int min = Integer.MAX_VALUE;
    int max = Integer.MIN_VALUE;
    for (int i : input) {
      max = Math.max(max, i);
      min = Math.min(min, i);
    }
    return new Pair(min, max);
  }

  private static int getBuckNum(int val, Pair pair, int buckets) {
    int bnum = (int) ((long) (val - pair.min) * (long) buckets / ((long) pair.max - (long) pair.min));
    return bnum >= buckets ? buckets - 1 : bnum;
  }

  private static void fillBuckets(ArrayList<LinkedList<Integer>> buckets, int[] input) {
    var pair = getPair(input);
    // then array is already sorted -> insert everything in first bucket
    if (pair.min == pair.max) {
      for (var val : input) {
        buckets.get(0).addFirst(val);
      }
      return;
    }

    int pos = 0;
    while (pos < input.length) {
      int ival = input[pos++];
      int bnum = getBuckNum(ival, pair, buckets.size());

      var bucket = buckets.get(bnum);

      boolean has_ins = false;
      var iter = bucket.listIterator();

      while (iter.hasNext()) {
        int bval = iter.next();
        if (ival <= bval) {
          int bpos = iter.nextIndex() - 1;
          bucket.add(bpos, ival);
          has_ins = true;
          break;
        }
      }
      if (!has_ins) {
        bucket.addLast(ival);
      }
    }
  }

  static class Pair {
    final int min;
    final int max;

    Pair(int min, int max) {
      this.min = min;
      this.max = max;
    }
  }
}
