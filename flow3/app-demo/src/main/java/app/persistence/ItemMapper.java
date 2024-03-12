package app.persistence;

import app.entities.item;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class ItemMapper {
    public static List<item> listAllItems() {
        String sql = "SELECT * FROM item";
        List<item> iteml;
        iteml = new ArrayList<>();

        try (
                Connection connection = ConnectionPool.getConnection();
                PreparedStatement ps = connection.prepareStatement(sql);
        ) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()){
                iteml.add(new item(
                        rs.getInt("id"),
                        rs.getInt("user_id"),
                        rs.getInt("category"),
                        rs.getBigDecimal("price"),
                        rs.getString("title"),
                        rs.getString("desc")
                ));
            }

        } catch (SQLException e) {
            throw new RuntimeException(e);
        }

        return iteml;
    }
}
