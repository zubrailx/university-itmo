package com.company.exceptions;

public class FirstPersonNarratorException extends Exception {
    private String str;

    public FirstPersonNarratorException(String message, String str){
        super(message);
        this.str=str;
    }

    public String getStr(){
        return str;
    }
}
