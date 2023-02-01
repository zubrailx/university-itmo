-- TEST cafe2menu
insert into cafes(name, address)
values('1', '1');

insert into cafes(name, address)
values('1', '1');

insert into menus(name)
values('1');
insert into menus(name)
values('1');
insert into menus(name)
values('1');

select * from cafes;
select * from menus;

insert into cafe2menu(cafe_id, menu_id)
values(1, 1);

insert into cafe2menu(cafe_id, menu_id)
values(1, 2);

select * from cafe2menu;
select * from cafes;

delete from cafe2menu where menu_id = 1;

select * from cafe2menu;
select * from cafes;

