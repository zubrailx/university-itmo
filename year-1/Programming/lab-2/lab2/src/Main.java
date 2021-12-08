import chars.*;
import ru.ifmo.se.pokemon.Battle;
import ru.ifmo.se.pokemon.Pokemon;

public class Main {
    public static void main(String[] args) {
        Battle b = new Battle();

        Pokemon[] AllyPokemon = {new Bellsprout("Bell", 2), new Dragalge("Drag", 3), new Farfetchd("Far", 5)};
        Pokemon[] EnemyPokemon = {new Weepinbell("Wip", 10)};

        if (AllyPokemon.length > 0 && EnemyPokemon.length > 0){
            for (Pokemon p : AllyPokemon){
                b.addAlly(p);
            }
            for (Pokemon p : EnemyPokemon){
                b.addFoe(p);
            }
            b.go();
        }
        else System.out.println("Хотя бы в одной из команд нет покемонов");
    }
}

