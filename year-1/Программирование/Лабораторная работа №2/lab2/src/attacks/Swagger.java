package attacks;

import ru.ifmo.se.pokemon.*;


public class Swagger extends StatusMove {
    public Swagger(){
        super(Type.NORMAL, 0, 85);
    }
    @Override
    protected String describe(){
        return "расхаживается с важным видом перед противником";
    }

    @Override
    protected void applyOppEffects(Pokemon p){
        Effect.confuse(p);
    }
}