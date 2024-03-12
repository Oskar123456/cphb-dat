package app.persistence;

import java.sql.SQLException;

public class DatabaseException extends Exception {
    public DatabaseException(SQLException ex, String s) {
        super(s, ex);
    }
    public DatabaseException(String s) {
        super(s);
    }
}
