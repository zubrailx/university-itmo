-- Characteristic
-- NOTE: mb replace with enum
create table units(
  id serial primary key,
  name varchar(255) not null unique
);

create type order_status as enum();

-- Core
create table suppliers(
  id serial primary key,
  name varchar(255) not null,
  description text
);

create table cafes(
  id serial primary key,
  name varchar(255) not null,
  description text,
  address text not null,
  menu_count integer not null 
);

create table menus(
  id serial primary key,
  name varchar(255) not null,
  description text,
  recipe_count integer not null
);

create table recipes(
  id serial primary key,
  name varchar(255) not null,
  description text,
  calories integer
    check(calories >= 0)
);

create table ingredients(
  id serial primary key,
  name varchar(255) not null,
  description text,
  unit_id integer not null references units
    on update cascade on delete cascade
);

-- Associative
create table prices(
  id serial primary key,
  supplier_id integer not null references suppliers
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredients
    on update cascade on delete cascade,
  nominal integer not null
    check(nominal > 0),
  nominal_price integer not null
    check(nominal_price >= 0),
  min_count integer not null
    check(min_count > 0),
  max_count integer,
  constraint price_nominal_count check(min_count <= max_count or max_count is null)
);

create table orders(
  id serial primary key,
  price_id integer not null,
  nominal_count integer not null,
  stock_id integer not null,
  status order_status not null
);

create table recipe2ingr(
  id serial primary key,
  recipe_id integer not null references recipes
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredients
    on update cascade on delete cascade,
  amount integer default 1 not null
    check(amount >= 0),
  required boolean default false not null
);

create table stocks(
  id serial primary key,
  cafe_id integer not null references cafes
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredients
    on update cascade on delete cascade,
  amount integer not null
    check(amount >= 0),
  bought_date timestamp with time zone not null,
  expiry_date timestamp with time zone not null,
  constraint stock_bought_expiry_date check(bought_date < expiry_date)
);

create table cafe2menu(
  id serial primary key,
  cafe_id integer not null references cafes,
    -- on update cascade on delete cascade,
  menu_id integer not null references menus
    -- on update cascade on delete cascade
);

create table menu2recipe(
  id serial primary key,
  menu_id integer not null references menus
    on update cascade on delete cascade,
  recipe_id integer not null references recipes
    on update cascade on delete cascade,
  price integer not null
    check(price >= 0)
);

