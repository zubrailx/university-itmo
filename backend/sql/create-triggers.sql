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
