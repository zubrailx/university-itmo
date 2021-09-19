package com.company.human;

public class Others extends AdvancedHuman {

    public Others(){
        super("others", "far away");
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        Others obj2 = (Others) obj;
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
