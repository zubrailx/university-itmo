package file;

import collection.*;
import exceptions.ValidValuesRangeException;
import io.UserIO;

import java.time.Instant;
import java.time.ZonedDateTime;

public class DragonFieldsReader {
    /**
     * поле, которое хранит ссылку на объект типа UserIO
     */
    private UserIO userIO;

    /**
     * Конструктор класса, который присваивает в поле userIO значение, переданное в конструкторе в качестве параметра
     *
     * @param userIO хранит ссылку на объект типа UserIO
     */
    public DragonFieldsReader(UserIO userIO) {
        this.userIO = userIO;
    }

    /**
     * Метод, выводящий производящий чтение данных из консоли. Запрашивает ввод полей в строго определенном порядке.
     *
     * @param id уникальный идентификатор объекта класса Dragon, который должен быть записан в качестве ключа в коллекцию
     * @return возращает объект типа Dragon
     */
    public Dragon read(Integer id) {

        String i = Instant.now().toString();
        return new Dragon(id, readName(), readCoordinates(), readColor(), readAge(), readType(), readCharacter(), readCave(), ZonedDateTime.parse(i));
    }

    /**
     * Метод, производящий чтение поля name типа String объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поля заново.
     *
     * @return значение поля name, уже проверенное на недопустимую ОДЗ.
     */
    public String readName() {
        String str;

        while (true) {
            userIO.printCommandText("name (not null): ");
            str = userIO.readLine().trim();
            if (str.equals("")) userIO.printCommandError("\nЗначение поля не может быть null или пустой строкой \n");
            else return str;
        }
    }

    /**
     * Метод, производящий чтение координат x, y.
     *
     * @return возвращает объект типа Coordinates.
     */
    public Coordinates readCoordinates() {
        return new Coordinates(readCoordinateX(), readCoordinateY());
    }

    /**
     * Метод, производящий чтение поля x типа Double объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново.
     *
     * @return значение поля x, уже проверенное на недопустимую ОДЗ.
     */
    public Double readCoordinateX() {
        Double x;
        while (true) {
            try {
                userIO.printCommandText("coordinate_x (double & not null & x > -648): ");
                x = Double.parseDouble(userIO.readLine().trim());
                if (x <= -648) throw new ValidValuesRangeException();
                else return x;
            } catch (ValidValuesRangeException ex) {
                System.out.println("Координата x должна быть больше -648");
            } catch (NumberFormatException ex) {
                System.err.println("Число должно быть типа Double и не null");
            }
        }
    }

    /**
     * Метод, производящий чтение поля y типа int объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново.
     *
     * @return значение поля y, уже проверенное на недопустимую ОДЗ.
     */
    public int readCoordinateY() {
        int y;
        while (true) {
            try {
                userIO.printCommandText("coordinate_y (int & can be null & y < 803): ");
                String str = userIO.readLine().trim();
                if (str.equals("")) y = 0;
                else {
                    y = Integer.parseInt(str);
                    if (y >= 803) throw new ValidValuesRangeException();
                }
                return y;
            } catch (ValidValuesRangeException ex) {
                System.out.println("Координата y должна быть меньше 803");
            } catch (NumberFormatException ex) {
                System.err.println("Число должно быть типа int");
            }
        }
    }

    /**
     * Метод, производящий чтение поля age типа Long объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново.
     *
     * @return значение поля age, уже проверенное на недопустимую ОДЗ.
     */
    public Long readAge() {
        Long age;
        while (true) {
            try {
                userIO.printCommandText("age (long & can be null & age > 0): ");
                String str = userIO.readLine().trim();
                if (str.equals("")) age = null;
                else {
                    age = Long.parseLong(str);
                    if (age <= 0) throw new ValidValuesRangeException();
                }
                return age;
            } catch (ValidValuesRangeException ex) {
                System.err.println("Значение age должно быть больше 0");
            } catch (NumberFormatException ex) {
                System.err.println("Число должно быть типа Long");
            }
        }
    }

    /**
     * Метод, производящий чтение поля color типа Color объекта Dragon из потока, указанного в поле userIO.
     * При некорректном вводе просит ввести поле заново. Список перечисляемых констант выводится
     *
     * @return значение поля x, уже проверенное на недопустимую ОДЗ.
     */
    public Color readColor() {
        Color color;
        while (true) {
            try {
                userIO.printCommandText("Допустимые значения color :\n");
                for (Color val : Color.values()) {
                    userIO.printCommandText(val.name() + "\n");
                }
                userIO.printCommandText("color: ");
                color = Color.valueOf(userIO.readLine().toUpperCase().trim());
                return color;
            } catch (IllegalArgumentException ex) {
                System.err.println("Значение введенной константы не представлено в перечислении Color");
            }
        }
    }

    /**
     * Метод, производящий чтение поля type типа DragonType объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново. Список перечисляемыъ констант выводится.
     *
     * @return значение поля type, уже проверенное на недопустимую ОДЗ.
     */
    public DragonType readType() {
        DragonType dragonType;
        while (true) {
            try {
                userIO.printCommandText("Допустимые значения type :\n");
                for (DragonType val : DragonType.values()) {
                    userIO.printCommandText(val.name() + "\n");
                }
                userIO.printCommandText("type: ");
                dragonType = DragonType.valueOf(userIO.readLine().toUpperCase().trim());
                return dragonType;
            } catch (IllegalArgumentException ex) {
                System.err.println("Значение введенной константы не представлено в перечислении DragonType");
            }
        }
    }

    /**
     * Метод, производящий чтение поля character типа DragonCharacter объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново. Список перечисляемых констант выводится.
     *
     * @return значение поля character, уже проверенное на недопустимую ОДЗ.
     */
    public DragonCharacter readCharacter() {
        DragonCharacter dragonCharacter;
        while (true) {
            try {
                userIO.printCommandText("Допустимые значения character :\n");
                for (DragonCharacter val : DragonCharacter.values()) {
                    userIO.printCommandText(val.name() + "\n");
                }
                userIO.printCommandText("character: ");
                dragonCharacter = DragonCharacter.valueOf(userIO.readLine().toUpperCase().trim());
                return dragonCharacter;
            } catch (IllegalArgumentException ex) {
                System.err.println("Значение введенной константы не представлено в перечислении DragonCharacter");
            }
        }
    }

    /**
     * Метод, производящий чтение поля depth объекта Dragon из потока, указанного в поле userIO. При некорректном вводе просит ввести поле заново.
     *
     * @return объект типа DragonCave.
     */
    public DragonCave readCave() {
        Double depth;
        while (true) {
            try {
                userIO.printCommandText("cave_depth (double & can be null): ");
                String str = userIO.readLine().trim();
                if (str.equals("")) depth = null;
                else {
                    depth = Double.parseDouble(str);
                }
                return new DragonCave(depth);
            } catch (NumberFormatException ex) {
                System.err.println("Число должно быть типа double");
            }
        }
    }
}
