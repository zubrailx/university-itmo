package chars;

import attacks.*;
import attacks.Sludge_Wave;
import ru.ifmo.se.pokemon.Type;

public class Dragalge extends Skrelp {

    public Dragalge(String name, int value){
        super(name, value);
        this.setStats(65.0,75.0,90.0,97.0,123.0,44.0);
        this.setType(Type.POISON, Type.WATER);
        setMove(new Sludge_Wave(), new Acid(), new Water_Gun(), new Twister());
    }

}
