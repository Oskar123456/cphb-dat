package app.entities;

import java.util.List;

public record FriskForslagRecipeSpec(FriskForslagRecipe Rec,
                                     List<String> IngredientNames,
                                     List<Integer> IngredientQuantities,
                                     List<String> IngredientUnits)
{
}
