package attacks;

import ru.ifmo.se.pokemon.*;

public class Twister extends SpecialMove {
    public Twister(){
        super(Type.DRAGON, 40, 100);
    }
    @Override
    protected String describe(){
        return "сдувает противника";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
        if(Math.random() < 0.3)
            Effect.flinch(p);
        else
            super.applyOppEffects(p);

    }
}