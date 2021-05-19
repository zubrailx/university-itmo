package com.company.human;

import com.company.enums.Adjective;
import com.company.enums.Location;

public abstract class AdvancedHumanWithBodyParts extends AdvancedHuman {

    public AdvancedHumanWithBodyParts(String name, Location location){
        super(name, location);
    }
    public AdvancedHumanWithBodyParts(String name){
        super(name);
    }
    public AdvancedHumanWithBodyParts(){
        super();
    }


    public class Cheeks extends BodyPart {

        public Cheeks(){
            super("cheeks");
        }
        public Cheeks(Adjective ... adj){
            super("cheeks", adj);
        }
    }

    public class Eyes extends BodyPart{
        public Eyes(){
            super("eyes");
        }
        public Eyes(Adjective ... adj){
            super("eyes", adj);
        }
    }

    public class Face extends BodyPart{
        public Face(){
            super("face");
        }
        public Face(Adjective ... adj){
            super("face", adj);
        }
    }

    public class Hair extends BodyPart{
        public Hair(){
            super("hair");
        }
        public Hair(Adjective ... adj){
            super("hair",adj);
        }
    }

    public class Lips extends BodyPart{
        public Lips(){
            super("lips");
        }
        public Lips(Adjective ... adj){
            super("lips", adj);
        }
    }
}
