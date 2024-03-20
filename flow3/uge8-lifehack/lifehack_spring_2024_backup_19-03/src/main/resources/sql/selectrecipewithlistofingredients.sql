SELECT * FROM
    friskforslagrecipe AS rec
        INNER JOIN
    (
        SELECT
            ing.recipe_id,
            ARRAY_AGG(food.name) AS ingrediens
        FROM friskforslagfooditem AS food
                 INNER JOIN friskforslagingredient AS ing
                            ON food.id = ing.fooditem_id
        WHERE name ILIKE 'Æble' OR name ILIKE 'poRre' OR name ILIKE 'aPPelSin'
        GROUP BY ing.recipe_id
    ) AS matchedingredients
    ON rec.id = matchedingredients.recipe_id;


SELECT * FROM
    friskforslagrecipe AS rec
        INNER JOIN
    (
        SELECT
            ing.recipe_id,
            ARRAY_AGG(CONCAT_WS(',', food.name, ing.quantity, ing.unit)) AS ingrediens
        FROM friskforslagfooditem AS food
                 INNER JOIN friskforslagingredient AS ing
                            ON food.id = ing.fooditem_id
        WHERE name ILIKE 'Æble' OR name ILIKE 'poRre' OR name ILIKE 'aPPelSin'
        GROUP BY ing.recipe_id
    ) AS matchedingredients
    ON rec.id = matchedingredients.recipe_id;