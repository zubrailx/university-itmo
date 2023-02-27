package task3;

import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestInstance;
import org.junit.jupiter.api.TestInstance.Lifecycle;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.ValueSource;

@TestInstance(Lifecycle.PER_CLASS)
public class VehicleTests {

  @ParameterizedTest
  @ValueSource(ints = { 5, 10 })
  public void Constructor_NoExcept(int seats) {
    Vehicle vehicle = new Vehicle(seats);
    assertEquals(seats, vehicle.getSeats().length);
  }

  @Test
  public void Constructor_NoExcept_OneSeat() {
    Vehicle vehicle = new Vehicle(-5);
    assertEquals(Vehicle.MIN_SEATS, vehicle.getSeats().length);
  }

  @Test
  public void IsDriver_TestDriver() {
    Vehicle vehicle = new Vehicle(-5);
    var human = new Human("dfa", "dfaf");
    var human2 = new Human("dfa", "dfaf");
    var vehicleOwner = new VehicleOwner(human, vehicle);
    assertEquals(false, vehicle.isDriver(vehicleOwner));
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    assertEquals(true, vehicle.isDriver(vehicleOwner));
    assertEquals(false, vehicle.isDriver(human2));
  }

  @Test
  public void SeatIsLocked_NoExcept() {
    Vehicle vehicle = new Vehicle(-5);
    vehicle.getSeats()[Vehicle.DRIVER_DEAT].isLocked();
  }

  @Test
  public void SeatIsOpened_NoExcept() {
    Vehicle vehicle = new Vehicle(-5);
    vehicle.getSeats()[Vehicle.DRIVER_DEAT].isOpened();
  }

  @Test
  public void SeatLock_NotDriverAndDriver() {
    Vehicle vehicle = new Vehicle(-5);
    Human ndriver = new Human("fsafa", "fasfa");
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    seat.lock(vehicleOwner);
    assertEquals(true, seat.isLocked());
    seat.unlock(vehicleOwner);
    seat.lock(ndriver);
    assertEquals(false, seat.isLocked());
  }

  @Test
  public void SeatOpen_LockedAndUnlocked() {
    Vehicle vehicle = new Vehicle(-5);
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    seat.close();
    seat.lock(vehicleOwner);
    seat.open();
    assertEquals(false, seat.isOpened());
    seat.unlock(vehicleOwner);
    seat.open();
    assertEquals(true, seat.isOpened());
  }

  @Test
  public void SeatUnLock_DriverAndNotDriver() {
    Vehicle vehicle = new Vehicle(-5);
    Human ndriver = new Human("fsafa", "fasfa");
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    seat.lock(vehicleOwner);
    assertEquals(true, seat.isLocked());
    seat.unlock(ndriver);
    assertEquals(true, seat.isLocked());
  }

  @Test
  public void SeatSit_TestPassenger() {
    Vehicle vehicle = new Vehicle(2);
    var human2 = new Human("faf", "faf");
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    var seat2 = vehicle.getSeats()[1];
    seat2.open();
    assertEquals(true, seat2.sit(human2));
    assertEquals(false, seat2.sit(human2));
  }

  @Test
  public void SeatSit_TestLockedOpened() {
    Vehicle vehicle = new Vehicle(2);
    var human2 = new Human("faf", "faf");
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    var seat2 = vehicle.getSeats()[1];
    seat2.lock(vehicleOwner);
    assertEquals(false, seat2.sit(human2));
    seat2.unlock(vehicleOwner);
    assertEquals(false, seat2.sit(human2));
    seat2.open();
    assertEquals(true, seat2.sit(human2));
  }

  @Test
  public void SeatLeave_HumanNoHuman_Any() {
    Vehicle vehicle = new Vehicle(2);
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    assertEquals(true, seat.leave(vehicleOwner));
    assertEquals(false, seat.leave(vehicleOwner));
  }

  @Test
  public void SeatLeave_NotSameHumanNull_Any() {
    Vehicle vehicle = new Vehicle(2);
    var human2 = new Human("dfaff", "fafaf");
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    assertEquals(false, seat.leave(human2));
    assertEquals(true, seat.leave(vehicleOwner));
    assertEquals(false, seat.leave(null));
  }

  @Test
  public void SeatLeave_Locked_Any() {
    Vehicle vehicle = new Vehicle(2);
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    seat.lock(vehicleOwner);
    assertEquals(false, seat.leave(vehicleOwner));
    seat.unlock(vehicleOwner);
    assertEquals(true, seat.leave(vehicleOwner));
  }

  @Test
  public void SeatLeave_Opened_Any() {
    Vehicle vehicle = new Vehicle(2);
    var vehicleOwner = new VehicleOwner(new Human("fafa", "fasf"), vehicle);
    var seat = vehicle.getSeats()[Vehicle.DRIVER_DEAT];
    seat.open();
    seat.sit(vehicleOwner);
    seat.close();
    assertEquals(false, seat.leave(vehicleOwner));
    seat.open();
    assertEquals(true, seat.leave(vehicleOwner));
  }
}
