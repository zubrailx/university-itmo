package attacks;

import ru.ifmo.se.pokemon.*;

public class Water_Gun extends SpecialMove {
    public Water_Gun(){
        super(Type.WATER, 40, 100);
    }
    @Override
    protected String describe(){
        return "выстреливает в противника";
    }

}