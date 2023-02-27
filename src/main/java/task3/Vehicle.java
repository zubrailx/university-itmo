package task3;

// we live in the world with no keys
public class Vehicle {
  public static int DRIVER_DEAT = 0;
  public static int MIN_SEATS = 1;
  private Seat seats[]; // first seat is owner seat

  public Vehicle(int seats) {
    seats = seats <= 0 ? MIN_SEATS : seats;
    this.seats = new Seat[seats];
    for (int i = 0; i < seats; ++i) {
      this.seats[i] = new Seat();
    }
  }

  public Seat[] getSeats() {
    return seats;
  }

  public boolean isDriver(AbstrHuman human) {
    if (seats[DRIVER_DEAT].passenger != null) {
      if (seats[DRIVER_DEAT].passenger == human) {
        return true;
      }
    }
    return false;
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

    public boolean isLocked() {
      return locked;
    }

    public boolean isOpened() {
      return opened;
    }

    public void lock(AbstrHuman human) {
      if (isDriver(human)) {
        this.locked = true;
      }
    }

    public void unlock(AbstrHuman human) {
      if (isDriver(human)) {
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

    public boolean sit(AbstrHuman human) {
      if (passenger == null) {
        if (!locked) {
          if (opened) {
            passenger = human;
            return true;
          }
        }
      }
      return false;
    }

    public boolean leave(AbstrHuman human) {
      if (human != null && passenger == human && !locked && opened) {
        passenger = null;
        return true;
      }
      return false;
    }

    public boolean isFree() {
      return this.passenger == null;
    }

  }
}
