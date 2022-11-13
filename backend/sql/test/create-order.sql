insert into ingredients(name, description, unit_id) 
values('cucumber', 'small cucumber', get_unit_id_from_name('gram'));

insert into cafes(name, description, address)
values('first', null, 'belorusskaya');

insert into suppliers(name, description, address)
values('groceries', null, 'kronva');

insert into prices(supplier_id, ingredient_id, nominal, nominal_price, min_count, max_count)
values(
  (select id from suppliers where name = 'groceries' limit 1),
  (select id from ingredients where name = 'cucumber' and unit_id = get_unit_id_from_name('gram') limit 1),
  1000,
  100,
  1,
  null
); 

insert into orders(price_id, nominal_count, cafe_id, status)
values(
  (select id from prices limit 1),
  10,
  (select id from cafes limit 1),
  'paid'
);
