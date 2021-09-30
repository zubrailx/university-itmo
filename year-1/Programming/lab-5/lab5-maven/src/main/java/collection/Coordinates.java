package collection;

/**
 * Координаты - класс, который содержит координаты элемента коллекции
 */
public class Coordinates { //остановился тут
    /**
     * поле координаты x. Значение поля должно быть больше -648, поле не может быть null.
     */
    private Double x;
    /**
     * поле координаты y. Максимальное значение поля: 803.
     */
    private int y;

    /**
     * Конструктор класса.
     *
     * @param x значение координаты x
     */
    public Coordinates(Double x) {
        this.x = x;
    }

    /**
     * Конструктор класса.
     *
     * @param x значение координаты x
     * @param y значение координаты y
     */
    public Coordinates(Double x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * метод, устанавливающий значение координаты x
     *
     * @param x значение координаты x
     */
    public void setX(Double x) {
        this.x = x > -648 ? x : this.x;
    }

    /**
     * метод, устанавливающий значение координаты y
     *
     * @param y значение координаты y
     */
    public void setY(int y) {
        this.y = y > -648 ? y : this.y;
    }

    /**
     * метод, возвращающий значение координаты x
     *
     * @return значение координаты x
     */
    public Double getX() {
        return x;
    }

    /**
     * метод, возвращающий значение координаты y
     *
     * @return значение координаты y
     */
    public int getY() {
        return y;
    }

    @Override
    public String toString() {
        return "coordinates (x, y) = (" + getX() + ", " + getY() + ")";
    }
}