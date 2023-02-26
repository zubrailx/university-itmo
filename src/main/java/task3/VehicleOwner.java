package task3;

public class VehicleOwner extends AbstrHuman {

  private Vehicle vehicle;

  public VehicleOwner(AbstrHuman human, Vehicle vehicle) {
    super(human);
    this.vehicle = vehicle;
  }

  public boolean sitDriverSeat() {
    var seats = vehicle.getSeats();
    seats[Vehicle.OWNER_SEAT].open();
    return seats[Vehicle.OWNER_SEAT].trySeat(this);
  }

  public Vehicle.Seat allowSeat(AbstrHuman human) {
    if (this.importance.isGreater(human.importance)) {
      return this.getFirstFreeSeat();  
    } else {
      return null;
    }
  }

  private Vehicle.Seat getFirstFreeSeat() {
    var seats = vehicle.getSeats();
    for (var seat : seats) {
      if (seat.isFree()) {
        return seat;
      }
    }
    return null;
  }
}
