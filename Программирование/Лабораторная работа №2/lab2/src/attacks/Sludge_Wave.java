package attacks;

import ru.ifmo.se.pokemon.*;

public class Sludge_Wave extends SpecialMove {
    public Sludge_Wave(){
        super(Type.POISON, 95, 100);
    }
    @Override
    protected String describe(){
        return "окисляет противника";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
        p.addEffect(new Effect().condition(Status.POISON).chance(0.1));
    }
}
