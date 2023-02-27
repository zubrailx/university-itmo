package task3;

public class VehicleOwner extends AbstrHuman {

  private Vehicle vehicle;

  public VehicleOwner(AbstrHuman human, Vehicle vehicle) {
    super(human);
    this.vehicle = vehicle;
  }

  public Vehicle.Seat allowSeat(AbstrHuman human) {
    if (this.getRep() < human.getRep() ||
        (this.getName() == human.getName())) {
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
