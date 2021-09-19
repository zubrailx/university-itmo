package attacks;

import ru.ifmo.se.pokemon.*;

    public class Leaf_Blade extends PhysicalMove {
        public Leaf_Blade(){
            super(Type.GRASS, 90, 100);
        }

        @Override
        protected String describe(){
            return "царапает противника";
        }

        @Override
        protected double calcCriticalHit(Pokemon p1, Pokemon p2) { //увеличение скорости атаки в 3 раза равно увеличению крита в 3 раза
            p1.addEffect(new Effect().turns(1).stat(Stat.SPEED,(int)p1.getStat(Stat.SPEED)*3));
            return calcCriticalHit(p1,p2);
        }
    }

    