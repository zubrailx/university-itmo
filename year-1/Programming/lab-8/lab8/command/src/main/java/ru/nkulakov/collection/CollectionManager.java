package ru.nkulakov.collection;

import java.io.PrintStream;
import java.util.TreeMap;

public interface CollectionManager {
    String info();

    String show();

    void insert(Integer id, Dragon dragon, PrintStream printStream);

    void removeKey(Integer id);

    void save(String filePath);

    Integer[] getLowerKeys(Integer id);

    Double averageAge();

    Dragon maxAge();

    TreeMap<Integer, Dragon> getTreeMap();
}
