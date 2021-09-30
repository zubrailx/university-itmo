package chars;

import attacks.*;
import ru.ifmo.se.pokemon.Type;

public class Victreebel extends Weepinbell {

    public Victreebel(String name, int value){
        super(name, value);
        this.setStats(80.0,105.0,65.0,100.0,70.0,70.0);
        this.setType(Type.POISON, Type.GRASS);
        setMove(new Sludge_Bomb(), new Swagger(), new Poison_Powder(), new Facade());
    }

}
