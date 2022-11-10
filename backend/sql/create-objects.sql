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
  address text not null,
  menu_count integer not null 
);

create table menu(
  id serial primary key,
  name varchar(255) not null,
  description text,
  recipe_count integer not null
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
    on update cascade on delete cascade
);

-- Associative
create table ingredient_price(
  id serial primary key,
  supplier_id integer not null references supplier
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredient
    on update cascade on delete cascade,
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
    on update cascade on delete cascade,
  amount integer default 1 not null
    check(amount >= 0),
  required boolean default false not null
);

create table stock(
  id serial primary key,
  cafe_id integer not null references cafe
    on update cascade on delete cascade,
  ingredient_id integer not null references ingredient
    on update cascade on delete cascade,
  amount integer not null
    check(amount >= 0),
  bought_date timestamp with time zone not null,
  expiry_date timestamp with time zone not null,
  constraint stock_bought_expiry_date CHECK(bought_date < expiry_date)
);

create table cafe2menu(
  id serial primary key,
  cafe_id integer not null references cafe,
    -- on update cascade on delete cascade,
  menu_id integer not null references menu
    -- on update cascade on delete cascade
);

create table menu2recipe(
  id serial primary key,
  menu_id integer not null references menu
    on update cascade on delete cascade,
  recipe_id integer not null references recipe
    on update cascade on delete cascade,
  price integer not null
    check(price >= 0)
);

-- Triggers & Functions

-- table cafe triggers
-- NOTE: if cafe_menu relations contain menu_id that are not present in table menu
create or replace function func_cafe_menu_count()
returns trigger 
language plpgsql
as $$
begin
  select count(id) into NEW.menu_count from cafe2menu where cafe2menu.menu_id = NEW.id;
  return NEW;
end
$$; 

create or replace trigger trigger_cafe_menu_count
before insert on cafe
for each row execute procedure func_cafe_menu_count();


create or replace function func_cafe_update_count()
returns trigger
language plpgsql
as $$
begin
  if (TG_OP = 'INSERT') then
    update cafe set menu_count = menu_count + 1 where id = NEW.cafe_id;
  elseif (TG_OP = 'DELETE') then
    update cafe set menu_count = menu_count - 1 where id = OLD.cafe_id;
    return OLD; -- we need to delete old otherwise no rows will be deleted
  end if;
  return NEW;
end
$$;

create or replace trigger trigger_cafe_update_count
before insert or delete on cafe2menu
for each row execute procedure func_cafe_update_count();


-- table menu triggers
-- NOTE: if menu_item relations contain item_id that are not present in table menu
create or replace function func_menu_recipe_count()
returns trigger 
language plpgsql
as $$
begin
  select count(id) into NEW.recipe_count from menu2recipe where menu2recipe.menu_id = NEW.id;
  return NEW;
end
$$; 

create or replace trigger trigger_menu_recipe_count 
before insert on menu
for each row execute procedure func_menu_recipe_count();


create or replace function func_menu_update_count()
returns trigger
language plpgsql
as $$
begin
  if (TG_OP = 'INSERT') then
    update menu set recipe_count = recipe_count + 1 where id = NEW.menu_id;
  elseif (TG_OP = 'DELETE') then
    update menu set recipe_count = recipe_count - 1 where id = OLD.menu_id;
    return OLD; -- we need to delete old otherwise no rows will be deleted
  end if;
  return NEW;
end
$$;

create or replace trigger trigger_menu_update_count
before insert or delete on menu2recipe
for each row execute procedure func_menu_update_count();
