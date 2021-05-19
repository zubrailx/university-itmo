package com.company.human;

import com.company.interfaces.*;

public class FrekenSnork extends AdvancedHuman implements Movable, Shakable, Speakable, Thinkable, Rootable {

    public FrekenSnork(){
        super("Freken Snork");
    }
    public FrekenSnork(String location){
        super("Freken Snork", location);
    }

    @Override
    public void run(boolean b) {
        if (b)
            System.out.println(this.getName() + " is running ");
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
    public void takeStepsHow(String adjective, int n) {

        System.out.println(this.getName() + " is taking " + n + " " + adjective + " steps");
    }

    @Override
    public void shake() {
        System.out.println(this.getName() + " is shaking");
    }

    @Override
    public void rooted(String location) {
        System.out.println(this.getName() + " is rooted to the " + location);
    }

    @Override
    public void say(String str) {
        System.out.println(this.getName() + " is saying: " + str);
    }

    @Override
    public void think() {
        System.out.print(this.getName() + " is thinking of ");
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        FrekenSnork obj2 = (FrekenSnork) obj;
        return this.getName().equals(obj2.getName()) && this.getName() != null && (this.getCurrentLocation().equals(obj2.getCurrentLocation())) && this.getCurrentLocation() != null;
    }
    @Override
    public int hashCode(){
        final int prime = 31;
        int result = 1;
        result = prime * result + ((this.getName() == null) ? 0: this.getName().hashCode());
        return result;
    }
    @Override
    public String toString() {
        return (this.getName() + " | " + this.getCurrentLocation());
    }
}
