-- NOTE: if cafe_menu relations contain menu_id that are not present in table menu
create or replace function tf_cafe_menu_count()
returns trigger 
language plpgsql
as $$
begin
  select count(id) into NEW.menu_count from cafe2menu where cafe2menu.menu_id = NEW.id;
  return NEW;
end
$$; 

create or replace trigger tr_cafe_menu_count
before insert on cafes
for each row execute procedure tf_cafe_menu_count();


create or replace function tf_cafe_update_count()
returns trigger
language plpgsql
as $$
begin
  if (TG_OP = 'INSERT') then
    update cafes set menu_count = menu_count + 1 where id = NEW.cafe_id;
  elseif (TG_OP = 'DELETE') then
    update cafes set menu_count = menu_count - 1 where id = OLD.cafe_id;
    return OLD;
  end if;
  return NEW;
end
$$;

create or replace trigger tr_cafe_update_count
before insert or delete on cafe2menu
for each row execute procedure tf_cafe_update_count();


-- NOTE: if menu_item relations contain item_id that are not present in table menu
create or replace function tf_menu_recipe_count()
returns trigger 
language plpgsql
as $$
begin
  select count(id) into NEW.recipe_count from menu2recipe where menu2recipe.menu_id = NEW.id;
  return NEW;
end
$$; 

create or replace trigger tr_menu_recipe_count 
before insert on menus
for each row execute procedure tf_menu_recipe_count();


create or replace function tf_menu_update_count()
returns trigger
language plpgsql
as $$
begin
  if (TG_OP = 'INSERT') then
    update menus set recipe_count = recipe_count + 1 where id = NEW.menu_id;
  elseif (TG_OP = 'DELETE') then
    update menus set recipe_count = recipe_count - 1 where id = OLD.menu_id;
    return OLD;
  end if;
  return NEW;
end
$$;

create or replace trigger tr_menu_update_count
before insert or delete on menu2recipe
for each row execute procedure tf_menu_update_count();

-- Order trigger on table orders update
create or replace function tf_order_status_updated()
returns trigger
as $$
declare
  v_price record;
begin
  select * into v_price from prices where id = OLD.price_id limit 1;
  if (OLD.status = 'paid' and NEW.status = 'received') then
    insert into stocks(cafe_id, ingredient_id, amount, bought_date)
    values(
      NEW.cafe_id,
      v_price.ingredient_id,
      get_amount_from_nominal(v_price.nominal, NEW.nominal_count),
      clock_timestamp()
    );
  elsif (OLD.status = 'received' and NEW.status <> 'received') then
    raise notice 'Cannot update status of %, it is final', v_price.id;
    return OLD;
  end if;
  return NEW;
end
$$
language plpgsql;

create or replace trigger tr_insert_order_in_stock
after update of status on orders
for each row
execute procedure tf_order_status_updated();
