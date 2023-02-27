import static org.junit.jupiter.api.Assertions.assertArrayEquals;

import java.util.stream.Stream;

import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.api.extension.ExtensionContext;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.ArgumentsProvider;
import org.junit.jupiter.params.provider.ArgumentsSource;

import task2.BucketSort;

@TestInstance(Lifecycle.PER_CLASS)
public class BucketSortTests {

  @ParameterizedTest
  @ArgumentsSource(BucketSortArgsProvider.class)
  void Sorted_Equals_SortResult(int buckets, int[] input, int[] output) {
    assertArrayEquals(output, BucketSort.sorted(input, buckets));
  }

  static class BucketSortArgsProvider implements ArgumentsProvider {
    @Override
    public Stream<? extends Arguments> provideArguments(ExtensionContext context) throws Exception {
      return Stream.of(
          // check array sort
          Arguments.of(5, new int[] {}, new int[] {}),
          Arguments.of(5, new int[] { 0 }, new int[] { 0 }),
          Arguments.of(5, new int[] { 5, 5, 5, 5, 5 }, new int[] { 5, 5, 5, 5, 5 }),
          Arguments.of(5, new int[] { Integer.MAX_VALUE, Integer.MIN_VALUE },
              new int[] { Integer.MIN_VALUE, Integer.MAX_VALUE }),
          Arguments.of(5, new int[] { 24, 32, 11 }, new int[] { 11, 24, 32 }),
          Arguments.of(5, new int[] { 5, 4, 3, 2, 1 }, new int[] { 1, 2, 3, 4, 5 }),
          Arguments.of(5, new int[] { 4, 2, 1, 5, 6, 11, 231, 31, 55, 65, 143, 43 },
              new int[] { 1, 2, 4, 5, 6, 11, 31, 43, 55, 65, 143, 231 }),
          // check number of buckets
          Arguments.of(-1, new int[] { 4, 2, 1, 5, 6, 11, 231, 31, 55, 65, 143, 43 },
              new int[] { 1, 2, 4, 5, 6, 11, 31, 43, 55, 65, 143, 231 }),
          Arguments.of(1, new int[] { 4, 2, 1, 5, 6, 11, 231, 31, 55, 65, 143, 43 },
              new int[] { 1, 2, 4, 5, 6, 11, 31, 43, 55, 65, 143, 231 }),
          Arguments.of(0, new int[] { 5, 5, 5, 5, 5 }, new int[] { 5, 5, 5, 5, 5 }),
          Arguments.of(1, new int[] {}, new int[] {}));
    }
  }
}
