package collection;

import file.FileManager;
import file.XmlParser;

import java.time.Instant;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;

/**
 * Менеджер коллекции - класс отвечающий за работу с коллекциями
 */
public class CollectionManager {
    /**
     * Коллекция, над которой осуществляется работа
     */
    TreeMap<Integer, Dragon> treeMap;
    /**
     * Время инициализации коллекции
     */
    ZonedDateTime collectionInitialization;

    /**
     * Конструктор - создание нового объекта менеджера коллекции
     */
    public CollectionManager() {
        this.treeMap = new TreeMap<>();
        String i = Instant.now().toString();
        collectionInitialization = ZonedDateTime.parse(i);
    }

    /**
     * Метод, выводящий основную информацию по используемой коллекции
     */
    public void info() {
        System.out.println("Коллекция " + treeMap.getClass().getSimpleName());
        System.out.println("Тип элементов коллекции: " + Dragon.class.getSimpleName());

        String pattern = "yyyy-MM-dd HH:mm:ss.SSS";
        DateTimeFormatter europeanDateFormatter = DateTimeFormatter.ofPattern(pattern);
        System.out.println("Время ининициализации коллекции: " + collectionInitialization.plusHours(3).format(europeanDateFormatter));
        System.out.println("Количество элементов в коллекции: " + treeMap.size());

    }

    /**
     * Метод, выводящий информацию по элементам коллекции
     */
    public void show() {
        if (treeMap.size() == 0) {
            System.out.println("Коллекция пуста.");
        } else {
            for (Map.Entry<Integer, Dragon> entry : treeMap.entrySet()) {
                System.out.println(entry.getValue().toString());
            }
        }
    }

    /**
     * Метод, добавляющий новый элемент в коллекцию
     *
     * @param id     уникальный идентицикатор элемента коллекции (ключ)
     * @param dragon элемент коллекции, требующий добавления
     */
    public void insert(Integer id, Dragon dragon) {
        if (treeMap.get(id) == null) {
            treeMap.put(id, dragon);
        } else System.out.println("Элемент с данным ключом уже существует");
    }

    /**
     * Метод, изменяющий поле выбранного по идентификатору элемента коллекции
     *
     * @param id    уникальный идентификатор элемента коллекции (ключ)
     * @param field имя поля элемента коллекции, требующее изменения
     * @param value значение поля элемента коллекции
     * @throws NullPointerException     исключение, выбрасываемое когда требует инициализации, но не инициалировано
     * @throws ClassCastException       исключение, выбрасываемое при попытке преобразовать один тип в другой, который не может быть получен из исходного
     * @throws IllegalArgumentException исключение, выбрасываемое при попытке передать методу недопустимые атрибуты
     */
    public void update(Integer id, String field, String value) {
        try {
            switch (field) {
                case "name": {
                    if (value.equals("")) throw new NullPointerException();
                    treeMap.get(id).setName(value);
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "coordinate_x": {
                    if (value.equals("")) value = null;
                    treeMap.get(id).setCoordinateX(Double.valueOf(value));
                    System.out.println("Значение поля было изменено");
                    break;

                }
                case "coordinate_y": {
                    if (value.equals("")) value = null;
                    treeMap.get(id).setCoordinateY(Integer.parseInt(value));
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "age": {
                    if (value.equals("")) {
                        treeMap.get(id).setAge(null);
                    } else {
                        treeMap.get(id).setAge(Long.parseLong(value));
                    }
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "color": {
                    treeMap.get(id).setColor(Color.valueOf(value.toUpperCase(Locale.ROOT)));
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "type": {
                    treeMap.get(id).setType(DragonType.valueOf(value.toUpperCase(Locale.ROOT)));
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "character": {
                    treeMap.get(id).setCharacter(DragonCharacter.valueOf(value.toUpperCase(Locale.ROOT)));
                    System.out.println("Значение поля было изменено");
                    break;
                }
                case "cave_depth": {
                    if (value.equals("")) {
                        treeMap.get(id).setCave(new DragonCave(null));
                    } else {
                        treeMap.get(id).setCaveDepth(Double.parseDouble(value));
                    }
                    System.out.println("Значение поля было изменено.");
                    break;
                }
                case "stop": {
                    break;
                }
                default: {
                    System.out.println("Поле не распознано");
                    break;
                }
            }
        } catch (ClassCastException ex) {
            System.err.println("Указано недопустимое значение для данного поля");
        } catch (IllegalArgumentException ex) {
            System.err.println("Значение аргумента не соответствует ожидаемому " + ex.getMessage());
        } catch (NullPointerException ex) {
            System.err.println("Невозможно записать null в данное поле");
        }
    }

    /**
     * Метод, удаляющий выбранный по идентификатору элемент коллекции
     *
     * @param id уникальный идентификатор элемента коллекции (ключ)
     */
    public void removeKey(Integer id) {
        treeMap.remove(id);
    }

    /**
     * Метод, удаляющий все элементы коллекции
     */
    public void clear() {
        treeMap.clear();
    }

    /**
     * Метод, сохраняющий элементы коллекции в формате XML
     *
     * @param filePath путь до файла, куда следует сохранить элементы коллекции
     */
    public void save(String filePath) {
        XmlParser xmlParser = new XmlParser();
        FileManager fileManager = new FileManager();

        Dragon[] dragons = new Dragon[treeMap.size()];
        dragons = treeMap.values().toArray(dragons);
        String str = xmlParser.parseToXml(dragons);
        fileManager.writeToFile(str, filePath);
    }

    /**
     * Метод, изменяющий поле выбранного по идентификатору элемента коллекции
     *
     * @param id    - уникальный идентификатор элемента коллекции (ключ)
     * @param field имя поля элемента коллекции, требующее изменения
     * @param value значение поля элемента коллекции
     * @throws NullPointerException     исключение, выбрасываемое когда требует инициализации, но не инициалировано
     * @throws ClassCastException       исключение, выбрасываемое при попытке преобразовать один тип в другой, который не может быть получен из исходного
     * @throws IllegalArgumentException исключение, выбрасываемое при попытке передать методу недопустимые атрибуты
     */
    public void replaceIfGreater(Integer id, String field, String value) {
        try {
            switch (field) {
                case "name": {
                    if (value.equals("")) value = null;
                    if (value.compareTo(treeMap.get(id).getName()) > 0) {
                        treeMap.get(id).setName(value);
                        System.out.println("Значение поля было изменено.");
                    } else {
                        System.out.println("Значение поля не было изменено, так как меньше");
                    }
                    break;
                }
                case "coordinate_x": {
                    if (value.equals("")) value = null;
                    if (Double.parseDouble(value) > treeMap.get(id).getCoordinates().getX()) {
                        if (Double.parseDouble(value) > -648) {
                            treeMap.get(id).setCoordinateX(Double.valueOf(value));
                            System.out.println("Значение поля было изменено");
                        } else System.err.println("Число не принадлежит ОДЗ");
                    } else {
                        System.out.println("Значение поля не было изменено, так как меньше");
                    }
                    break;

                }
                case "coordinate_y": {
                    if (value.equals("")) value = null;
                    if (Integer.parseInt(value) > treeMap.get(id).getCoordinates().getY()) {
                        treeMap.get(id).setCoordinateY(Integer.parseInt(value));
                        System.out.println("Значение поля было изменено");
                    } else {
                        System.out.println("Значение поля не было изменено, так как меньше");
                    }
                    break;
                }
                case "age": {
                    if (value.equals("")) value = null;
                    if (Long.parseLong(value) > 0 && Long.parseLong(value) > treeMap.get(id).getAge()) {
                        treeMap.get(id).setAge(Long.parseLong(value));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено, так как меньше");
                    break;
                }
                case "color": {
                    if (Color.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getColor()) > 0) {
                        treeMap.get(id).setColor(Color.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено, так как меньше");
                    break;
                }
                case "type": {
                    if (DragonType.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getType()) > 0) {
                        treeMap.get(id).setType(DragonType.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено, так как меньше");
                    break;
                }
                case "character": {
                    if (DragonCharacter.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getCharacter()) > 0) {
                        treeMap.get(id).setCharacter(DragonCharacter.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено, так как меньше");
                    break;
                }
                case "cave_depth": {
                    if (value.equals("")) value = null;
                    if (Double.parseDouble(value) > treeMap.get(id).getCave().getDepth()) {
                        treeMap.get(id).setCaveDepth(Double.parseDouble(value));
                        System.out.println("Значение поля было изменено, так как меньше");
                    }
                    break;
                }
            }
        } catch (ClassCastException ex) {
            System.err.println("Указано недопустимое значение для данного поля");
        } catch (IllegalArgumentException ex) {
            System.err.println("Значение аргумента не соответствует ожидаемому " + ex.getMessage());
        } catch (NullPointerException ex) {
            System.err.println("Значение полей null невозможно сравнивать");
        }
    }

    /**
     * Метод, изменяющий поле выбранного по идентификатору элемента коллекции
     *
     * @param id    уникальный идентификатор элемента коллекции (ключ)
     * @param field имя поля элемента коллекции, требующее изменения
     * @param value значение поля элемента коллекции
     * @throws NullPointerException     исключение, выбрасываемое когда требует инициализации, но не инициалировано
     * @throws ClassCastException       исключение, выбрасываемое при попытке преобразовать один тип в другой, который не может быть получен из исходного
     * @throws IllegalArgumentException исключение, выбрасываемое при попытке передать методу недопустимые атрибуты
     */
    public void replaceIfLower(Integer id, String field, String value) {
        try {
            switch (field) {
                case "name": {
                    if (value.equals("")) value = null;
                    if (value.compareTo(treeMap.get(id).getName()) < 0) {
                        treeMap.get(id).setName(value);
                        System.out.println("Значение поля было изменено.");
                    } else {
                        System.out.println("Значение поля не было изменено");
                    }
                    break;
                }
                case "coordinate_x": {
                    if (value.equals("")) value = null;
                    if (Double.parseDouble(value) < treeMap.get(id).getCoordinates().getX()) {
                        if (Double.parseDouble(value) > -648) {
                            treeMap.get(id).setCoordinateX(Double.valueOf(value));
                            System.out.println("Значение поля было изменено");
                        } else System.err.println("Число не принадлежит ОДЗ");
                    } else {
                        System.out.println("Значение поля не было изменено");
                    }
                    break;

                }
                case "coordinate_y": {
                    if (value.equals("")) value = null;
                    if (Integer.parseInt(value) < treeMap.get(id).getCoordinates().getY()) {
                        treeMap.get(id).setCoordinateY(Integer.parseInt(value));
                        System.out.println("Значение поля было изменено");
                    } else {
                        System.out.println("Значение поля не было изменено");
                    }
                    break;
                }
                case "age": {
                    if (value.equals("")) value = null;
                    if (Long.parseLong(value) > 0 && Long.parseLong(value) < treeMap.get(id).getAge()) {
                        treeMap.get(id).setAge(Long.parseLong(value));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено");
                    break;
                }
                case "color": {
                    if (Color.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getColor()) < 0) {
                        treeMap.get(id).setColor(Color.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено");
                    break;
                }
                case "type": {
                    if (DragonType.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getType()) < 0) {
                        treeMap.get(id).setType(DragonType.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено");
                    break;
                }
                case "character": {
                    if (DragonCharacter.valueOf(value.toUpperCase(Locale.ROOT)).compareTo(treeMap.get(id).getCharacter()) < 0) {
                        treeMap.get(id).setCharacter(DragonCharacter.valueOf(value.toUpperCase(Locale.ROOT)));
                        System.out.println("Значение поля было изменено");
                    } else System.out.println("Значение поля не было изменено");
                    break;
                }
                case "cave_depth": {
                    if (value.equals("")) value = null;
                    if (Double.parseDouble(value) < treeMap.get(id).getCave().getDepth()) {
                        treeMap.get(id).setCaveDepth(Double.parseDouble(value));
                        System.out.println("Значение поля было изменено.");
                    }
                    break;
                }
                case "": {
                    System.out.println("Uзменение полей завершено");
                    break;
                }
            }
        } catch (ClassCastException ex) {
            System.err.println("Указано недопустимое значение для данного поля");
        } catch (IllegalArgumentException ex) {
            System.err.println("Значение аргумента не соответствует ожидаемому " + ex.getMessage());
        } catch (NullPointerException ex) {
            System.err.println("Значение полей null невозможно сравнивать");
        }
    }

    /**
     * Метод, удаляющий все элементы коллекции, значение идентификатора которых меньше указанного
     *
     * @param id значение идентификатора, меньше которого следует удалять элементы
     */
    public void removeLowerKey(Integer id) {
        ArrayList<Integer> keys = new ArrayList<>();
        for (Map.Entry<Integer, Dragon> entry : treeMap.entrySet()) {
            if (entry.getKey() < id) keys.add(entry.getKey());
        }
        for (Integer key : keys) {
            treeMap.remove(key);
        }
    }

    /**
     * Метод, удаляющий случайный элемент коллекции, цвет которого соответствует выбранному
     *
     * @param color выбранный цвет элемента коллекции
     */
    public void removeAnyByColor(Color color) {
        int size = treeMap.size();
        for (Map.Entry<Integer, Dragon> entry : treeMap.entrySet()) {
            if (entry.getValue().getColor().equals(color)) {
                treeMap.remove(entry.getKey());
                break;
            }
        }
        if (size > treeMap.size()) System.out.println("Элемент был удален");
        else System.out.println("Не было найдено элементов с таким значением поля");
    }

    /**
     * Метод, выводящий среднее значение возрастов всех элементов коллекции
     *
     * @return среднее значение возрастов по всем элементам коллекции
     */
    public Double averageAge() {
        double sumAges = 0;
        int size = treeMap.size();
        for (Map.Entry<Integer, Dragon> entry : treeMap.entrySet()) {
            if (entry.getValue().getAge() != null) sumAges += (double) (entry.getValue().getAge() / size);
        }
        return sumAges;
    }

    /**
     * Метод, возращающий объект (элемент) коллекции, значение age которого максимально
     *
     * @return элемент коллекции, значение age которого максимально
     */
    public Dragon maxAge() {
        long maxAge = 0;
        try {
            Integer id = treeMap.firstKey();
            for (Map.Entry<Integer, Dragon> entry : treeMap.entrySet()) {
                if (entry.getValue().getAge() != null) {
                    if (entry.getValue().getAge() > maxAge) {
                        maxAge = entry.getValue().getAge();
                        id = entry.getKey();
                    }
                }
            }
            return treeMap.get(id);
        } catch (NoSuchElementException ignored) {
        }
        return null;
    }

    /**
     * Метод, выводящий булевый результат истины, если в коллекции существует элемент с выбранным ключом, иначе ложь
     *
     * @param id уникальный идентификатор элемента коллекции (ключ)
     * @return true - в коллекции существует элемент с выбранным ключом, false - такого элемента не существует
     */
    public boolean containsKey(Integer id) {
        return treeMap.containsKey(id);
    }

    /**
     * Метод, возвращающий названия всех полей коллекции, которые могут быть изменены/получены
     *
     * @return строка, содержащая все поля коллекции. Отформатированно выводит в столбец
     */
    public String getFieldNames() {
        return "Список всех полей:\nname(String)\ncoordinate_x(Double)\ncoordinate_y(int)\n" +
                "age(Long)\ncolor: " + Arrays.toString(Color.values()) + "\ntype: "
                + Arrays.toString(DragonType.values()) + "\ncharacter: " + Arrays.toString(DragonCharacter.values()) + "\ncave_depth(Double)\nu";
    }
}
