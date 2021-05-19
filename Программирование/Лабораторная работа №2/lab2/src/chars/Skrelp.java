package chars;

import attacks.*;
import ru.ifmo.se.pokemon.Pokemon;
import ru.ifmo.se.pokemon.Type;

public class Skrelp extends Pokemon {

    public Skrelp(String name, int value){
        super(name, value);
        this.setStats(50.0,60.0,60.0,60.0,60.0,30.0);
        this.setType(Type.POISON, Type.WATER);
        setMove(new Sludge_Wave(), new Acid(), new Water_Gun());
    }

}
