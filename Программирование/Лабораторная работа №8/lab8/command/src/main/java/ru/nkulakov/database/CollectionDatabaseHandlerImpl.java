package ru.nkulakov.database;

import ru.nkulakov.collection.Color;
import ru.nkulakov.collection.Dragon;
import ru.nkulakov.collection.DragonCharacter;
import ru.nkulakov.collection.DragonType;

import java.sql.*;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.util.ArrayList;
import java.util.TreeMap;

public class CollectionDatabaseHandlerImpl implements CollectionDatabaseHandler {

    private final Connection connection;

    public CollectionDatabaseHandlerImpl(Connection connection) {
        this.connection = connection;
        loadTable();
    }

    public void insertRow(Dragon dragon) throws SQLException {
        String sql = "INSERT INTO DRAGONCOLLECTION (name, coordinate_x, coordinate_y, creation_date, age, color, type, " +
                "character, cave_depth, owner, id) Values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, dragon.getName());
        ps.setDouble(2, dragon.getCoordinates().getX());
        ps.setInt(3, dragon.getCoordinates().getY());
        ps.setTimestamp(4, Timestamp.valueOf(dragon.getCreationDate().toLocalDateTime()));
        if (dragon.getAge() != null) {
            ps.setLong(5, dragon.getAge());
        } else ps.setNull(5, Types.NULL);
        ps.setString(6, dragon.getColor().toString());
        ps.setString(7, dragon.getType().toString());
        ps.setString(8, dragon.getCharacter().toString());
        if (dragon.getCave().getDepth() != null) {
            ps.setDouble(9, dragon.getCave().getDepth());
        } else ps.setNull(9, Types.NULL);
        ps.setString(10, dragon.getOwner());
        ps.setInt(11, dragon.getId());

        ps.executeUpdate();
        ps.close();
    }

    public void replaceRow(Dragon dragon) throws SQLException {
        String sql = "UPDATE DRAGONCOLLECTION SET (name, coordinate_x, coordinate_y, age, color, type, " +
                "character, cave_depth) = (?, ?, ?, ?, ?, ?, ?, ?) WHERE id = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, dragon.getName());
        ps.setDouble(2, dragon.getCoordinates().getX());
        ps.setInt(3, dragon.getCoordinates().getY());
        if (dragon.getAge() != null) {
            ps.setLong(4, dragon.getAge());
        } else ps.setNull(4, Types.NULL);
        ps.setString(5, dragon.getColor().toString());
        ps.setString(6, dragon.getType().toString());
        ps.setString(7, dragon.getCharacter().toString());
        if (dragon.getCave().getDepth() != null) {
            ps.setDouble(8, dragon.getCave().getDepth());
        } else ps.setNull(8, Types.NULL);
        ps.setInt(9, dragon.getId());

        ps.executeUpdate();
        ps.close();
    }

    public void deleteRowById(Integer id) throws SQLException {
        String sql = "DELETE FROM DRAGONCOLLECTION WHERE ID = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setInt(1, id);
        int delRows = ps.executeUpdate();
        if (delRows == 1) {
            System.out.println("Строка была удалена.");
        } else System.out.println("Не было удалено строк.");

    }

    public Integer getOwnedRowByColor(Color color, UserData userData) throws SQLException { //if find - return id, else return null
        String sql = "SELECT ID FROM DRAGONCOLLECTION WHERE color = ? AND OWNER = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, color.name());
        ps.setString(2, userData.getLogin());
        ResultSet rs = ps.executeQuery();
        if (rs.next()) {
            return rs.getInt(1);
        }
        return null;
    }

    public boolean isAnyRowById(Integer id) throws SQLException {
        String sql = "SELECT * FROM DRAGONCOLLECTION WHERE ID = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setInt(1, id);
        ResultSet rs = ps.executeQuery();
        return rs.isBeforeFirst();
    }

    public boolean isOwner(Integer id, UserData userData) throws SQLException {
        String sql = "SELECT * FROM DRAGONCOLLECTION WHERE ID = ? AND OWNER = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setInt(1, id);
        ps.setString(2, userData.getLogin());
        ResultSet rs = ps.executeQuery();
        return rs.isBeforeFirst();
    }

    public Dragon getDragonById(Integer id) throws SQLException {
        String sql = "SELECT * FROM DRAGONCOLLECTION WHERE ID = ?";

        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setInt(1, id);
        ResultSet rs = ps.executeQuery();
        if (rs.next()) {
            return this.createDragonFromCurrentRow(rs);
        }
        return null;
    }

    public void deleteAllOwned(UserData userData) throws SQLException { //Возвращает id всех удаленных элементов
        String sql = "DELETE FROM DRAGONCOLLECTION WHERE OWNER = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, userData.getLogin());
        int delRows = ps.executeUpdate();
        System.out.println("Было удалено " + delRows + " строк.");
    }

    public Dragon[] loadInMemory() throws SQLException {
        TreeMap<Integer, Dragon> treeMap = new TreeMap<>();
        String sql = "SELECT * FROM DRAGONCOLLECTION";
        Statement statement = connection.createStatement();
        ResultSet rs = statement.executeQuery(sql);
        if (rs.isBeforeFirst()) {
            while (rs.next()) {
                Dragon dragon = this.createDragonFromCurrentRow(rs);
                treeMap.put(dragon.getId(), dragon); //if (dragon != null)
            }
        }
        return treeMap.values().toArray(new Dragon[0]);
    }

    private Dragon createDragonFromCurrentRow(ResultSet rs) throws SQLException {
        Integer id = rs.getInt(1);
        String dragon_name = rs.getString(2);
        Double coordinate_x = rs.getDouble(3);
        Integer coordinate_y = rs.getInt(4);
        ZonedDateTime creationDate = rs.getTimestamp(5).toLocalDateTime().atZone(ZoneId.of("UTC+03:00"));
        Long age = rs.getLong(6);
        Color color = Color.valueOf(rs.getString(7));
        DragonType type = DragonType.valueOf(rs.getString(8));
        DragonCharacter dragonCharacter = DragonCharacter.valueOf(rs.getString(9));
        Double depth = rs.getDouble(10);
        String owner = rs.getString(11);

        return Dragon.createDragon(id, dragon_name, coordinate_x, coordinate_y, age, color, type, dragonCharacter, depth, creationDate, owner);
    }

    public Integer[] getAllOwner(UserData userData) throws SQLException {
        String sql = "SELECT * FROM DRAGONCOLLECTION WHERE OWNER = ?";
        PreparedStatement ps = connection.prepareStatement(sql);
        ps.setString(1, userData.getLogin());
        ResultSet rs = ps.executeQuery();
        ArrayList<Integer> ids = new ArrayList<>();
        while (rs.next()) {
            ids.add(rs.getInt(1));
        }
        return ids.toArray(new Integer[0]);
    }

    public void loadTable() {
        String sql = "CREATE TABLE IF NOT EXISTS dragoncollection (" +
                "id               integer primary key," +
                "name             text not null," +
                "coordinate_x     double precision not null check(coordinate_x > -648)," +
                "coordinate_y     integer not null check(coordinate_y <= 803)," +
                "creation_date    timestamp with time zone," +
                "age              bigint check(age > 0)," +
                "color            text," +
                "type             text," +
                "character        text," +
                "cave_depth       double precision," +
                "owner            text," +
                "foreign key (owner) REFERENCES userdata(user_login) on delete cascade" +
                ");";
        try {
            Statement st = connection.createStatement();
            st.executeUpdate(sql);
        } catch (SQLException ex) {
            System.out.println("Ошибка подключения к бд.");
        }
    }
}