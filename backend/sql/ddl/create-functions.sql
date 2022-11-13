create or replace function get_unit_id_from_name(unit_name varchar(255))
returns integer
as $$
begin
  return (select id from units where units.name = unit_name limit 1);
end
$$
language plpgsql;

create or replace function get_amount_from_nominal(nominal integer, nominal_count integer)
returns integer
as $$
begin
  return nominal * nominal_count;
end
$$
language plpgsql;
