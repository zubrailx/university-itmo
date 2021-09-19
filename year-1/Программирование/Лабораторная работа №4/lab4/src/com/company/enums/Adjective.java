package com.company.enums;

public enum Adjective {
    BIG("big"),
    FRIGHTENED("frightened"),
    WOODEN("wooden"),
    PRETTY("pretty"),
    UNKNOWN("unknown"),
    SMILING("smiling"),
    ROSY("rosy"),
    ROUND("round"),
    WIDELY_OPENED("widely opened"),
    BLUE("blue"),
    LONG("long");

    private final String value;

    Adjective(String str){
        value = str;
    }

    public String getValueOfAdjective() {
        return value;
    }
}
