package attacks;

import ru.ifmo.se.pokemon.*;

public class Peck extends PhysicalMove {
    public Peck(){
        super(Type.FLYING, 35, 100);
    }
    @Override
    protected String describe(){
        return "клюет противника";
    }

}