-- Characteristic
-- NOTE: mb replace with enum
create table unit(
  id serial primary key,
  name varchar(255) not null unique
);

-- Core
create table supplier(
  id serial primary key,
  name varchar(255) not null,
  description text
);

create table cafe(
  id serial primary key,
  name varchar(255) not null,
  description text,
  address text not null
);

create table menu(
  id serial primary key,
  name varchar(255) not null,
  description text
);

create table recipe(
  id serial primary key,
  name varchar(255) not null,
  description text,
  calories integer
    check(calories >= 0)
);

create table ingredient(
  id serial primary key,
  name varchar(255) not null,
  description text,
  unit_id integer not null references unit
);

-- Associative
create table ingredient_price(
  id serial primary key,
  supplier_id integer not null references supplier
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredient
    on update cascade on delete restrict,
  amount integer not null
    check(amount >= 0),
  price integer not null
    check(price >=0)
);

create table recipe_item(
  id serial primary key,
  recipe_id integer not null references recipe
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredient
    on update cascade on delete restrict,
  amount integer default 1 not null
    check(amount >= 0),
  required boolean default false not null
);

create table stock(
  id serial primary key,
  cafe_id integer not null references cafe
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredient
    on update cascade on delete restrict,
  amount integer not null
    check(amount >= 0),
  bought_date timestamp with time zone not null,
  expiry_date timestamp with time zone not null,
  constraint stock_bought_expiry_date CHECK(bought_date < expiry_date)
);

create table cafe2menu(
  id serial primary key,
  cafe_id integer not null references cafe
    on update cascade on delete cascade,
  menu_id integer not null references menu
    on update cascade on delete restrict
);

create table menu_item(
  id serial primary key,
  menu_id integer not null references menu
    on update cascade on delete cascade,
  recipe_id integer not null references recipe
    on update cascade on delete restrict,
  price integer not null
    check(price >= 0)
);
