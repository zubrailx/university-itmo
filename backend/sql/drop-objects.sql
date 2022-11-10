-- NOTE: will be deleted with tables

-- drop trigger if exists
--   trigger_cafe_menu_count on cafe
-- cascade;

-- drop trigger if exists
--   trigger_menu_recipe_count on menu
-- cascade;
  
drop table if exists
  unit,
  supplier,
  cafe,
  menu,
  recipe,
  ingredient,
  ingredient_price,
  recipe_item,
  stock,
  cafe2menu,
  menu2recipe
cascade;

-- Functions
drop function if exists
  func_menu_recipe_count,
  func_cafe_menu_count
cascade;

