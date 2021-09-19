package com.company.enums;

public enum Location {

    UNKNOWN("unknown"),
    WATER("water"),
    GROUND("ground");

    private String location;

    Location(String str){
        location = str;
    }
    public String getLocation(){
        return location;
    }
}
