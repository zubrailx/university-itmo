package attacks;

import ru.ifmo.se.pokemon.*;

public class Poison_Powder extends StatusMove {
    public Poison_Powder(){
        super(Type.POISON, 0, 75);
    }
    @Override
    protected String describe(){
        return "расбрасывает ядовитый порошок";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
            Effect.poison(p);
    }
}