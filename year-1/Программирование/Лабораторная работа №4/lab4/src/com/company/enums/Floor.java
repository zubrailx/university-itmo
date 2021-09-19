package com.company.enums;

public enum Floor {
    GRASS("grass"),
    DIRT("dirt"),
    WATER("water");

    private String floor;

    Floor(String str){
        floor = str;
    }
    public String getValueOfFloor(){
        return floor;
    }
}
