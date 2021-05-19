package com.company.bodyParts;

import com.company.enums.Adjective;

public abstract class BodyParts {

    private final String typeOfBodyPart;

    BodyParts(String str){
        typeOfBodyPart = str;
    }
    BodyParts(){
        typeOfBodyPart = Adjective.UNKNOWN.getValue();
    }

    public String getTypeOfBodyPart(){
        return typeOfBodyPart;
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        BodyParts obj2 = (BodyParts) obj;
        return this.typeOfBodyPart.equals(obj2.typeOfBodyPart) && this.getTypeOfBodyPart() != null;
    }
    @Override
    public int hashCode(){
        final int prime = 31;
        int result = 1;
        result = prime * result + ((this.getTypeOfBodyPart() == null) ? 0: this.getTypeOfBodyPart().hashCode()) + this.getClass().hashCode()*(13);
        return result;
    }
    @Override
    public String toString() {
        return (this.getClass().getSimpleName() + " -> type of body part: " + this.getTypeOfBodyPart());
    }

}
