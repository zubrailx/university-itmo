package com.company.exceptions;

public class SomeSameObjectsException extends RuntimeException{
    private int value;

    public SomeSameObjectsException(String message, int val){
        super(message);
        value = val;
    }

    public int getValue(){
        return value;
    }
}
