package com.company.human;

import com.company.enums.Adjective;

public abstract class Human {
    private String name;

    Human(String name){
        this.name = name;
    }
    Human(){
        this.name = Adjective.UNKNOWN.getValue();
    }

    public String getName(){
        return name;
    }

    public void setName(String name){
        this.name = name;
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        Human obj2 = (Human) obj;
        return this.getName().equals(obj2.getName()) && this.getName() != null;
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
        return this.getClass().getSimpleName() + ", whose name is " +this.getName();
    }
}
