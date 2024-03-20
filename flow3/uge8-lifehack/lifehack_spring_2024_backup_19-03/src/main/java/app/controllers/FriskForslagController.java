package app.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import app.entities.FriskForslagFoodItem;
import app.entities.FriskForslagIngredient;
import app.entities.FriskForslagRecipe;
import app.entities.FriskForslagRecipeSpec;
import app.exceptions.DatabaseException;
import app.persistence.ConnectionPool;
import app.persistence.FriskForslagMapper;
import io.javalin.Javalin;
import io.javalin.http.Context;

public class FriskForslagController {
    public static void addRoutes(Javalin app, ConnectionPool connectionPool) {
        app.get("/friskforslag", ctx -> index(ctx, connectionPool));
        app.get("/friskforslag/index.html", ctx -> index(ctx, connectionPool));
        app.get("/friskforslag/search.html", ctx -> search(ctx, connectionPool));
        app.get("/friskforslag/search", ctx -> search(ctx, connectionPool));
    }

    private static void index(Context ctx, ConnectionPool connectionPool) {
        ctx.render("/friskforslag/index.html");
    }

    private static void search(Context ctx, ConnectionPool connectionPool) {
        String searchString = ctx.queryParam("ingredients");

        if (searchString != null && !searchString.isEmpty()) {
            String ingredientList[] = searchString.split(" ");
            List<FriskForslagRecipeSpec> recipeSpecs = null;

            try {
                recipeSpecs = FriskForslagMapper.ListRecipesWithNIngredients(connectionPool, ingredientList);
            } catch (DatabaseException e) {
                e.printStackTrace();
            }
            if (recipeSpecs == null || recipeSpecs.isEmpty()) {
                // TODO: handle empty recommended recipe list
            }
            else {
                ctx.attribute("recipeSpecs", recipeSpecs);
            }
        }

        ctx.render("/friskforslag/search.html");
    }
}
