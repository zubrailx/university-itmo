package collection;

import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

/**
 * Класс объектов(значений) коллекции.
 */
public class Dragon {
    /**
     * @value уникальный идентификатор, присваиваемый объекту коллекции
     */
    private static int uniqueId = 1;
    /**
     * уникальный идентификатор коллекции. Поле не может быть null, Значение поля должно быть больше 0, Значение этого поля должно быть уникальным, Значение этого поля генерируется автоматически
     */
    private Integer id;
    /**
     * имя объекта класса. Поле не может быть null, Строка не может быть пустой
     */
    private String name;
    /**
     * координаты объекта класса. Поле не может быть null.
     */
    private Coordinates coordinates;
    /**
     * время создания объекта класса. Поле не может быть null, Значение генерируется автоматически
     */
    private ZonedDateTime creationDate;
    /**
     * возраст объекта класса. Значение поля должно быть больше 0, Поле может быть null
     */
    private Long age;
    /**
     * цвет объекта класса. Поле может быть null
     */
    private Color color; //Поле не может быть null
    /**
     * тип объекта класса. Поле может быть null
     */
    private DragonType type;
    /**
     * характер объекта класса. Поле может быть null
     */
    private DragonCharacter character;
    /**
     * уровень пещеры объекта класса. Поле может быть null
     */
    private DragonCave cave;

    /**
     * Конструктор объекта класса.
     *
     * @param name            имя объекта класса
     * @param coordinates     координаты объекта класса
     * @param color           цвет объекта класса
     * @param dragonType      тип объекта класса
     * @param dragonCharacter характер объекта класса
     * @param dragonCave      уровень пещеры объекта класса
     * @param zonedDateTime   время создания объекта класса
     */
    public Dragon(String name, Coordinates coordinates, Color color, DragonType dragonType, DragonCharacter dragonCharacter, DragonCave dragonCave, ZonedDateTime zonedDateTime) {
        this.id = uniqueId++;
        this.name = name;
        this.coordinates = coordinates;
        this.color = color;
        this.type = dragonType;
        this.character = dragonCharacter;
        this.cave = dragonCave;
        this.creationDate = zonedDateTime;
    }

    /**
     * Конструктор объекта класса.
     *
     * @param id              уникальный идентификатор объекта коллекции
     * @param name            имя объекта класса
     * @param coordinates     координаты объекта класса
     * @param color           цвет объекта класса
     * @param age             возраст объекта класса
     * @param dragonType      тип объекта класса
     * @param dragonCharacter характер объекта класса
     * @param dragonCave      уровень пещеры объекта класса
     * @param zonedDateTime   время создания объекта класса
     */
    public Dragon(Integer id, String name, Coordinates coordinates, Color color, Long age, DragonType dragonType, DragonCharacter dragonCharacter, DragonCave dragonCave, ZonedDateTime zonedDateTime) {
        this.id = id;
        this.name = name;
        this.coordinates = coordinates;
        this.color = color;
        this.type = dragonType;
        this.character = dragonCharacter;
        this.cave = dragonCave;
        this.creationDate = zonedDateTime;
        this.age = age;
    }

    /**
     * Метод, возвращающий идентификатор объекта класса
     *
     * @return id - идентификатор объекта класса
     */
    public Integer getId() {
        return id;
    }

    /**
     * Метод, возвращающий имя объекта класса
     *
     * @return name - имя объекта класса
     */
    public String getName() {
        return name;
    }

    /**
     * Метод, возвращающий координаты объекта класса
     *
     * @return coordinates - координаты объекта класса
     */
    public Coordinates getCoordinates() {
        return coordinates;
    }

    /**
     * Метод, возвращающий время создания объекта класса
     *
     * @return creationDate - время создания объекта класса
     */
    public ZonedDateTime getCreationDate() {
        return creationDate;
    }

    /**
     * Метод, возвращающий отформатированное время создания объекта класса
     *
     * @return formattedCreationDate - отформатированное время создания объекта класса
     */
    public String getFormattedCreationDate() {
        String pattern = "yyyy-MM-dd HH:mm:ss.SSS";
        DateTimeFormatter europeanDateFormatter = DateTimeFormatter.ofPattern(pattern);
        return creationDate.plusHours(3).format(europeanDateFormatter);
    }

    /**
     * Метод, возвращающий возраст объекта класса
     *
     * @return age - возраст объекта класса
     */
    public Long getAge() {
        return age;
    }

    /**
     * Метод, возвращающий цвет объекта класса
     *
     * @return color - цвет объекта класса
     */
    public Color getColor() {
        return color;
    }

    /**
     * Метод, возвращающий тип объекта класса
     *
     * @return type - тип объекта класса
     */
    public DragonType getType() {
        return type;
    }

    /**
     * Метод, возвращающий характер объекта класса
     *
     * @return character - характер объекта класса
     */
    public DragonCharacter getCharacter() {
        return character;
    }

    /**
     * Метод, возвращающий уровень пещеры объекта класса
     *
     * @return cave - уровень пещеры объекта класса
     */
    public DragonCave getCave() {
        return cave;
    }

    /**
     * Метод, присваивающий имя объекта класса
     *
     * @param name имя объекта класса
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Метод, присваивающий координату x объекта класса
     *
     * @param x координата объекта класса
     */
    public void setCoordinateX(Double x) {
        this.getCoordinates().setX(x);
    }

    /**
     * Метод, присваивающий координату y объекта класса
     *
     * @param y координата y объекта класса
     */
    public void setCoordinateY(int y) {
        this.getCoordinates().setY(y);
    }

    /**
     * Метод, присваивающий возраст объекта класса
     *
     * @param age возраст объекта класса
     */
    public void setAge(Long age) {
        this.age = age;
    }

    /**
     * Метод, присваивающий цвет объекта класса
     *
     * @param color цвет объекта класса
     */
    public void setColor(Color color) {
        this.color = color;
    }

    /**
     * Метод, присваивающий тип объекта класса
     *
     * @param type тип объекта класса
     */
    public void setType(DragonType type) {
        this.type = type;
    }

    /**
     * Метод, присваивающий характер объекта класса
     *
     * @param character характер объекта класса
     */
    public void setCharacter(DragonCharacter character) {
        this.character = character;
    }

    /**
     * Метод, присваивающий глубину пещеры объекта класса
     *
     * @param depth глубина пещеры объекта класса
     */
    public void setCaveDepth(Double depth) {
        this.cave.setDepth(depth);
    }

    /**
     * Метод, присваивающий пещеру объекта класса
     *
     * @param cave пещера объекта класса
     */
    public void setCave(DragonCave cave) {
        this.cave = cave;
    }

    /**
     * Метод, возвращающий отформатированный вывод полей объекта класса
     *
     * @return поля объекта класса
     */
    public String toString() {
        return "Dragon id = " + this.getId() + ":\n\tname: " + this.getName() + "\n\t" + this.getCoordinates().toString() + "\n\tage: " + this.getAge() + "\n\tcolor: "
                + this.getColor().toString() + "\n\ttype: " + this.getType().toString() + "\n\tcharacter : " + this.getCharacter() +
                "\n\tcave_depth: " + this.getCave().toString() + "\n\tcreation_date: " + this.getFormattedCreationDate();
    }
}
