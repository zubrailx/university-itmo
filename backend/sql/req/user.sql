-- Show recipes in menu in cafe
select recipes.name, recipes.calories, menu2recipe.price 
from 
  cafe2menu
  join menus on menus.id = cafe2menu.menu_id
  join menu2recipe on menu2recipe.menu_id = menus.id
  join recipes on recipes.id = menu2recipe.recipe_id
where
  cafe_id = 1;

-- Show ingredients of recipes
select ingredients.name, ingredients.description, recipe2ingr.amount, recipe2ingr.required
from
  recipes
  join recipe2ingr on recipe2ingr.recipe_id = recipes.id
  join ingredients on ingredients.id = recipe2ingr.ingredient_id
where
  recipes.name = 'makaroni';
