package attacks;

import ru.ifmo.se.pokemon.*;

public class Night_Slash extends PhysicalMove {
    public Night_Slash(){
        super(Type.DARK, 70, 100);
    }

    @Override
    protected String describe(){
        return "скретчит противника";
    }

    @Override
    protected double calcCriticalHit(Pokemon p1, Pokemon p2) {
        p1.addEffect(new Effect().turns(1).stat(Stat.SPEED,(int)p1.getStat(Stat.SPEED)*3));
        return super.calcCriticalHit(p1,p2);
    }
}