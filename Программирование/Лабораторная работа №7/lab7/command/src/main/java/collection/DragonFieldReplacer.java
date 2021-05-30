package collection;

import java.io.PrintStream;
import java.util.Locale;

public class DragonFieldReplacer {
    public static Dragon replaceIfLower(Dragon source, String[] field, String[] value, PrintStream printStream) throws IllegalArgumentException { //req-element = field[i] , value[i]
        if (field.length != value.length) throw new IllegalArgumentException();
        for (int i = 0; i < field.length; i++) {
            if (DragonFieldValidation.validate(field[i], value[i]) && !value[i].equals("")) {
                switch (field[i]) {
                    case "name": {
                        if (value[i].compareTo(source.getName()) < 0) {
                            source.setName(value[i]);
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;
                    }
                    case "coordinate_x": {
                        if (Double.parseDouble(value[i]) < source.getCoordinates().getX()) {
                            source.setCoordinateX(Double.valueOf(value[i]));
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;

                    }
                    case "coordinate_y": {
                        if (Integer.parseInt(value[i]) < source.getCoordinates().getY()) {
                            source.setCoordinateY(Integer.parseInt(value[i]));
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;
                    }
                    case "age": {
                        if (Long.parseLong(value[i]) > 0 && Long.parseLong(value[i]) < source.getAge()) {
                            source.setAge(Long.parseLong(value[i]));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "color": {
                        if (Color.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getColor()) < 0) {
                            source.setColor(Color.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "type": {
                        if (DragonType.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getType()) < 0) {
                            source.setType(DragonType.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "character": {
                        if (DragonCharacter.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getCharacter()) < 0) {
                            source.setCharacter(DragonCharacter.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "cave_depth": {
                        if (Double.parseDouble(value[i]) < source.getCave().getDepth()) {
                            source.setCaveDepth(Double.parseDouble(value[i]));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                }
            } else {
                printStream.println("Значение поля не было изменено: " + field[i] + " : " + (value[i].equals("") ? "null" : value[i]));
            }
        }
        printStream.println("Значение указанных полей было изменено.");
        return source;
    }

    public static Dragon replaceIfGreater(Dragon source, String[] field, String[] value, PrintStream printStream) throws IllegalArgumentException { //req-element = field[i] , value[i]
        if (field.length != value.length) throw new IllegalArgumentException();
        for (int i = 0; i < field.length; i++) {
            if (DragonFieldValidation.validate(field[i], value[i]) && !value[i].equals("")) {
                switch (field[i]) {
                    case "name": {
                        if (value[i].compareTo(source.getName()) > 0) {
                            source.setName(value[i]);
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;
                    }
                    case "coordinate_x": {
                        if (Double.parseDouble(value[i]) > source.getCoordinates().getX()) {
                            source.setCoordinateX(Double.valueOf(value[i]));
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;

                    }
                    case "coordinate_y": {
                        if (Integer.parseInt(value[i]) > source.getCoordinates().getY()) {
                            source.setCoordinateY(Integer.parseInt(value[i]));
                        } else {
                            printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        }
                        break;
                    }
                    case "age": {
                        if (Long.parseLong(value[i]) > 0 && Long.parseLong(value[i]) > source.getAge()) {
                            source.setAge(Long.parseLong(value[i]));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "color": {
                        if (Color.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getColor()) > 0) {
                            source.setColor(Color.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "type": {
                        if (DragonType.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getType()) > 0) {
                            source.setType(DragonType.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "character": {
                        if (DragonCharacter.valueOf(value[i].toUpperCase(Locale.ROOT)).compareTo(source.getCharacter()) > 0) {
                            source.setCharacter(DragonCharacter.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                    case "cave_depth": {
                        if (Double.parseDouble(value[i]) > source.getCave().getDepth()) {
                            source.setCaveDepth(Double.parseDouble(value[i]));
                        } else printStream.println("Значение поля не было изменено: " + field[i] + " : " + value[i]);
                        break;
                    }
                }
            } else {
                printStream.println("Значение поля не было изменено: " + field[i] + " : " + (value[i].equals("") ? "null" : value[i]));
            }
        }
        printStream.println("Значение указанных полей было изменено.");
        return source;
    }

    public static Dragon update(Dragon source, String[] field, String[] value, PrintStream printStream) throws IllegalArgumentException { //req-element = field[i] , value[i]
        if (field.length != value.length) throw new IllegalArgumentException();
        for (int i = 0; i < field.length; i++) {
            if (DragonFieldValidation.validate(field[i], value[i])) {
                switch (field[i]) {
                    case "name": {
                        source.setName(value[i]);
                        break;
                    }
                    case "coordinate_x": {
                        source.setCoordinateX(Double.valueOf(value[i]));
                        break;
                    }
                    case "coordinate_y": {
                        source.setCoordinateY(Integer.parseInt(value[i]));
                        break;
                    }
                    case "age": {
                        if (value[i].equals("")) {
                            source.setAge(null);
                        } else {
                            source.setAge(Long.parseLong(value[i]));
                        }
                        break;
                    }
                    case "color": {
                        source.setColor(Color.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        break;
                    }
                    case "type": {
                        source.setType(DragonType.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        break;
                    }
                    case "character": {
                        source.setCharacter(DragonCharacter.valueOf(value[i].toUpperCase(Locale.ROOT)));
                        break;
                    }
                    case "cave_depth": {
                        if (value[i].equals("")) {
                            source.setCave(new DragonCave(null));
                        } else {
                            source.setCaveDepth(Double.parseDouble(value[i]));
                        }
                        break;
                    }
                }
            }
        }
        printStream.println("Значение указанных полей было изменено.");
        return source;
    }
}
