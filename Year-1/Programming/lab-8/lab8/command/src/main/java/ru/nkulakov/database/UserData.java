package ru.nkulakov.database;

import ru.nkulakov.collection.Dragon;
import ru.nkulakov.commands.abstr.CommandContainer;

import java.io.Serializable;
import java.net.InetAddress;
import java.util.TreeMap;

public class UserData implements Serializable {
    private String login;
    private String password;
    private boolean isNewUser;
    private boolean isConnected;

    private InetAddress inetAddress;
    private Integer port;

    private CommandContainer commandContainer;
    private TreeMap<Integer, Dragon> dragonCollection;

    public UserData(boolean isNewUser) {
        this.isNewUser = isNewUser;
        this.isConnected = false;
    }

    //Login.
    public String getLogin() {
        return login;
    }

    public void setLogin(String login) {
        this.login = login;
    }

    //Password.
    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    //IsNewUser.
    public boolean getIsNewUser() {
        return isNewUser;
    }

    public void setIsNewUser(boolean value) {
        this.isNewUser = value;
    }

    //IsConnected.
    public boolean getIsConnected() {
        return isConnected;
    }

    public void setIsConnected(boolean value) {
        this.isConnected = value;
    }

    //CommandContainer.
    public CommandContainer getCommandContainer() {
        return commandContainer;
    }

    public void setCommandContainer(CommandContainer commandContainer) {
        this.commandContainer = commandContainer;
    }

    //InetAddress.
    public InetAddress getInetAddress() {
        return inetAddress;
    }

    public void setInetAddress(InetAddress inetAddress) {
        this.inetAddress = inetAddress;
    }

    //Port.
    public Integer getPort() {
        return port;
    }

    public void setPort(Integer port) {
        this.port = port;
    }

    //DragonCollection.
    public TreeMap<Integer, Dragon> getDragonCollection() {
        return dragonCollection;
    }

    public void setDragonCollection(TreeMap<Integer, Dragon> dc) {
        this.dragonCollection = dc;
    }
}
