package database;

import commands.abstr.CommandContainer;

import java.io.Serializable;
import java.net.InetAddress;

public class UserData implements Serializable {
    private String login;
    private String password;
    private boolean isNewUser;
    private boolean isConnected;
    private InetAddress inetAddress;
    private Integer port;

    private CommandContainer commandContainer;

    public UserData(boolean isNewUser) {
        this.isNewUser = isNewUser;
        this.isConnected = false;
    }

    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public boolean getIsNewUser() {
        return isNewUser;
    }

    public void setIsNewUser(boolean value) {
        this.isNewUser = value;
    }

    public CommandContainer getCommandContainer() {
        return commandContainer;
    }

    public void setCommandContainer(CommandContainer commandContainer) {
        this.commandContainer = commandContainer;
    }

    public boolean getIsConnected() {
        return isConnected;
    }

    public void setIsConnected(boolean value) {
        this.isConnected = value;
    }

    public InetAddress getInetAddress() {
        return inetAddress;
    }

    public void setInetAddress(InetAddress inetAddress) {
        this.inetAddress = inetAddress;
    }

    public Integer getPort() {
        return port;
    }

    public void setPort(Integer port) {
        this.port = port;
    }
}
