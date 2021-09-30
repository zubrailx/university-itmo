package com.company.exceptions;;

public class NegativeAmountException extends Exception {
    private int value;

    public NegativeAmountException(String message, int n){
        super(message);
        this.value = n;
    }

    public int getValue(){
        return value;
    }
}
