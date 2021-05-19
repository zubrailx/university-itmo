package chars;

import attacks.*;
import ru.ifmo.se.pokemon.Type;

public class Weepinbell extends Bellsprout {

    public Weepinbell(String name, int value){
        super(name, value);
        this.setStats(65.0,90.0,50.0,85.0,45.0,55.0);
        this.setType(Type.POISON, Type.GRASS);
        setMove(new Sludge_Bomb(), new Swagger(), new Poison_Powder());
    }

}

