package persistence;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class Database {
    private Connection connection;
    private final String USER;
    private final String PASSWORD;
    private final String URL;

    public Database(String user, String password, String url) {
        USER = user;
        PASSWORD = password;
        URL = url;
        try {
            Class.forName("org.postgresql.Driver");
        } catch (ClassNotFoundException e) {
            // TODO: Make own throwable exception and let it bubble upwards
            e.printStackTrace();
            System.out.println("Fejl ved instantiering af Driver klasse");
        }
    }

    public Connection connect(){
        Connection connection = null;
        try {
            connection = DriverManager.getConnection(URL, USER, PASSWORD);
        } catch (SQLException throwables) {
            // TODO: Make own throwable exception and let it bubble upwards
            throwables.printStackTrace();
            System.out.println("Fejl under etablering af forbindelse til database");
        }
        return connection;
    }
}
