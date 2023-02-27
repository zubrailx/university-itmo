package task3;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;

@TestInstance(Lifecycle.PER_CLASS)
public class VehicleOwnerTests {

  @Test
  public void Constructor_NoExcept() {
    Human human = new Human("abobich", "vesilbev");
    Vehicle vehicle = new Vehicle(6);
    VehicleOwner owner = new VehicleOwner(human, vehicle);
  }

  @Test
  public void AllowSeat_EqualHuman_RetSeat() {
    Human human = new Human("abobich", "vesilbev");
    Front front = new Front("abobich", "vesilbev");
    Vehicle vehicle = new Vehicle(6);
    VehicleOwner owner = new VehicleOwner(front, vehicle);
    assertNotNull(owner.allowSeat(human));
  }

  @Test
  public void AllowSeat_HumanWithMoreRep_RetSeat() {
    Human human = new Human("abobich", "vesilbev");
    Front front = new Front("abobich", "vesilbev2");
    Vehicle vehicle = new Vehicle(6);
    VehicleOwner owner = new VehicleOwner(human, vehicle);
    assertNotNull(owner.allowSeat(front));
  }


  @Test
  public void AllowSeat_HumanWithEqRep_RetNull() {
    Human human = new Human("abobich", "vesilbev");
    Human human2 = new Human("abobich2", "vesilbev");
    Vehicle vehicle = new Vehicle(6);
    VehicleOwner owner = new VehicleOwner(human, vehicle);
    assertNull(owner.allowSeat(human2));
  }

  @Test
  public void AllowSeat_HumanWithLowRep_RetNull() {
    Human human = new Human("abobich", "vesilbev");
    Front front = new Front("abobich2", "vesilbev");
    Vehicle vehicle = new Vehicle(6);
    VehicleOwner owner = new VehicleOwner(front, vehicle);
    assertNull(owner.allowSeat(human));
  }

  @Test
  public void AllowSeat_HumanNoSpace_RetNull() {
    Human human = new Human("abobich", "vesilbev");
    Vehicle vehicle = new Vehicle(1);
    VehicleOwner owner = new VehicleOwner(human, vehicle);
    var dseat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    dseat.open();
    dseat.sit(owner);
    assertNull(owner.allowSeat(human));
  }
}
