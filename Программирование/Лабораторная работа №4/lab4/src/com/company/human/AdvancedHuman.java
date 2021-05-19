package com.company.human;

import com.company.enums.Adjective;
import com.company.enums.Floor;
import com.company.enums.Location;

public abstract class AdvancedHuman extends Human {
    private String name;
    private Location location;

    public AdvancedHuman(String name, Location location){
        this.name = name;
        this.location = location;
    }
    public AdvancedHuman(String name){
        this.name = name;
        this.location = Location.UNKNOWN;
    }
    public AdvancedHuman(){
        this.name = Adjective.UNKNOWN.getValueOfAdjective();
        this.location = Location.UNKNOWN;
    }

    public Location getLocation(){
        return location;
    }
    public void setLocation(Location location){
        this.location = location;
    }
    public String getLocationAsString(){
        return this.getLocation().getValueOfLocation();
    }

    @Override
    public String getName(){
        return name;
    }
    @Override
    public void setName(String name){
        this.name = name;
    }

    static public class Surface{
        private Floor surface;

        public Surface(Floor floor){
            this.surface = floor;
            System.out.println("Current surface of this person is " + getSurface().getValueOfFloor());
        }

        public Floor getSurface() {
            return surface;
        }
        public void setSurface(Floor floor){
            surface = floor;
            System.out.println("Now the surface is " + getSurface().getValueOfFloor());
        }
        public void printSurface(){
            System.out.println("Current surface is " + getSurface().getValueOfFloor());
        }
    }
    @Override
    public boolean equals(Object obj) {
        if (obj == this) return true;
        if (obj == null || obj.getClass() != this.getClass()) return false;
        AdvancedHuman objH = (AdvancedHuman) obj;
        return this.getName().equals(objH.getName()) && this.getName() != null && this.getLocation().equals(objH.getLocation());
    }

    @Override
    public int hashCode() {
        final int prime = 17;
        int result = 1;
        result = prime * result + ((this.getName() == null) ? 0: this.getName().hashCode()) + ((this.getLocation() == null) ? 0: this.getLocation().hashCode()*3);
        return result;
    }

    @Override
    public String toString() {
        return "AdvancedHuman." + this.getName() + ", location: " + this.getLocation();
    }
}
