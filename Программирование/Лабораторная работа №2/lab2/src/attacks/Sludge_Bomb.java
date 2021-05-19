package attacks;

import ru.ifmo.se.pokemon.*;

public class Sludge_Bomb extends SpecialMove {
    public Sludge_Bomb(){
        super(Type.POISON, 90, 100);
    }
    @Override
    protected String describe(){
        return "бросает в противника бомбочку";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
        p.addEffect(new Effect().condition(Status.POISON).chance(0.3));
    }
}