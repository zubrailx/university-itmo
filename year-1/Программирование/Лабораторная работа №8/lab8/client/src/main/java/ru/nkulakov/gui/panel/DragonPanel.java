package ru.nkulakov.gui.panel;

import ru.nkulakov.collection.Dragon;
import ru.nkulakov.collection.DragonFieldValidation;
import ru.nkulakov.gui.frame.MainFrame;
import ru.nkulakov.gui.frame.command.CommandFrame;
import ru.nkulakov.gui.locale.Localization;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import java.awt.*;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.Format;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Locale;
import java.util.ResourceBundle;
import java.util.TreeMap;

public class DragonPanel extends JPanel {  //TODO get locale formatted

    private final CommandFrame commandFrame;
    private final TreeMap<Integer, Dragon> treeMap;
    private final String[] colorItems = {"GREEN", "BLUE", "YELLOW", "ORANGE"};
    private final ArrayList<String> colorItemsTranslated = new ArrayList<>();
    private final String[] typeItems = {"WATER", "UNDERGROUND", "FIRE"};
    private final ArrayList<String> typeItemsTranslated = new ArrayList<>();
    private final String[] characterItems = {"CUNNING", "WISE", "EVIL", "FICKLE"};
    private final ArrayList<String> characterItemsTranslated = new ArrayList<>();
    private JLabel id;
    private JFormattedTextField idT;
    private JLabel name;
    private JTextField nameT;
    private JLabel coordinate_x;
    private JFormattedTextField coordinate_xT;
    private JLabel coordinate_y;
    private JFormattedTextField coordinate_yT;
    private JLabel creationDate;
    private JFormattedTextField creationDateT;
    private JLabel age;
    private JFormattedTextField ageT;
    private JLabel color;
    private JComboBox<String> colorT;
    private JLabel type;
    private JComboBox<String> typeT;
    private JLabel character;
    private JComboBox<String> characterT;
    private JLabel cave_depth;
    private JFormattedTextField cave_depthT;
    private JLabel owner;
    private JTextField ownerT;

    public DragonPanel(CommandFrame commandFrame, MainFrame mainFrame) {
        super();
        this.commandFrame = commandFrame;
        treeMap = new TreeMap<>();
        ArrayList<Dragon> arrayList = mainFrame.getGuiManager().getClientCollection().getClientArrayList();
        for (Dragon dragon : arrayList) {
            treeMap.put(dragon.getId(), dragon);
        }

        initAndSet();
        setLocalization();
        locate();
    }

    private void initAndSet() {
        Font font = new Font("SansSerif", Font.PLAIN, 14);
        NumberFormat doubleFormat = DecimalFormat.getInstance();
        NumberFormat integerFormat = NumberFormat.getIntegerInstance();
        Format fullUSDate = DateFormat.getDateInstance(DateFormat.FULL, Locale.US);

        id = new JLabel();
        id.setFont(font);
        idT = new JFormattedTextField(integerFormat);
        idT.setFont(font);
        name = new JLabel();
        name.setFont(font);
        nameT = new JTextField();
        nameT.setFont(font);
        coordinate_x = new JLabel();
        coordinate_x.setFont(font);
        coordinate_xT = new JFormattedTextField(doubleFormat);
        coordinate_xT.setFont(font);
        coordinate_y = new JLabel();
        coordinate_y.setFont(font);
        coordinate_yT = new JFormattedTextField(integerFormat);
        coordinate_yT.setFont(font);
        creationDate = new JLabel();
        creationDate.setFont(font);
        creationDateT = new JFormattedTextField(fullUSDate);
        creationDateT.setEditable(false);
        creationDateT.setFont(font);
        age = new JLabel();
        age.setFont(font);
        ageT = new JFormattedTextField(integerFormat);
        ageT.setFont(font);
        color = new JLabel();
        color.setFont(font);
        colorT = new JComboBox<>();
        colorT.setEditable(true);
        colorT.setFont(font);
        type = new JLabel();
        type.setFont(font);
        typeT = new JComboBox<>();
        typeT.setEditable(true);
        typeT.setFont(font);
        character = new JLabel();
        character.setFont(font);
        characterT = new JComboBox<>();
        characterT.setFont(font);
        characterT.setEditable(true);
        cave_depth = new JLabel();
        cave_depth.setFont(font);
        cave_depthT = new JFormattedTextField(doubleFormat);
        cave_depthT.setFont(font);
        owner = new JLabel();
        owner.setFont(font);
        ownerT = new JTextField();
        ownerT.setEditable(false);
        ownerT.setFont(font);

        idT.getDocument().addDocumentListener(new DocumentListener() {  //TODO прикрутить многопоток.
            @Override
            public void insertUpdate(DocumentEvent e) {
                updated();
            }

            @Override
            public void removeUpdate(DocumentEvent e) {
                updated();
            }

            @Override
            public void changedUpdate(DocumentEvent e) {
                updated();
            }

            public void updated() {
                try {
                    Integer id = Integer.parseInt(idT.getText());
                    if (treeMap.containsKey(Integer.parseInt(idT.getText()))) {
                        Dragon dragon = treeMap.get(id);
                        nameT.setText(dragon.getName());
                        coordinate_xT.setText(dragon.getCoordinates().getX().toString());
                        coordinate_yT.setText(((Integer) dragon.getCoordinates().getY()).toString());
                        creationDateT.setText(dragon.getFormattedCreationDate());
                        ageT.setText((dragon.getAge() != null) ? dragon.getAge().toString() : "null");
                        colorT.setSelectedItem(dragon.getColor().toString());
                        typeT.setSelectedItem(dragon.getType().toString());
                        characterT.setSelectedItem(dragon.getCharacter().toString());
                        cave_depthT.setText(dragon.getCave().getDepth() != null ? dragon.getCave().getDepth().toString() : "null");
                        ownerT.setText(dragon.getOwner());
                    } else {
                        nameT.setText("");
                        coordinate_xT.setText("");
                        coordinate_yT.setText("");
                        creationDateT.setText("");
                        ageT.setText("");
                        colorT.setSelectedIndex(-1);
                        typeT.setSelectedIndex(-1);
                        characterT.setSelectedIndex(-1);
                        cave_depthT.setText("");
                        ownerT.setText("");

                    }
                } catch (NumberFormatException ignored) {
                }
            }
        });
    }

    private void locate() {
        GridLayout gridLayout = new GridLayout(11, 2, 0, 10);
        this.setLayout(gridLayout);
        this.add(id);
        this.add(idT);
        this.add(name);
        this.add(nameT);
        this.add(coordinate_x);
        this.add(coordinate_xT);
        this.add(coordinate_y);
        this.add(coordinate_yT);
        this.add(creationDate);
        this.add(creationDateT);
        this.add(age);
        this.add(ageT);
        this.add(color);
        this.add(colorT);
        this.add(type);
        this.add(typeT);
        this.add(character);
        this.add(characterT);
        this.add(cave_depth);
        this.add(cave_depthT);
        this.add(owner);
        this.add(ownerT);
        this.setPreferredSize(new Dimension(300, 400));
    }

    private void setLocalization() {
        ResourceBundle currentBundle = Localization.getCurrentResourceBundle();
        id.setText(currentBundle.getString("id"));
        name.setText(currentBundle.getString("name"));
        coordinate_x.setText(currentBundle.getString("coordinate_x"));
        coordinate_y.setText(currentBundle.getString("coordinate_y"));
        creationDate.setText(currentBundle.getString("creation_date"));
        age.setText(currentBundle.getString("age"));
        color.setText(currentBundle.getString("color"));
        type.setText(currentBundle.getString("type"));
        character.setText(currentBundle.getString("character"));
        cave_depth.setText(currentBundle.getString("cave_depth"));
        owner.setText(currentBundle.getString("owner"));


        colorItemsTranslated.clear();
        colorItemsTranslated.add(currentBundle.getString(colorItems[0]));
        colorItemsTranslated.add(currentBundle.getString(colorItems[1]));
        colorItemsTranslated.add(currentBundle.getString(colorItems[2]));
        colorItemsTranslated.add(currentBundle.getString(colorItems[3]));
        colorT = new JComboBox<>(colorItemsTranslated.toArray(new String[0]));

        typeItemsTranslated.clear();
        typeItemsTranslated.add(currentBundle.getString(typeItems[0]));
        typeItemsTranslated.add(currentBundle.getString(typeItems[1]));
        typeItemsTranslated.add(currentBundle.getString(typeItems[2]));
        typeT = new JComboBox<>(typeItemsTranslated.toArray(new String[0]));

        characterItemsTranslated.clear();
        characterItemsTranslated.add(currentBundle.getString(characterItems[0]));
        characterItemsTranslated.add(currentBundle.getString(characterItems[1]));
        characterItemsTranslated.add(currentBundle.getString(characterItems[2]));
        characterItemsTranslated.add(currentBundle.getString(characterItems[3]));
        characterT = new JComboBox<>(characterItemsTranslated.toArray(new String[0]));
    }

    public String getDragonAdaptation() {
        StringBuilder sb = new StringBuilder();
        if (!commandFrame.getOption().equals("insert")) {
            sb.append(commandFrame.getOption()).append(" ").append(idT.getValue()).append("\n");
            sb.append("name ").append(nameT.getText()).append("\n");
            sb.append("coordinate_x ").append(coordinate_xT.getValue()).append("\n");
            sb.append("coordinate_y ").append(coordinate_yT.getValue()).append("\n");
            sb.append("age ").append(ageT.getValue()).append("\n");
            if (colorT.getSelectedIndex() != -1)
                sb.append("color ").append(colorItems[colorT.getSelectedIndex()]).append("\n");
            if (typeT.getSelectedIndex() != -1)
                sb.append("type ").append(typeItems[typeT.getSelectedIndex()]).append("\n");
            if (characterT.getSelectedIndex() != -1)
                sb.append("character ").append(characterItems[characterT.getSelectedIndex()]).append("\n");
            sb.append("cave_depth ").append(cave_depthT.getValue());
            sb.append("\n\r");
        }
        return sb.toString();
    }

    public String getInsertDragonAdaptation() {
        String[] field = {"name", "coordinate_x", "coordinate_y", "age", "color", "type", "character", "cave_depth"};
        ArrayList<String> values = new ArrayList<>();
        values.add(nameT.getText());
        values.add(String.valueOf(coordinate_xT.getValue()));
        values.add(String.valueOf(coordinate_yT.getValue()));

        values.add(String.valueOf(ageT.getValue()));
        values.add((colorT.getSelectedIndex() != -1) ? colorItems[colorT.getSelectedIndex()] : "hehe");
        values.add((typeT.getSelectedIndex() != -1) ? typeItems[typeT.getSelectedIndex()] : "hehe");
        values.add((characterT.getSelectedIndex() != -1) ? characterItems[characterT.getSelectedIndex()] : "hehe");
        values.add(String.valueOf(cave_depthT.getValue()));
        int i = 0;
        StringBuilder sb = new StringBuilder();
        sb.append("insert ").append(idT.getValue()).append("\n");
        while (i != field.length && DragonFieldValidation.validate(field[i], values.get(i))) {
            sb.append(values.get(i)).append("\n");
            i++;
        }
        if (i == field.length) {
            return sb.toString();
        }
        return null;
    }

    public void setIdText(Integer id) {
        this.idT.setValue(id);
    }

    public JFormattedTextField getIdT() {
        return idT;
    }
}
