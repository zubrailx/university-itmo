package collection;

import file.FileManager;
import file.XmlParser;

import java.io.PrintStream;
import java.time.Instant;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;

/**
 * Менеджер коллекции - класс отвечающий за работу с коллекциями.
 */
public class CollectionManager {

    /**
     * Коллекция, над которой осуществляется работа.
     */
    TreeMap<Integer, Dragon> treeMap;

    /**
     * Время инициализации коллекции.
     */
    ZonedDateTime collectionInitialization;

    /**
     * Конструктор - создание нового объекта менеджера коллекции.
     */
    public CollectionManager() {
        this.treeMap = new TreeMap<>();
        String i = Instant.now().toString();
        collectionInitialization = ZonedDateTime.parse(i);
    }

    public CollectionManager(Dragon[] dragons) {
        this.treeMap = new TreeMap<>();
        Arrays.stream(dragons).forEach(dragon -> treeMap.put(dragon.getId(), dragon));
        String i = Instant.now().toString();
        collectionInitialization = ZonedDateTime.parse(i);
    }

    /**
     * Метод, возвращающий названия всех полей коллекции, которые могут быть изменены/получены.
     *
     * @return String - строка, содержащая все поля коллекции. Отформатированно выводит в столбец.
     */
    public static String getFieldNames() {
        return "Список всех полей:\nname(String)\ncoordinate_x(Double)\ncoordinate_y(int)\n" +
                "age(Long)\ncolor: " + Arrays.toString(Color.values()) + "\ntype: "
                + Arrays.toString(DragonType.values()) + "\ncharacter: " + Arrays.toString(DragonCharacter.values()) + "\ncave_depth(Double)\n";
    }

    /**
     * Метод, выводящий основную информацию по используемой коллекции.
     *
     * @return String - результат работы программы.
     */
    public String info() {
        StringBuilder sb = new StringBuilder();
        sb.append("Коллекция ").append(treeMap.getClass().getSimpleName()).append("\n");
        sb.append("Тип элементов коллекции: ").append(Dragon.class.getSimpleName()).append("\n");

        String pattern = "yyyy-MM-dd HH:mm:ss.SSS";
        DateTimeFormatter europeanDateFormatter = DateTimeFormatter.ofPattern(pattern);
        sb.append("Время ининициализации коллекции: ").append(collectionInitialization.plusHours(3).format(europeanDateFormatter)).append("\n");
        sb.append("Количество элементов в коллекции: ").append(treeMap.size()).append("\n");
        return sb.toString();
    }

    /**
     * Метод, выводящий информацию по элементам коллекции.
     *
     * @return String - результат работы программы.
     */
    public String show() {
        StringBuilder sb = new StringBuilder();
        if (treeMap.size() == 0) {
            sb.append("Коллекция пуста.");
        } else {
            List<Dragon> dragonByName = new ArrayList<>(treeMap.values());
            dragonByName.sort(Comparator.comparing(Dragon::getName).thenComparing(Dragon::getId));

            dragonByName.forEach(dragon -> sb.append(dragon).append("\n"));
        }
        return sb.toString();
    }

    /**
     * Метод, добавляющий новый элемент в коллекцию
     *
     * @param id          уникальный идентицикатор элемента коллекции (ключ).
     * @param dragon      элемент коллекции, требующий добавления.
     * @param printStream поток вывода.
     */
    public void insert(Integer id, Dragon dragon, PrintStream printStream) {
        if (treeMap.get(id) == null) {
            treeMap.put(id, dragon);
        } else printStream.printf("Элемент с ключом %d уже существует%n", id);
    }

    /**
     * Метод, удаляющий выбранный по идентификатору элемент коллекции.
     *
     * @param id уникальный идентификатор элемента коллекции (ключ).
     */
    public void removeKey(Integer id) {
        treeMap.remove(id);
    }

//    /**
//     * Метод, удаляющий все элементы коллекции, значение идентификатора которых меньше указанного.
//     *
//     * @param id значение идентификатора, меньше которого следует удалять элементы.
//     */
//    public void removeLowerKey(Integer id) {
//        ArrayList<Integer> keys = new ArrayList<>();
//
//        treeMap.entrySet().stream().filter(mapEntry -> mapEntry.getKey() < id)
//                .forEach(mapEntry -> keys.add(mapEntry.getKey()));
//
//        keys.forEach(treeMap::remove);
//
//    }

    /**
     * Метод, сохраняющий элементы коллекции в формате XML.
     *
     * @param filePath путь до файла, куда следует сохранить элементы коллекции.
     */
    public void save(String filePath) {
        XmlParser xmlParser = new XmlParser();
        FileManager fileManager = new FileManager();

        Dragon[] dragons = new Dragon[treeMap.size()];
        dragons = treeMap.values().toArray(dragons);
        String str = xmlParser.parseToXml(dragons);
        fileManager.writeToFile(str, filePath);
    }
//    /**
//     * Метод, удаляющий случайный элемент коллекции, цвет которого соответствует выбранному.
//     *
//     * @param color       выбранный цвет элемента коллекции.
//     * @param printStream поток вывода.
//     */
//    public void removeAnyByColor(Color color, PrintStream printStream) {
//        Optional<Integer> key = treeMap.entrySet().stream()
//                .filter(entrySet -> entrySet.getValue().getColor().equals(color)).map(Map.Entry::getKey).findFirst();
//        if (key.isPresent()) {
//            treeMap.remove(key.get());
//            printStream.println("Элемент был удален.");
//        }
//    }

    public Integer[] getLowerKeys(Integer id) {
        ArrayList<Integer> keys = new ArrayList<>();

        treeMap.entrySet().stream().filter(mapEntry -> mapEntry.getKey() < id)
                .forEach(mapEntry -> keys.add(mapEntry.getKey()));

        return keys.toArray(new Integer[0]);
    }

    /**
     * Метод, выводящий среднее значение возрастов всех элементов коллекции.
     *
     * @return Double - среднее значение возрастов по всем элементам коллекции.
     */
    public Double averageAge() {

        int size = treeMap.size();

        Optional<Long> optional = treeMap.values().stream().map(Dragon::getAge).filter(Objects::nonNull).reduce(Long::sum);

        return optional.map(aLong -> (double) aLong / size).orElse(0.0);
    }

//    /**
//     * Метод, выводящий булевый результат истины, если в коллекции существует элемент с выбранным ключом, иначе ложь.
//     *
//     * @param id уникальный идентификатор элемента коллекции (ключ).
//     * @return boolean. true - в коллекции существует элемент с выбранным ключом, false - такого элемента не существует.
//     */
//    public boolean containsKey(Integer id) {
//        return treeMap.containsKey(id);
//    }

    /**
     * Метод, возращающий объект (элемент) коллекции, значение age которого максимально.
     *
     * @return Dragon - элемент коллекции, значение age которого максимально.
     */
    public Dragon maxAge() {

        Optional<Dragon> optional = treeMap.values().stream().filter(dragon -> dragon.getAge() != null).max(Comparator.comparing(Dragon::getAge));

        return optional.orElse(null);

    }
}