package attacks;

import ru.ifmo.se.pokemon.*;


public class Acid extends SpecialMove {
    public Acid(){
        super(Type.POISON, 40, 100);
    }

    @Override
    protected String describe(){
        return "заставляет противника киснуть";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
             p.addEffect(new Effect().turns(-1).stat(Stat.SPECIAL_DEFENSE, (int) (p.getStat(Stat.SPECIAL_DEFENSE)*0.9)));
    }
}