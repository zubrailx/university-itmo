package com.company.human;

import com.company.enums.*;
import com.company.exceptions.SomeSameObjectsException;

public class WoodenWoman extends AdvancedHumanWithBodyParts{

    public static int id = 0;
    {
        id++;
        if (id > 1){
            try {
                throw new SomeSameObjectsException("There can't be more than 1 Wooden Woman", id);
            }catch(SomeSameObjectsException ex){
                System.out.println("-----------------------------------------------------------\n" + ex.getMessage());
                System.out.println("Your number of objects: " + ex.getValue()+"\n-----------------------------------------------------------");
            }
        }
    }

    private final String height = Adjective.BIG.getValueOfAdjective();
    private final String kindOfBody = Adjective.WOODEN.getValueOfAdjective();

    public WoodenWoman(){
        super("Wooden Woman", Location.WATER);
    }

    public Cheeks cheeks = new Cheeks(Adjective.ROSY);
    public Eyes eyes = new Eyes(Adjective.ROUND, Adjective.WIDELY_OPENED, Adjective.BLUE);
    public Face face = new Face(Adjective.SMILING);
    public Hair hair = new Hair(Adjective.BLUE, Adjective.LONG);
    public Lips lips = new Lips(Adjective.ROSY);

    public String getHeight() {
        return height;
    }
    public String getKindOfBody(){
        return kindOfBody;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == this) return true;
        if (obj == null || obj.getClass() != this.getClass()) return false;
        WoodenWoman objH = (WoodenWoman) obj;
        return this.getName().equals(objH.getName()) && this.getName() != null && this.getLocation().equals(objH.getLocation())
                && this.getHeight().equals(objH.getHeight()) && this.getKindOfBody().equals(objH.getKindOfBody());
    }

    @Override
    public int hashCode() {
        final int prime = 21;
        int result = 1;
        result = prime * result + ((this.getName() == null) ? 0: this.getName().hashCode()) + ((this.getLocation() == null) ? 0: this.getLocation().hashCode()*3) +
                ((this.getKindOfBody() == null)? 0: this.getKindOfBody().hashCode())*5 + ((this.getHeight() == null) ? 0: this.getHeight().hashCode())*7;
        return result;
    }

    @Override
    public String toString() {
        return "AdvancedHumanWithBodyParts." + this.getName() + ", location: " + this.getLocation() + ", height: " + this.getHeight() + ", kindOfBody: " + this.getKindOfBody();
    }
}
