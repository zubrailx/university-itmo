-- Supplier account
insert into suppliers(name, description, address)
values('groceries', null, 'kronva');

update suppliers set description = 'groc' where name = 'groceries';
delete from suppliers where name = 'groceries';

-- Prices
insert into prices(supplier_id, ingredient_id, nominal, nominal_price, min_count, max_count)
values(
  (select id from suppliers where name = 'groceries' limit 1),
  (select id from ingredients where name = 'cucumber' and unit_id = get_unit_id_from_name('gram')),
  1000,
  100,
  1,
  null
); 
update prices set nominal_price = 120 
where 
  ingredient_id = (select id from ingredients where name = 'cucumber' and 
  unit_id = get_unit_id_from_name('gram'));

delete from prices 
where 
  ingredient_id = (select id from ingredients where name = 'cucumber' and 
    unit_id = get_unit_id_from_name('gram')) and
  supplier_id = (select id from suppliers where name = 'groceries' limit 1);
