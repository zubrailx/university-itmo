package collection;

/**
 * Класс - пещера объекта класса Dragon
 */
public class DragonCave {
    /**
     * глубина пещеры объекта класса. Поле может быть null.
     */
    private Double depth = null;

    /**
     * Конструктор класса
     *
     * @param depth глубина пещеры объекта класса
     */
    public DragonCave(Double depth) {
        this.depth = depth;
    }

    /**
     * Конструктор класса без параметров
     */
    public DragonCave() {

    }

    /**
     * Метод, присваивающий значение глубины поля объекта класса.
     *
     * @param depth глубина объекта
     */
    public void setDepth(Double depth) {
        this.depth = depth;
    }

    /**
     * Метод, возвращающий значение глубины поля объекта класса.
     *
     * @return значение глубины поля объекта класса
     */
    public Double getDepth() {
        return depth;
    }

    /**
     * Метод, возвращающий значения полей объекта класса
     *
     * @return значение глубины поля объекта класса
     * @throws NullPointerException возращает строку "not initialized"
     */
    @Override
    public String toString() {
        try {
            return getDepth().toString();
        } catch (NullPointerException e) {
            return "not initialized";
        }
    }
}