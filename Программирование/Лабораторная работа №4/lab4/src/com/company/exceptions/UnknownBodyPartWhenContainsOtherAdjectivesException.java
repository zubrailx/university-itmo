package com.company.exceptions;

import com.company.enums.Adjective;

public class UnknownBodyPartWhenContainsOtherAdjectivesException extends Exception {

    private Adjective [] adjective;

    public UnknownBodyPartWhenContainsOtherAdjectivesException(String message, Adjective [] adjective){
        super(message);
        this.adjective = adjective;
    }

    public Adjective [] getAdjective() {
        return adjective;
    }
}