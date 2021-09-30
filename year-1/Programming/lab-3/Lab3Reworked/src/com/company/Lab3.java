package com.company;

import com.company.bodyParts.*;
import com.company.enums.*;
import com.company.human.*;

public class Lab3 {

    public static void main(String[] args) {
        FrekenSnork frekenSnork = new FrekenSnork();
        Others others = new Others();
        WoodenWoman woodenWoman = new WoodenWoman(Location.WATER.getLocation());
        Narrator narrator = new Narrator();
        Cheeks woodenWomanCheeks = new Cheeks(Adjective.ROSY.getValue());
        Eyes woodenWomanEyes = new Eyes(Adjective.ROUND.getValue() + ", " + Adjective.WIDELY_OPENED.getValue() + ", " + Adjective.BLUE.getValue());
        Face woodenWomanFace = new Face(Adjective.SMILING.getValue());
        Hair woodenWomanHair = new Hair(Adjective.BLUE.getValue() + ", " + Adjective.LONG.getValue());
        Lips woodenWomanLips = new Lips(Adjective.ROSY.getValue());

        frekenSnork.think();
        frekenSnork.runToHuman(others);
        frekenSnork.run(false);
        frekenSnork.say("be braver!");
        frekenSnork.say("let's see who it is!");frekenSnork.shake();
        frekenSnork.comeUp(woodenWoman);woodenWoman.setName("Wooden Woman");
        narrator.say("It was a " + woodenWoman.getHeight() + " " + woodenWoman.getName());frekenSnork.takeStepsHow(Adjective.FRIGHTENED.getValue(), 5 );frekenSnork.rooted(Location.GROUND.getLocation());narrator.say(woodenWoman.getName() +" was " + woodenWoman.getWomanIs() + "!");
        narrator.say(woodenWoman.getName() + " was also " + Adjective.PRETTY.getValue());
        narrator.say(woodenWoman.getName() + " had a " + woodenWoman.getFace(woodenWomanFace) + ", " +
                woodenWoman.getLips(woodenWomanLips) + ", " + woodenWoman.getCheeks(woodenWomanCheeks) +", " + woodenWoman.getEyes(woodenWomanEyes) +" and " + woodenWoman.getHair(woodenWomanHair));

        System.out.println(woodenWomanCheeks.toString());
        System.out.println(narrator.toString());
    }
}