package database;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class UserDatabaseHandler {

    private final Connection connection;

    public UserDatabaseHandler(Connection connection) {
        this.connection = connection;
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
        String sql = "SELECT * FROM USERDATA WHERE USER_LOGIN = ? AND user_password = ?";
        try {
            PreparedStatement ps = connection.prepareStatement(sql);
            ps.setString(1, user_login);
            ps.setString(2, user_pass);
            ResultSet rs = ps.executeQuery();
            userData.setIsNewUser(!rs.isBeforeFirst());
            ps.close();
        } catch (SQLException ex) {
            System.out.println("Ошибка подключения к базе данных. Команда verify_user.");
        }
    }
}
