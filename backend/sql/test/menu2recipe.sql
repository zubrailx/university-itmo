-- TEST menu2recipe
insert into menu(name)
values('1', '1');

insert into menu(name)
values('1', '1');

insert into recipe(name)
values('1');
insert into recipe(name)
values('1');
insert into recipe(name)
values('1');

select * from menu;
select * from recipe;

insert into menu2recipe(menu_id, recipe_id, price)
values(1, 1, 100);

insert into menu2recipe(menu_id, recipe_id, price)
values(1, 2, 100);

select * from menu2recipe;
select * from cafe;

delete from menu2recipe where recipe_id = 1;

select * from menu2recipe;
select * from menu;
