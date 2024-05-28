package app.persistence;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import app.entities.FriskForslagFoodItem;
import app.entities.FriskForslagIngredient;
import app.entities.FriskForslagRecipe;
import app.entities.FriskForslagRecipeSpec;
import app.exceptions.DatabaseException;

public class FriskForslagMapper
{
    /*
     * INSERTS
     */
    public static void RecipeInsert(FriskForslagRecipe rec,
                                    ConnectionPool connPool) throws DatabaseException
    {
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
                                        ConnectionPool connPool) throws DatabaseException
    {
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
                                      ConnectionPool connPool) throws DatabaseException
    {
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

    public static ArrayList<FriskForslagRecipe> ListRecipes(ConnectionPool connPool) throws DatabaseException
    {
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

    public static ArrayList<FriskForslagIngredient> listIngredients(ConnectionPool connPool) throws DatabaseException
    {
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

    public static ArrayList<FriskForslagFoodItem> ListFoodItems(ConnectionPool connPool) throws DatabaseException
    {
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
            throws DatabaseException
    {
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

    public static List<FriskForslagRecipeSpec> ListRecipesWithNIngredients(
            ConnectionPool connPool,
            String... foodItems) throws DatabaseException
    {
        if (foodItems == null || foodItems.length < 1)
            return null;
        // Set WHERE clause to match the searched fooditems
        StringBuilder sqlCondition = new StringBuilder();
        for (int i = 0; i < foodItems.length; ++i) {
            // ---------------------------------
            // TODO: Guard against malicious input
            Pattern p = Pattern.compile("[; \r\n\t]");
            Matcher m = p.matcher(foodItems[i]);
            if (m.find())
                continue;
            // ---------------------------------
            // TODO: More advanced name matching?
            sqlCondition.append("name ILIKE '" + foodItems[i] + "'");
            if (i < foodItems.length - 1)
                sqlCondition.append(" OR ");
        }

        List<FriskForslagRecipeSpec> recipesToReturn = new ArrayList<>();
        try (
               /* Connection conn = connPool.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);*/
                Connection c = connPool.getConnection();
                PreparedStatement ps = c.prepareStatement(
                            """
                                    SELECT * FROM friskforslagrecipe AS rec\s
                                    INNER JOIN\s
                                    (
                                    SELECT
                                    ing.recipe_id,
                                    ARRAY_AGG(CONCAT_WS(',', food.name, ing.quantity, ing.unit)) AS ingrediens\s
                                    FROM friskforslagfooditem AS food\s
                                    INNER JOIN friskforslagingredient AS ing\s
                                    ON food.id = ing.fooditem_id\s
                                    WHERE\s
                                    """
                                    +
                                    sqlCondition
                                    +
                                    """
                                     GROUP BY ing.recipe_id\s
                                    ) AS matchedingredients\s
                                    ON rec.id = matchedingredients.recipe_id
                                    """);) {
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                String[] ingredients = (String[]) rs.getArray("ingrediens").getArray();
                List<String> ingredientNames = new ArrayList<>();
                List<Integer> ingredientQuantities = new ArrayList<>();
                List<String> ingredientUnits = new ArrayList<>();
                for (String s : ingredients) {
                    String[] s_split = s.split(",");
                    ingredientNames.add(s_split[0]);
                    ingredientQuantities.add(Integer.parseInt(s_split[1]));
                    ingredientUnits.add(s_split[2]);
                }


                recipesToReturn.add(new FriskForslagRecipeSpec(
                        new FriskForslagRecipe(
                                rs.getInt("recipe_id"),
                                rs.getString("name"),
                                rs.getString("descr"),
                                rs.getString("proc"),
                                rs.getInt("duration_in_minutes")
                        ),
                        ingredientNames,
                        ingredientQuantities,
                        ingredientUnits
                ));
            }
        } catch (SQLException e) {
            String msg = "Fejl ved søgning af passende opskrifter i databasen";
            throw new DatabaseException(msg, e.getMessage());
        }
        return recipesToReturn;
    }
}
