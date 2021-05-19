package file;

import collection.*;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import java.io.IOException;
import java.time.Instant;
import java.time.ZonedDateTime;
import java.util.ArrayList;

/**
 * Класс, осуществляющий парсинг данных, переданных в качестве строки, в коллекцию или из коллекции доставать данные, которые будут записаны в строку в формате XML.
 */
public class XmlParser {
    /**
     * Метод, осуществляющий парсинг данных, представленных в качестве строки, в коллекцию
     *
     * @param text данные, которые следует запарсить
     * @return массив объектов типа Dragon
     * @throws ParserConfigurationException указывает на серьезную ошибку, возникшую в ходе выполнения метода
     * @throws SAXException                 Encapsulate a general SAX error or warning
     * @throws IOException                  Signals that an I/O exception of some sort has occurred
     */
    public Dragon[] parseToCollection(InputSource text) throws ParserConfigurationException, SAXException, IOException {
        SAXParserFactory factory = SAXParserFactory.newInstance();
        SAXParser parser = factory.newSAXParser();

        XmlHandler handler = new XmlHandler();
        try {
            parser.parse(text, handler);
        } catch (SAXException ignored) {
        }
        Dragon[] dragonArr = new Dragon[handler.dragons.size()];
        return handler.dragons.toArray(dragonArr);
    }


    private static class XmlHandler extends DefaultHandler {
        /**
         * Массив объектов типа Dragon
         */
        private ArrayList<Dragon> dragons = new ArrayList<>();
        /**
         * Хранит поле name объекта класса Dragon
         */
        private String name;
        /**
         * Хранит поле x объекта класса Coordinates
         */
        private Double x = null;
        /**
         * Хранит поле y объекта класса Coordinates
         */
        private Integer y = null;
        /**
         * Хранит поле creationDate объекта класса Dragon
         */
        private ZonedDateTime creationDate = null;
        /**
         * Хранит поле age объекта класса Dragon
         */
        private Long age = null;
        /**
         * Хранит поле color объекта класса DragonColor
         */
        private Color color = null;
        /**
         * Хранит поле type объекта класса DragonType
         */
        private DragonType type = null;
        /**
         * Хранит поле character объекта класса DragonCharacter
         */
        private DragonCharacter character = null;
        /**
         * Хранит поле depth объекта класса DragonCave
         */
        private Double depth = null;
        /**
         * Хранит название тега последнего встретившегося элемента, представленного в формате XML
         */
        private String lastElementName;

        /**
         * Метод, который присваивает полю lastElementName значение тега, который встретился в начале элемента.
         *
         * @param uri        The Namespace URI, or the empty string if the element has no Namespace URI or if Namespace processing is not being performed.
         * @param localName  The local name (without prefix), or the empty string if Namespace processing is not being performed.
         * @param qName      The qualified name (with prefix), or the empty string if qualified names are not available.
         * @param attributes The attributes attached to the element. If there are no attributes, it shall be an empty Attributes object.
         */
        @Override
        public void startElement(String uri, String localName, String qName, Attributes attributes) {
            lastElementName = qName;
        }

        /**
         * Метод, присваивающий полю, который хранится в данном объекте класса, значение, соответствующее значению между тегами. Проверка осуществляется по последнему открытому тегу.
         *
         * @param ch     The characters.
         * @param start  The start position in the character array.
         * @param length The number of characters to use from the character array.
         * @throws ClassCastException бросается, когда мы пытаемся привести значение между тегами к недопустимому типу в поле, имя которого совпадает в названием тега
         */
        @Override
        public void characters(char[] ch, int start, int length) throws ClassCastException {
            String information = new String(ch, start, length);

            information = information.replace("\n", "").trim();
            try {
                if (!information.isEmpty()) {
                    if (lastElementName.equals("name"))
                        name = information;
                    else if (lastElementName.equals("coordinate_x"))
                        x = Double.parseDouble(information);
                    else if (lastElementName.equals("coordinate_y"))
                        y = Integer.parseInt(information);
                    else if (lastElementName.equals("age"))
                        age = Long.parseLong(information);
                    else if (lastElementName.equals("color"))
                        color = Color.valueOf(information);
                    else if (lastElementName.equals("type"))
                        type = DragonType.valueOf(information);
                    else if (lastElementName.equals("character"))
                        character = DragonCharacter.valueOf(information);
                    else if (lastElementName.equals("depth"))
                        depth = Double.parseDouble(information);
                    else if (lastElementName.equals("creation_date"))
                        creationDate = ZonedDateTime.parse(information);
                }
            } catch (IllegalArgumentException ex) {
                System.err.println("Указанной константы перечисляемого типа не существует, либо невозможно преобразование типов");
            }
        }

        /**
         * Метод, добавляющий объект Dragon, когда встречает в файле встречается закрывающийся тег Dragon.
         * Если поля не соответствуют ОДЗ, то их значение не записывается, объект коллекции не сохраняется. После выполнения метода значения полей обнуляются.
         *
         * @param uri       The Namespace URI, or the empty string if the element has no Namespace URI or if Namespace processing is not being performed.
         * @param localName The local name (without prefix), or the empty string if Namespace processing is not being performed.
         * @param qName     The qualified name (with prefix), or the empty string if qualified names are not available.
         */
        @Override
        public void endElement(String uri, String localName, String qName) {
            if (qName.equals("dragon")) {
                if ((name != null && !name.isEmpty()) && (x != null && x > -648) && (y == null || y <= 803) && (age == null || age > 0)
                        && (color != null) && (type != null) && (character != null)) {

                    Coordinates coordinates = new Coordinates(x);
                    int intY;
                    if (y != null) {
                        intY = y;
                        coordinates.setY(intY);
                    }

                    if (creationDate == null) {
                        String i = Instant.now().toString();
                        creationDate = ZonedDateTime.parse(i);
                    }

                    DragonCave cave = new DragonCave();
                    if (depth != null) cave.setDepth(depth);

                    Dragon dragon = new Dragon(name, coordinates, color, type, character, cave, creationDate);

                    if (age != null) {
                        dragon.setAge(age);
                    }

                    dragons.add(dragon);


                } else System.err.println("Указаны не все параметры, либо параметры не принадлежат допустимой ОДЗ");

                name = null;
                x = null;
                y = null;
                creationDate = null;
                age = null;
                color = null;
                type = null;
                character = null;
                depth = null;
            }
        }
    }

    /**
     * Метод, осуществляющий преобразование массива объектов в формат XML, представленный в виде строки
     *
     * @param dragons массив объектов класса Dragon
     * @return строка, хранящая данные, представленные в формате XML
     */
    public String parseToXml(Dragon[] dragons) {

        StringBuilder sb = new StringBuilder();
        sb.append("<?xml version = \"1.0\"?>\n");
        sb.append("<treemap>\n");
        for (Dragon dragon : dragons) {
            sb.append("\t<dragon>\n");
            sb.append("\t\t<name>").append(dragon.getName()).append("</name>");
            sb.append("\n\t\t<coordinate_x>").append(dragon.getCoordinates().getX()).append("</coordinate_x>");
            try {
                Integer str = dragon.getCoordinates().getY();
                sb.append("\n\t\t<coordinate_y>").append(str).append("</coordinate_y>");
            } catch (NullPointerException ignored) {
            }
            sb.append("\n\t\t<creation_date>").append(dragon.getCreationDate()).append("</creation_date>");
            try {
                String str = dragon.getAge().toString();
                sb.append("\n\t\t<age>").append(str).append("</age>");
            } catch (NullPointerException ignored) {
            }
            sb.append("\n\t\t<color>").append(dragon.getColor()).append("</color>");
            sb.append("\n\t\t<type>").append(dragon.getType()).append("</type>");
            sb.append("\n\t\t<character>").append(dragon.getCharacter()).append("</character>");
            try {
                String str = dragon.getCave().getDepth().toString();
                sb.append("\n\t\t<depth>").append(str).append("</depth>");
            } catch (NullPointerException ignored) {
            }
            sb.append("\n\t</dragon>\n");
        }
        sb.append("</treemap>\n");
        return sb.toString();
    }
}