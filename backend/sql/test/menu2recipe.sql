-- TEST menu2recipe
insert into menus(name)
values('1', '1');

insert into menus(name)
values('1', '1');

insert into recipes(name)
values('1');
insert into recipes(name)
values('1');
insert into recipes(name)
values('1');

select * from menus;
select * from recipes;

insert into menu2recipe(menu_id, recipe_id, price)
values(1, 1, 100);

insert into menu2recipe(menu_id, recipe_id, price)
values(1, 2, 100);

select * from menu2recipe;
select * from cafes;

delete from menu2recipe where recipe_id = 1;

select * from menu2recipe;
select * from menus;
