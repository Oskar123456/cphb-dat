package app.entities;

import app.persistence.ConnectionPool;

import java.util.Objects;

public class ChatUser {
    public String name;
    public String pwd;

    private static int idNum;

    public ChatUser(String name, String pwd) {
        this.name = name;
        this.pwd = pwd;
    }

    public static void init(ConnectionPool conn){

    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ChatUser chatUser = (ChatUser) o;
        return Objects.equals(name, chatUser.name) && Objects.equals(pwd, chatUser.pwd);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name, pwd);
    }
}
