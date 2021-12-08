package collection;

import java.io.Serializable;

/**
 * Класс - пещера объекта класса Dragon.
 */
public class DragonCave implements Serializable {

    /**
     * Глубина пещеры объекта класса. Поле может быть null.
     */
    private Double depth = null;

    /**
     * Конструктор класса.
     *
     * @param depth глубина пещеры объекта класса.
     */
    public DragonCave(Double depth) {
        this.depth = depth;
    }

    /**
     * Конструктор класса без параметров.
     */
    public DragonCave() {
    }

    /**
     * Метод, присваивающий значение глубины поля объекта класса.
     *
     * @param depth глубина объекта.
     */
    public void setDepth(Double depth) {
        this.depth = depth;
    }

    /**
     * Метод, возвращающий значение глубины поля объекта класса.
     *
     * @return Double - значение глубины поля объекта класса.
     */
    public Double getDepth() {
        return depth;
    }

    /**
     * Метод, возвращающий значения полей объекта класса.
     *
     * @return String - значение глубины поля объекта класса.
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