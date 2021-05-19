package com.company.human;

import com.company.enums.Adjective;
import com.company.interfaces.Speakable;

public class Narrator extends Human implements Speakable {

    public Narrator(){
        super(Adjective.UNKNOWN.getValue());
    }
    public Narrator(String name){
        super(name);
    }

    @Override
    public void say(String str){
        System.out.println("Narrator: \"" + str + "\"");
    }
}
