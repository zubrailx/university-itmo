-- Units
-- pounds
insert into units(name) values('pounds');

create or replace function pound_to_ounce(cnt double precision)
returns double precision
as $$
begin
  return cnt * 16;
end
$$
language plpgsql;

create or replace function ounce_to_pound(cnt double precision)
returns double precision
as $$
begin
  return cnt / 16;
end
$$
language plpgsql;

update units set name = 'pound' where name = 'pounds';

delete from units where name = 'pounds';
drop function if exists
  pound_to_ounce,
  ounce_to_pound
restrict;

-- Ingredients
insert into ingredients(name, description, unit_id) 
values('cucumber', 'small cucumber', get_unit_id_from_name('gram'));

update ingredients set description = null where name = 'cucumber';
delete from ingredients 
where name = 'cucumber' and unit_id = get_unit_id_from_name('gram');
