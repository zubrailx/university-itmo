package chars;

import attacks.*;
import ru.ifmo.se.pokemon.Pokemon;
import ru.ifmo.se.pokemon.Type;

public class Farfetchd extends Pokemon {

    public Farfetchd(String name, int value){
        super(name, value);
        this.setStats(52.0,90.0,55.0,58.0,62.0,60.0);
        this.setType(Type.NORMAL, Type.FLYING);
        setMove(new Leaf_Blade(), new Peck(), new Night_Slash(),new Swagger());
    }

}
