alter type order_status add value 'paid';
alter type order_status add value 'received';

insert into units(name) values('milliliter');
insert into units(name) values('unit'); -- кол-во штук

insert into units(name) values('gram');

-- ounce
insert into units(name) values('ounce');

create or replace function ounce_to_gram(cnt double precision)
returns double precision
as $$
begin
  return cnt * 28.35;
end
$$
language plpgsql;

create or replace function gram_to_ounce(cnt double precision)
returns double precision
as $$
begin
  return cnt / 28.35;
end
$$
language plpgsql;

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
