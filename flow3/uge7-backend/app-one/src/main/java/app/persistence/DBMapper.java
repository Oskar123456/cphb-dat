package app.persistence;

import app.entities.ChatMsg;
import app.entities.Student;

import java.sql.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class DBMapper {
    public static List<Student> getAllStudents(ConnectionPool connectionPool) throws DatabaseException
    {
        List<Student> userList = new ArrayList<>();
        String sql = "select * from students";
        try (Connection connection = connectionPool.getConnection())
        {
            try (PreparedStatement ps = connection.prepareStatement(sql))
            {
                ResultSet rs = ps.executeQuery();

                while (rs.next())
                {
                    int ID = rs.getInt("id");
                    String userName = rs.getString("name");
                    String password = rs.getString("password");
                    Student user = new Student(ID, userName, password);
                    userList.add(user);
                }
            }
        }
        catch (SQLException ex)
        {
            throw new DatabaseException(ex, "Could not get users from database");
        }
        return userList;
    }

    public static List<ChatMsg> getAllChatMsgs(ConnectionPool connectionPool) throws DatabaseException
    {
        List<ChatMsg> msgList = new ArrayList<>();
        String sql = "select * from msg";
        try (Connection connection = connectionPool.getConnection())
        {
            try (PreparedStatement ps = connection.prepareStatement(sql))
            {
                ResultSet rs = ps.executeQuery();

                while (rs.next())
                {
                    Timestamp sentTime = rs.getTimestamp("time");
                    String content = rs.getString("content");
                    String userId = rs.getString("user_name");
                    ChatMsg msg = new ChatMsg(userId, sentTime, content);
                    msgList.add(msg);
                }
            }
        }
        catch (SQLException ex)
        {
            throw new DatabaseException(ex, "Could not get users from database");
        }
        return msgList;
    }

    public static int insertChatMsg(ConnectionPool connectionPool,
                                     String content,
                                     String userName) throws DatabaseException {
        ChatMsg msg = new ChatMsg(userName, new Timestamp(System.currentTimeMillis()), content);
        String sql = "INSERT INTO msg (id, user_name, time, content) VALUES (DEFAULT, ?, ?, ?)";
        try (Connection connection = connectionPool.getConnection()){
            try (PreparedStatement stmt = connection.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)){
                stmt.setString(1, msg.userName());
                stmt.setTimestamp(2, msg.time());
                stmt.setString(3, msg.content());
                stmt.executeUpdate();
                var keySet = stmt.getGeneratedKeys();
                if (keySet.next())
                    return keySet.getInt(1);
                else
                    return -1;
            }
        }
        catch (SQLException ex){
            throw new DatabaseException(ex, "Could not insert chat message");
        }
    }
}
