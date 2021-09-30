package com.company.human;

import com.company.enums.*;
import com.company.exceptions.UnknownBodyPartWhenContainsOtherAdjectivesException;

import java.util.Arrays;

public abstract class BodyPart {
    private final Adjective [] typeOfBodyPart;
    private final String bodyPart;

    public BodyPart(String bodyPart, Adjective ... adj) {
        this.bodyPart = bodyPart;

        typeOfBodyPart = new Adjective[adj.length];

        for (int i = 0; i < adj.length; i++) {
            typeOfBodyPart[i] = adj[i];
        }

        if(typeOfBodyPart.length > 1){
            try{
                for (Adjective a : typeOfBodyPart){
                    if (a == Adjective.UNKNOWN) throw new UnknownBodyPartWhenContainsOtherAdjectivesException("\"Unknown\" adjective when others when there are other ones", typeOfBodyPart);
                }
            }catch(UnknownBodyPartWhenContainsOtherAdjectivesException ex){
                System.out.println(ex.getMessage());
                for (int i = 0; i < ex.getAdjective().length; i++){
                    System.out.println(ex.getAdjective()[i]);
                }
            }
        }
    }
    public BodyPart(String bodyPart){
        typeOfBodyPart = new Adjective[1];
        typeOfBodyPart[0] = Adjective.UNKNOWN;

        this.bodyPart = bodyPart;
    }

    public Adjective[] getTypeOfBodyPart(){
        return typeOfBodyPart;
    }

    public String getTypeOfBodyPartAsString(){
        StringBuilder sb = new StringBuilder();
        for (int i=0; i < getTypeOfBodyPart().length; i++){
            sb.append(typeOfBodyPart[i].getValueOfAdjective());
            if (i < this.getTypeOfBodyPart().length - 1){
                sb.append(", ");
            }
        }
        return sb.toString() + " "+ this.getBodyPart();
    }

    private String getBodyPart() {
        return bodyPart;
    }

    @Override
    public boolean equals(Object obj){
        if (obj == this){
            return true;
        }
        if(obj == null || obj.getClass() != this.getClass()){
            return false;
        }
        BodyPart obj2 = (BodyPart) obj;
        return Arrays.equals(this.getTypeOfBodyPart(), obj2.getTypeOfBodyPart()) && this.getTypeOfBodyPart() != null;
    }
    @Override
    public int hashCode(){
        final int prime = 31;
        int result = 1;
        result = prime * result + ((this.getTypeOfBodyPart() == null) ? 0: Arrays.hashCode(this.getTypeOfBodyPart())) + this.getClass().hashCode()*(13);
        return result;
    }
    @Override
    public String toString() {
        return (this.getBodyPart() + " -> type of body part: " + Arrays.toString(this.getTypeOfBodyPart()));
    }
}
