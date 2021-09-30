package com.company.human;

import com.company.enums.Adjective;

public abstract class Human {
    private String name;

    public Human(String name){
        this.name = name;
    }
    public Human(){
        this.name = Adjective.UNKNOWN.getValueOfAdjective();
    }
    public String getName(){
        return name;
    }
    public void setName(String name){
        this.name = name;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == this) return true;
        if (obj == null || obj.getClass() != this.getClass()) return false;
        Human objH = (Human) obj;
        return this.getName().equals(objH.getName()) && this.getName() != null;
    }

    @Override
    public int hashCode() {
        final int prime = 27;
        int result = 1;
        result = prime * result + ((this.getName() == null) ? 0: this.getName().hashCode());
        return result;
    }
    @Override
    public String toString() {
        return "Human." + this.getName();
    }
}
