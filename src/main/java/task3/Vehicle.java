package task3;

// we live in the world with no keys
public class Vehicle {
  public static int OWNER_SEAT = 0;
  private Seat seats[]; // first seat is owner seat

  public Vehicle(int seats) {
    seats = seats <= 0 ? 1 : seats;
    this.seats = new Seat[seats];
  }

  public Seat[] getSeats() {
    return seats;
  }

  // DoorSeat
  public class Seat {
    private AbstrHuman passenger;
    private boolean locked;
    private boolean opened;

    public Seat() {
      locked = false;
      opened = false;
    }

    public void lock(AbstrHuman human) {
      if (isEqOwner(human)) {
        this.locked = false;
      }
    }

    public void unlock(AbstrHuman human) {
      if (isEqOwner(human)) {
        this.locked = false;
      }
    }

    public void open() {
      if (!this.locked) {
        this.opened = true;
      }
    }

    public void close() {
      this.opened = false;
    }

    public boolean trySeat(AbstrHuman human) {
      if (passenger == null && !locked && opened) {
        passenger = human;
        return true;
      }
      return false;
    }

    public boolean tryLeave(AbstrHuman human) {
      if (human != null && passenger == human && !locked && opened) {
        passenger = null;
        return true;
      }
      return false;
    }

    public boolean isFree() {
      return this.passenger == null;
    }

    private boolean isEqOwner(AbstrHuman human) {
      return seats[OWNER_SEAT].passenger != null && seats[OWNER_SEAT].passenger == human;
    }

  }
}
