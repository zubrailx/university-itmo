package chars;

import attacks.*;
import ru.ifmo.se.pokemon.Pokemon;
import ru.ifmo.se.pokemon.Type;

public class Bellsprout extends Pokemon {

    public Bellsprout(String name, int value){
        super(name, value);
        this.setStats(50.0,75.0,35.0,70.0,30.0,40.0);
        this.setType(Type.POISON, Type.GRASS);
        setMove(new Sludge_Bomb(), new Swagger());
    }

}
