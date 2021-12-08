package attacks;

import ru.ifmo.se.pokemon.*;

public class Facade extends PhysicalMove {
    public Facade(){
        super(Type.NORMAL, 70, 100);
    }
    @Override
    protected String describe(){
        return "пинает противника";
    }
    @Override
    protected void applySelfEffects(Pokemon p) {
        if (p.getCondition() == Status.BURN || p.getCondition() == Status.POISON || p.getCondition() == Status.PARALYZE)
        {
            p.addEffect(new Effect().stat(Stat.ATTACK, (int) p.getStat(Stat.ATTACK) * 2));
        }
        else super.applySelfEffects(p);
    }

}