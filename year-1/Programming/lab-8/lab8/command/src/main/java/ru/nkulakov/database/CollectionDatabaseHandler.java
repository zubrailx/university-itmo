package ru.nkulakov.database;

import ru.nkulakov.collection.Color;
import ru.nkulakov.collection.Dragon;

import java.sql.SQLException;

public interface CollectionDatabaseHandler {
    void insertRow(Dragon dragon) throws SQLException;

    void replaceRow(Dragon dragon) throws SQLException;

    void deleteRowById(Integer id) throws SQLException;

    Integer getOwnedRowByColor(Color color, UserData userData) throws SQLException;

    boolean isAnyRowById(Integer id) throws SQLException;

    boolean isOwner(Integer id, UserData userData) throws SQLException;

    Dragon getDragonById(Integer id) throws SQLException;

    void deleteAllOwned(UserData userData) throws SQLException;

    Dragon[] loadInMemory() throws SQLException;

    Integer[] getAllOwner(UserData userData) throws SQLException;

    void loadTable();
}
