package ru.nkulakov.database;

import java.sql.*;

public class UserDatabaseHandlerImpl implements UserDatabaseHandler {

    private final Connection connection;

    public UserDatabaseHandlerImpl(Connection connection) {
        this.connection = connection;
        loadTable();
    }

    public void addUser(UserData userData) {
        String user_login = userData.getLogin();
        String user_pass = userData.getPassword();
        String sql = "INSERT INTO USERDATA (USER_LOGIN, USER_PASSWORD) VALUES (?, ?)";
        try {
            PreparedStatement ps = connection.prepareStatement(sql);
            ps.setString(1, user_login);
            ps.setString(2, user_pass);

            ps.executeUpdate();
            ps.close();

        } catch (SQLException ex) {
            System.out.println("Пользователь с указанным login уже существует\nДальнейшая ответственность за командой verifyUser");
        }
    }

    public void ConnectUser(UserData userData) {
        userData.setIsConnected(true);
    }

    public void verifyUser(UserData userData) {
        String user_login = userData.getLogin();
        String user_pass = userData.getPassword();
        String sql = "SELECT * FROM USERDATA WHERE USER_LOGIN = ? AND USER_PASSWORD = ?";
        try {
            PreparedStatement ps = connection.prepareStatement(sql);
            ps.setString(1, user_login);
            ps.setString(2, user_pass);
            ResultSet rs = ps.executeQuery();
            userData.setIsNewUser(!rs.isBeforeFirst());
            ps.close();
        } catch (SQLException ex) {
            System.out.println("Ошибка подключения к базе данных. Команда verify_user.");
            userData.setIsNewUser(true);
        }
    }

    private void loadTable() {
        String sql = "CREATE TABLE IF NOT EXISTS  USERDATA (" +
                "USER_LOGIN     text PRIMARY KEY," +
                "USER_PASSWORD  text " +
                ");";
        try {
            Statement st = connection.createStatement();
            st.executeUpdate(sql);
        } catch (SQLException ex) {
            System.out.println("Ошибка подключения к бд.");
        }
    }
}
