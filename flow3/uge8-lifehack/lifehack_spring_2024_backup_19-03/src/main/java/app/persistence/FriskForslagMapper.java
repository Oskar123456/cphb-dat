package app.persistence;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import app.entities.FriskForslagFoodItem;
import app.entities.FriskForslagIngredient;
import app.entities.FriskForslagRecipe;
import app.exceptions.DatabaseException;

public class FriskForslagMapper {
    public static String recipeTable = "friskforslagrecipe";
    public static String ingredientTable = "friskforslagingredient";
    public static String foodItemTable = "friskforslagfooditem";

    /*
     * INSERTS
     */

    public static void RecipeInsert(FriskForslagRecipe rec,
            ConnectionPool connPool) throws DatabaseException {
        String sql = "INSERT INTO friskforslagrecipe (name, descr, proc, duration_in_minutes) VALUES (?, ?, ?, ?)";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ps.setString(1, rec.Name());
            ps.setString(2, rec.Desc());
            ps.setString(3, rec.Proc());
            ps.setInt(4, rec.DurationInMinutes());

            int rowsAffected = ps.executeUpdate();
            if (rowsAffected != 1) {
                throw new DatabaseException("Fejl ved indsættelse af opskrift i databasen");
            }
        } catch (SQLException e) {
            String msg = "Fejl ved indsættelse af opskrift i databasen";
            throw new DatabaseException(msg, e.getMessage());
        }
    }

    public static void IngredientInsert(FriskForslagIngredient ing,
            ConnectionPool connPool) throws DatabaseException {
        String sql = "INSERT INTO friskforslagingredient (recipe_id, fooditem_id, quantity, unit) VALUES (?, ?, ?, ?)";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ps.setInt(1, ing.RecipeId());
            ps.setInt(2, ing.FoodItemId());
            ps.setInt(3, ing.Quantity());
            ps.setString(4, ing.Unit());

            int rowsAffected = ps.executeUpdate();
            if (rowsAffected != 1) {
                throw new DatabaseException("Fejl ved indsættelse af ingrediens i databasen");
            }
        } catch (SQLException e) {
            String msg = "Fejl ved indsættelse af ingrediens i databasen";
            throw new DatabaseException(msg, e.getMessage());
        }
    }

    public static void FoodItemInsert(FriskForslagFoodItem food,
            ConnectionPool connPool) throws DatabaseException {
        String sql = "INSERT INTO friskforslagfooditem (name, descr) VALUES (?, ?)";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ps.setString(1, food.Name());
            ps.setString(2, food.Desc());

            int rowsAffected = ps.executeUpdate();
            if (rowsAffected != 1) {
                throw new DatabaseException("Fejl ved indsættelse af fødevare i databasen");
            }
        } catch (SQLException e) {
            String msg = "Fejl ved indsættelse af fødevare i databasen";
            throw new DatabaseException(msg, e.getMessage());
        }
    }

    /*
     * SELECTS
     */

    public static ArrayList<FriskForslagRecipe> ListRecipes(ConnectionPool connPool) throws DatabaseException {
        ArrayList<FriskForslagRecipe> recipeList = new ArrayList<>();

        String sql = "SELECT * FROM friskforslagrecipe";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                FriskForslagRecipe rec = new FriskForslagRecipe(rs.getInt("id"),
                        rs.getString("name"),
                        rs.getString("descr"),
                        rs.getString("proc"),
                        rs.getInt("duration_in_minutes"));
                recipeList.add(rec);
            }
        } catch (SQLException e) {
            String msg = "Fejl ved læsning af opskriftstabel";
            throw new DatabaseException(msg, e.getMessage());
        }
        return recipeList;
    }

    public static ArrayList<FriskForslagIngredient> listIngredients(ConnectionPool connPool) throws DatabaseException {
        ArrayList<FriskForslagIngredient> ingredientList = new ArrayList<>();

        String sql = "SELECT * FROM friskforslagingredient";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                FriskForslagIngredient ing = new FriskForslagIngredient(rs.getInt("id"),
                        rs.getInt("recipe_id"),
                        rs.getInt("fooditem_id"),
                        rs.getInt("quantity"),
                        rs.getString("unit"));
                ingredientList.add(ing);
            }
        } catch (SQLException e) {
            String msg = "Fejl ved læsning af ingredienstabel";
            throw new DatabaseException(msg, e.getMessage());
        }
        return ingredientList;
    }

    public static ArrayList<FriskForslagFoodItem> ListFoodItems(ConnectionPool connPool) throws DatabaseException {
        ArrayList<FriskForslagFoodItem> foodItemList = new ArrayList<>();

        String sql = "SELECT * FROM friskforslagfooditem";

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                FriskForslagFoodItem food = new FriskForslagFoodItem(rs.getInt("id"),
                        rs.getString("name"),
                        rs.getString("descr"));
                foodItemList.add(food);
            }
        } catch (SQLException e) {
            String msg = "Fejl ved indsættelse af fødevare i databasen";
            throw new DatabaseException(msg, e.getMessage());
        }
        return foodItemList;
    }

    /*
     * SELECTS - ADVANCED
     */

    public static ArrayList<FriskForslagFoodItem> ListSpecificFoodItems(ConnectionPool connPool, String... foodItems)
            throws DatabaseException {
        if (foodItems == null || foodItems.length < 1)
            return null;

        ArrayList<FriskForslagFoodItem> foodItemList = new ArrayList<>();

        for (String foodItem : foodItems) {
            // TODO: Guard against malicious activity :)
            Pattern p = Pattern.compile("[; \r\n\t]");
            Matcher m = p.matcher(foodItem);
            if (m.find())
                continue;
            // TODO: maybe more advanced name matching?
            String sql = "SELECT * FROM friskforslagfooditems " +
                    "WHERE name ILIKE " + foodItem;
            try (
                    Connection conn = connPool.getConnection();
                    PreparedStatement ps = conn.prepareStatement(sql);) {
                ResultSet rs = ps.executeQuery();
                while (rs.next()) {
                    FriskForslagFoodItem food = new FriskForslagFoodItem(rs.getInt("id"),
                            rs.getString("name"),
                            rs.getString("descr"));
                    foodItemList.add(food);
                }
            } catch (SQLException e) {
                String msg = "Fejl ved søgning af fødevarer i databasen";
                throw new DatabaseException(msg, e.getMessage());
            }
        }
        return foodItemList;
    }

    public static HashMap<FriskForslagRecipe, Integer> ListRecipesWithNIngredients(ConnectionPool connPool,
            String... foodItems) throws DatabaseException {
        if (foodItems == null || foodItems.length < 1)
            return null;

        ArrayList<FriskForslagRecipe> recipeList;
        ArrayList<FriskForslagIngredient> ingredientList;
        ArrayList<FriskForslagFoodItem> foodItemList = new ArrayList<>();

        StringBuilder sqlCondition = new StringBuilder();
        for (int i = 0; i < foodItems.length; ++i) {
            // ---------------------------------
            // TODO: Guard against malicious input
            Pattern p = Pattern.compile("[; \r\n\t]");
            Matcher m = p.matcher(foodItems[i]);
            if (m.find())
                continue;
            // ---------------------------------
            sqlCondition.append("name ILIKE '" + foodItems[i] + "'");
            if (i < foodItems.length - 1)
                sqlCondition.append(" OR ");
        }

        String sql = "SELECT * FROM " +
                foodItemTable +
                " WHERE " +
                sqlCondition;

        System.out.println(sql);

        // String sql = """
        // SELECT
        // recs.id AS recId,
        // recs.name AS recName,
        // recs.descr AS recDescr,
        // recs.proc AS recProc,
        // recs.duration_in_minutes AS time,
        // matchedfood.name AS foodItemName,
        // ing.quantity AS quantity,
        // ing.unit AS unit
        // FROM friskforslagingredient as ing
        // INNER JOIN (
        // SELECT * FROM friskforslagfooditem as food
        // WHERE
        // """
        // +
        // sqlCondition
        // +
        // """
        // ) AS matchedfood
        // ON matchedfood.id = ing.fooditem_id
        // INNER JOIN friskforslagrecipe AS recs
        // ON recs.id = ing.recipe_id
        // """;

        try (
                Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                FriskForslagFoodItem food = new FriskForslagFoodItem(rs.getInt("id"),
                        rs.getString("name"),
                        rs.getString("descr"));
                foodItemList.add(food);
            }
            recipeList = ListRecipes(connPool);
            ingredientList = listIngredients(connPool);
        } catch (SQLException e) {
            String msg = "Fejl ved søgning i database";
            throw new DatabaseException(msg, e.getMessage());
        }

        // ----------------------
        // TODO: might want to 'optimise' by delegating
        // TODO: sorting to SQL query
        HashMap<Integer, Integer> recipeFreqMap = new HashMap<>();
        for (FriskForslagRecipe rec : recipeList){
            recipeFreqMap.put(rec.Id(), 0);
        }
        for (FriskForslagFoodItem foodItem : foodItemList){
            for (FriskForslagIngredient ingredient : ingredientList){
                if (foodItem.Id() == ingredient.FoodItemId())
                    recipeFreqMap.put(ingredient.RecipeId(), recipeFreqMap.get(ingredient.RecipeId()) + 1);
            }
        }
        // ----------------------
        HashMap<FriskForslagRecipe, Integer> finalFreqMap = new HashMap<>();
        for (Entry<Integer, Integer> ent : recipeFreqMap.entrySet()){
            System.out.println("Recipe and number of ingredients: " + ent.getKey() + " / " + ent.getValue());
            for (FriskForslagRecipe rec : recipeList){
                if (rec.Id() == ent.getKey()){
                    finalFreqMap.put(rec, ent.getValue());
                }
            }
        }

        return (finalFreqMap.isEmpty()) ? null : finalFreqMap;
    }
}
