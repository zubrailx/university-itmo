-- TEST cafe2menu
insert into cafe(name, address)
values('1', '1');

insert into cafe(name, address)
values('1', '1');

insert into menu(name)
values('1');
insert into menu(name)
values('1');
insert into menu(name)
values('1');

select * from cafe;
select * from menu;

insert into cafe2menu(cafe_id, menu_id)
values(1, 1);

insert into cafe2menu(cafe_id, menu_id)
values(1, 2);

select * from cafe2menu;
select * from cafe;

delete from cafe2menu where menu_id = 1;

select * from cafe2menu;
select * from cafe;

