package app.entities;

public record FriskForslagIngredient(
        int Id,
        int RecipeId,
        int FoodItemId,
        int Quantity,
        String Unit) {
}
