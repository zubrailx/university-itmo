package com.company;

import com.company.enums.*;
import com.company.exceptions.*;
import com.company.human.*;
import com.company.interfaces.*;

public class Main {

    public static void main(String[] args) {
        Speakable narrator = new Speakable() {
            @Override
            public void say(String str) {

                String firstPerson = " i ";

                class Replica{
                    public void narratorSays() throws FirstPersonNarratorException{
                        String replica = "Narrator says that ";
                        if (str.toLowerCase().contains(firstPerson)){ throw new FirstPersonNarratorException("Narrator can't say anything as a first person",str );
                        }
                        else System.out.println(replica + str);
                    }
                }
                Replica replica = new Replica();
                try {
                    replica.narratorSays();
                }catch(FirstPersonNarratorException e){
                    System.out.println("-----------------------------------------------------------");
                    System.out.println(e.getMessage());
                    System.out.println("Your str: \""+e.getStr()+ "\"" + "\nIt contains \"I\" or \"i\"");
                    System.out.println("-----------------------------------------------------------");
                }
            }

        };
        FrekenSnork frekenSnork = new FrekenSnork();
        WoodenWoman woodenWoman = new WoodenWoman();
        Others others = new Others();
        FrekenSnork.Surface surf = new AdvancedHuman.Surface(Floor.GRASS);

        frekenSnork.think();
        frekenSnork.runToHuman(others);
        frekenSnork.run(false);
        frekenSnork.say("be braver!");
        frekenSnork.say("let's see who it is!");
        frekenSnork.shake();
        frekenSnork.comeUp(woodenWoman);
        woodenWoman.setName("Wooden Woman");
        narrator.say("It was a " + woodenWoman.getHeight() + " " + woodenWoman.getName());

        try{
        frekenSnork.takeStepsHow(Adjective.FRIGHTENED, 5);
        }catch(NegativeAmountException e){
        System.out.println(e.getMessage());
        System.out.println("Your amount: " + e.getValue());
        }

        frekenSnork.rooted(Location.GROUND);
        surf.setSurface(Floor.DIRT);
        narrator.say(woodenWoman.getName() +" was " + woodenWoman.getKindOfBody() + "!");
        narrator.say(woodenWoman.getName() + " was also " + Adjective.PRETTY.getValueOfAdjective());
        narrator.say(woodenWoman.getName() + " had a " + woodenWoman.face.getTypeOfBodyPartAsString() + ", " +
                woodenWoman.lips.getTypeOfBodyPartAsString() + ", " + woodenWoman.cheeks.getTypeOfBodyPartAsString() +", " + woodenWoman.eyes.getTypeOfBodyPartAsString() +" and " + woodenWoman.hair.getTypeOfBodyPartAsString());
    }
}
