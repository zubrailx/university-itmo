package task2;

import java.util.ArrayList;
import java.util.LinkedList;

public class BucketSort {
  private int[] oldarr, newarr;
  private int maxval;
  private int cur_idx = 0;
  private ArrayList<LinkedList<Integer>> buckets;

  public BucketSort(int[] array, int buckets) {
    this.oldarr = array;

    this.buckets = new ArrayList<LinkedList<Integer>>(buckets);
    for (int i = 0; i < buckets; ++i) {
      this.buckets.set(i, new LinkedList<Integer>());
    }
    // get max value in array
    if (array.length <= 0) {
      maxval = 0;
    } else {
      maxval = array[0];
      for (int i = 0; i < array.length; ++i) {
        maxval = Math.max(maxval, array[i]);
      }
    }
  }

  /*
   * @return true - something is done, false - perform merging
   */
  public boolean sortOne() {
    if (cur_idx >= oldarr.length) {
      return false;
    }

    int val = oldarr[cur_idx++];
    int bnum = (val * buckets.size()) / maxval;

    var bucket = buckets.get(bnum);

    boolean has_ins = false;
    var iter = bucket.listIterator();

    while(iter.hasNext()) {
      int inb = iter.next();
      if (val <= inb) {
        int pos = iter.nextIndex() - 1;
        bucket.add(pos, val);
        has_ins = true;
        break;
      }
    }
    if (!has_ins) {
      bucket.addLast(val);
    }
    return true;
  }

  /*
   * @return if do_merge == true then merge array and return it
   */
  public boolean merge() {
    if (cur_idx < oldarr.length) {
      return false;
    }

    this.newarr = new int[oldarr.length];
    int pos = 0;
    for (var bucket : buckets) {
      for (var elem : bucket) {
        this.newarr[pos++] = elem;
      }
    }
    return true;
  }

  public int[] getArray() {
    return newarr;
  }

  /*
   * Uses BucketSort operations to sort array at once
   *
   * @n number of buckets
   */
  public static int[] sort(int[] array, int n) {
    var bs = new BucketSort(array, n);

    while (bs.sortOne())
      ;
    bs.merge();
    return bs.getArray();
  }
}
