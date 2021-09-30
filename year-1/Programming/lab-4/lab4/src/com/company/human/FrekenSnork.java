package com.company.human;

import com.company.exceptions.*;
import com.company.interfaces.*;
import com.company.enums.*;

public class FrekenSnork extends AdvancedHuman implements Movable, Shakable, Speakable, Thinkable, Rootable{

    public static int id = 0;
    {
        id++;
        if (id > 1){
            try {
                throw new SomeSameObjectsException("There can't be more than 1 Freken Snork", id);
            }catch(SomeSameObjectsException ex){
                System.out.println("-----------------------------------------------------------\n" + ex.getMessage());
                System.out.println("Your number of objects: " + ex.getValue()+"\n-----------------------------------------------------------");
            }
        }
    }

    public FrekenSnork(){
        super("Freken Snork");
    }
    public FrekenSnork(Location location){
        super("Freken Snork", location);
    }

    @Override
    public void run(boolean b) {
        if (b) System.out.println(this.getName() + " is running ");
        else System.out.println(this.getName() + " is not running");
    }

    @Override
    public void runToHuman(AdvancedHuman human) {
        System.out.println("running to " + human.getName());
    }

    @Override
    public void comeUp() {
        System.out.println(this.getName() + " is coming closer");
    }

    @Override
    public void comeUp(AdvancedHuman human) {
        System.out.println(this.getName() + " is coming closer to " + human.getName());
    }

    @Override
    public void takeStepsHow(Adjective adj, int n) throws NegativeAmountException {
        if (n < 0) throw new NegativeAmountException("You can't do negative amount of steps", n);
        System.out.println(this.getName() + " is taking " + n + " " + adj.getValueOfAdjective() + " steps");
    }

    @Override
    public void shake() {
        System.out.println(this.getName() + " is shaking");
    }

    @Override
    public void rooted(Location location) {
        System.out.println(this.getName() + " is rooted to the " + location.getValueOfLocation());
    }

    @Override
    public void say(String str) {
        System.out.println(this.getName() + " is saying: " + str);
    }

    @Override
    public void think() {
        System.out.print(this.getName() + " is thinking of ");
    }

}
