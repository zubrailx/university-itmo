import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

import task1.Secans;

@TestInstance(Lifecycle.PER_CLASS)
class SecansTests {

  int tailor_n;
  double epsilon;

  @BeforeAll
  public void init() {
    tailor_n = 20;
    epsilon = 0.01;
  }

  private double mathSec(double x) {
    return 1 / Math.cos(x);
  }

  // @ParameterizedTest
  // @ValueSource(doubles = { -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6 })
  // void Secans_GreenZone_Greater(final double x) {
  // assertTrue(1 / Math.cos(x) - epsilon * 2 < Secans.secans(x, tailor_n));
  // }

  // @ParameterizedTest
  // @ValueSource(doubles = { -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6 })
  // void Secans_GreenZone_Lower(final double x) {
  // assertTrue(1 / Math.cos(x) + epsilon * 2 > Secans.secans(x, tailor_n));
  // }

  @ParameterizedTest
  @ValueSource(doubles = { -0.3, -0.2, -0.1, 0, 0.1, 0.2, 0.3 })
  void Secans_GreenZone_EqualsEps(final double x) {
    assertEquals(mathSec(x), Secans.secans(x, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { 0.8, 0.6, 0.4 })
  void Secans_BlueZoneRight_EqualsEps(final double x) {
    assertEquals(mathSec(x), Secans.secans(x, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { 1.1, 1.0, 0.9 })
  void Secans_YellowZoneRight_EqualsEps(final double x) {
    assertEquals(mathSec(x), Secans.secans(x, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { -0.8, -0.6, -0.4 })
  void Secans_BlueZoneLeft_EqualsEps(final double x) {
    assertEquals(mathSec(x), Secans.secans(x, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { -1.1, -1.0, -0.9 })
  void Secans_YellowZoneLeft_EqualsEps(final double x) {
    assertEquals(mathSec(x), Secans.secans(x, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { -0.3, -0.2, -0.1, 0, 0.1, 0.2, 0.3 })
  void Secans_GreenZoneInv_EqualsEps(final double x) {
    assertEquals(mathSec(x - Math.PI), Secans.secans(x - Math.PI, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { 0.8, 0.6, 0.4 })
  void Secans_BlueZoneInvRight_EqualsEps(final double x) {
    assertEquals(mathSec(x - Math.PI), Secans.secans(x - Math.PI, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { 1.1, 1.0, 0.9 })
  void Secans_YellowZoneInvRight_EqualsEps(final double x) {
    assertEquals(mathSec(x - Math.PI), Secans.secans(x - Math.PI, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { -0.8, -0.6, -0.4 })
  void Secans_BlueZoneInvLeft_EqualsEps(final double x) {
    assertEquals(mathSec(x - Math.PI), Secans.secans(x - Math.PI, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { -1.1, -1.0, -0.9 })
  void Secans_YellowZoneInvLeft_EqualsEps(final double x) {
    assertEquals(mathSec(x - Math.PI), Secans.secans(x - Math.PI, tailor_n), epsilon);
  }

  @ParameterizedTest
  @ValueSource(doubles = { 2, 4, 7, 11, 15 })
  void Secans_GreenZonePeriod_EqualsEps(final double period) {
    assertEquals(mathSec(0.6 + period * Math.PI), Secans.secans(0.6 + period * Math.PI, tailor_n), epsilon);
  }
}
