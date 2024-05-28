package app.persistence;

import app.entities.ChatMsg;
import app.entities.ChatUser;
import app.entities.Student;

import java.sql.*;
import java.util.*;
import java.util.Date;

public class DBMapper {

    public static HashMap<String, String> getAllChatUsers(ConnectionPool connectionPool) throws DatabaseException
    {
        HashMap<String, String> userList = new HashMap<>();
        String sql = "select * from users";
        try (Connection connection = connectionPool.getConnection())
        {
            try (PreparedStatement ps = connection.prepareStatement(sql))
            {
                ResultSet rs = ps.executeQuery();

                while (rs.next())
                {
                    String id = rs.getString("id");
                    String pwd = rs.getString("pwd");
                    userList.put(id, pwd);
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


    public static void insertChatUser(ConnectionPool connectionPool,
                                    String userName,
                                    String pwd) throws DatabaseException {
        ChatUser user = new ChatUser(userName, pwd);
        String sql = "INSERT INTO users (id, pwd) VALUES (?, ?)";
        try (Connection connection = connectionPool.getConnection()){
            try (PreparedStatement stmt = connection.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS)){
                stmt.setString(1, user.name);
                stmt.setString(2, user.pwd);
                stmt.executeUpdate();
            }
        }
        catch (SQLException ex){
            throw new DatabaseException(ex, "Could not insert user %s".formatted(userName));
        }
    }

}
