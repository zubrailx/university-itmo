create index idx_prices_nominal_price on prices using btree(nominal_price);
create index idx_stocks_expiry_date on stocks using btree(expiry_date);
create index idx_cafe_name on cafes using btree(name);
create index idx_menu_name on menus using btree(name);
create index idx_recipes_name on recipes using btree(name);
create index idx_ingredients_name on ingredients using btree(name);
