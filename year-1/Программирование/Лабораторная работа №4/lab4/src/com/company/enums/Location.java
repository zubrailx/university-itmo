package com.company.enums;

public enum Location {
    UNKNOWN("unknown"),
    WATER("water"),
    GROUND("ground"),
    FAR_AWAY("far away");

    private final String value;

    Location(String str){
        value = str;
    }
    public String getValueOfLocation(){
        return value;
    }
}
