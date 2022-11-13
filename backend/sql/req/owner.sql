-- Cafe
insert into cafes(name, description, address)
values('first', null, 'belorusskaya');

update cafes set address = 'belorusskaya 6' where name = 'first';
delete from cafes where name = 'first';

-- Menu
insert into menus(name) values('first');

update menus set description = 'vegan' where name = 'first';
delete from menus where description = 'vegan';

-- Menu in cafe
insert into cafe2menu(cafe_id, menu_id)
values(
  (select id from cafes where name = 'first'),
  (select id from menus where name = 'first')
);
delete from cafe2menu
where
  cafe_id = (select id from cafes where name = 'first') and 
  menu_id = (select id from menus where name = 'first');

-- Recipes
insert into recipes(name) values('makaroni');
-- update
delete from recipes where name = 'makaroni';

-- Search suppliers requests
select prices.id, nominal, nominal_price, min_count, max_count, name, description, unit_id from prices 
join ingredients on ingredients.id = prices.ingredient_id
where
  supplier_id = (select id from suppliers where name = 'groceries');

-- Order from supplier
insert into orders(price_id, nominal_count, cafe_id, status)
values(
  (select id from prices limit 1),
  10,
  (select id from cafes limit 1),
  'paid'
);
update orders set status = 'received';

-- Stock
update stocks set expiry_date = clock_timestamp();
delete from stocks;
