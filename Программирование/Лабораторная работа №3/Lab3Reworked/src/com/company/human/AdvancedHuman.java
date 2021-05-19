package com.company.human;

import com.company.enums.*;

public abstract class AdvancedHuman extends Human{

    private String name;
    private String currentLocation;

    AdvancedHuman(String name, String location){
        this.name = name;
        this.currentLocation = location;
    }

    AdvancedHuman(String name){
        this.name = name;
        this.currentLocation = Location.UNKNOWN.getLocation();
    }
    AdvancedHuman(){
        this.name = Adjective.UNKNOWN.getValue();
        this.currentLocation = Location.UNKNOWN.getLocation();
    }

    public String getCurrentLocation(){
        return currentLocation;
    }
    public void setCurrentLocation(String str){
        currentLocation = str;
    }

    @Override
    public String getName(){
        return name;
    }

    @Override
    public void setName(String name){
        this.name = name;
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        AdvancedHuman obj2 = (AdvancedHuman) obj;
        return this.getName().equals(obj2.getName()) && this.getName() != null && this.getCurrentLocation().equals(obj2.getCurrentLocation()) && this.getCurrentLocation() != null;
    }
    @Override
    public int hashCode(){
        final int prime = 13;
        int result = 1;
        result = prime * result + ((this.getName() == null) ? 0: this.getName().hashCode()) + ((this.getCurrentLocation() == null)? 0: this.getCurrentLocation().hashCode());
        return result;
    }
    @Override
    public String toString() {
        return this.getClass().getSimpleName() + ", whose name is " +this.getName() + ", location: " + this.getCurrentLocation();
    }
}
