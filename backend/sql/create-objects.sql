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
);

create table ingredient(
  id serial primary key,
  name varchar(255) not null,
  description text,
  unit integer not null references unit
);

-- Associative
create table ingredient_price(
  id serial primary key,
  supplier_id integer not null references supplier,
  ingredient_id integer not null references ingredient,
  amount integer not null,
  price integer not null
);

create table recipe_item(
  id serial primary key,
  ingredient_id integer not null references ingredient,
  recipe_id integer not null references recipe,
  amount integer default 1 not null,
  required boolean default false not null
);

create table stock(
  id serial primary key,
  cafe_id integer not null references cafe,
  ingredient_id integer not null references ingredient,
  amount integer not null,
  bought_date timestamp with time zone not null,
  expiry_date timestamp with time zone not null
);

create table cafe2menu(
  id serial primary key,
  cafe_id integer not null references cafe,
  menu_id integer not null references menu
);

create table menu_item(
  id serial primary key,
  menu_id integer not null references menu,
  recipe_id integer not null references recipe,
  price integer not null
);
