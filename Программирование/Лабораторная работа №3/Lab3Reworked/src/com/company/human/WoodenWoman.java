package com.company.human;

import com.company.bodyParts.*;
import com.company.enums.Adjective;

public class WoodenWoman extends AdvancedHuman {

    private String height;
    private String womanIs;

    {
        height = Adjective.BIG.getValue();
        womanIs = Adjective.WOODEN.getValue();
    }

    public WoodenWoman(){
        super("Weird Object");
    }
    public WoodenWoman(String location){
        super("Wooden Woman", location);
    }

    public String getHeight(){
        return height;
    }
    public String getWomanIs(){
        return womanIs;
    }
    public String getCheeks(Cheeks obj){return obj.getTypeOfBodyPart() + " cheeks";}
    public String getEyes(Eyes obj){return obj.getTypeOfBodyPart() + " eyes";}
    public String getFace(Face obj){return obj.getTypeOfBodyPart() + " face";}
    public String getHair(Hair obj){return obj.getTypeOfBodyPart() + " hair";}
    public String getLips(Lips obj){return obj.getTypeOfBodyPart() + " lips";}

}
