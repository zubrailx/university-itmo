package ru.nkulakov.database;

import java.sql.Connection;
import java.sql.SQLException;

public interface DatabaseConnection {
    Connection connectToDatabase() throws SQLException;
}
