## Connection

Connect to helios

```bash
ssh -p 2222 -L 9999:pg:5432 helios
```

Connect to helios

```bash
psql -h pg -d ucheb
```

## Examples

```sql
-- SHOW — show the value of a run-time parameter
show all;
show <variable>;
```

oid - object identifier 
relname - name of object

```sql
SELECT * FROM pg_attribute WHERE attrelid = 'mytable'::regclass;

SELECT * FROM pg_attribute -- stores information about table columns
  WHERE attrelid = (SELECT oid FROM pg_class WHERE relname = 'mytable');


SELECT * FROM pg_attribute -- stores information about table columns
  WHERE attrelid = (SELECT oid FROM pg_class WHERE relname = 'mytable');

```

The catalog `pg_class` catalogs tables and most everything else that has columns or is otherwise similar to a table.

https://www.postgresql.org/docs/current/catalog-pg-class.html

## Task(1)

### For Oracle Database

— Используя сведения из системных каталогов, получить информацию обо всех файлах данных, доступных для чтения и записи.

```
No. FILE#     CREATION_TIME     STATUS
 --- -----------   ----------------------  ------------------------------
   1 00000001      2014-09-10 00:00:00     ONLINE
```

Обошел все системные каталоги в PostreSQL, не нашел необходимой информации: https://www.postgresql.org/docs/current/catalogs.html

Однако, нашел требуемое для БД Oracle:

```
SELECT NAME,
    FILE#,
    STATUS 
  FROM   V$DATAFILE;
```

### For PostreSQL

Not found in system catalog.

# Task(2)

### For PostreSQL

```sql
select * from pg_attribute where attrelid = 'public.Н_ЛЮДИ'::regclass;
```


Schema is a collection of logical structures of data

```sql
select schema_name from information_schema.schemata ;

    schema_name
--------------------
 pg_catalog
 information_schema
 public
(3 строки)
```

### Database vs Schema

A database is the main container, it contains the data and log files, and all the schemas within it. You always back up a database, it is a discrete unit on its own.

Schemas are like folders within a database, and are mainly used to group logical objects together, which leads to ease of setting permissions by schema.


Select table with attribute names and their types.

```sql
select attrelid, attname, attnum from pg_attribute where attrelid = 'public.Н_ЛЮДИ'::regclass and attnum > 0;
```

List types in pg_catalog.

```sql
select oid,typname from pg_catalog.pg_type;
```

In addition to public and user-created schemas, each database contains a pg_catalog schema,


Combine:

```sql
select * 
from 
    (select attrelid, attname, attnum, atttypid, atttypmod
    from pg_attribute 
    where attrelid = 'public.Н_ЛЮДИ'::regclass and attnum > 0
    ) as a 
join 
    (select oid, typname
    from pg_catalog.pg_type
    ) as b on a.atttypid = b.oid;

```

Why atttypmod is different from \d+?
https://stackoverflow.com/questions/52376045/why-does-atttypmod-differ-from-character-maximum-length

```sql
nullif(information_schema._pg_char_max_length(atttypid, atttypmod), -1) as maxlen
```

```sql
select * 
from 
    (select attrelid, 
            attname, 
            attnum, 
            atttypid, 
            nullif(information_schema._pg_char_max_length(atttypid, atttypmod), -1) as maxlen,
            attnotnull
    from pg_attribute 
    where attrelid = 'public.Н_ЛЮДИ'::regclass and attnum > 0
    ) as a 
join 
    (select oid, typname
    from pg_catalog.pg_type
    ) as b on a.atttypid = b.oid;
```

```txt
 attrelid |    attname     | attnum | atttypid | maxlen | oid  |  typname
----------+----------------+--------+----------+--------+------+-----------
    39005 | ИД             |      1 |       23 |        |   23 | int4
    39005 | ФАМИЛИЯ        |      2 |     1043 |     25 | 1043 | varchar
    39005 | ИМЯ            |      3 |     1043 |     15 | 1043 | varchar
    39005 | ОТЧЕСТВО       |      4 |     1043 |     20 | 1043 | varchar
    39005 | ПИН            |      5 |     1043 |     20 | 1043 | varchar
    39005 | ИНН            |      6 |     1043 |     20 | 1043 | varchar
    39005 | ДАТА_РОЖДЕНИЯ  |      7 |     1114 |        | 1114 | timestamp
    39005 | ПОЛ            |      8 |     1042 |      1 | 1042 | bpchar
    39005 | МЕСТО_РОЖДЕНИЯ |      9 |     1043 |    200 | 1043 | varchar
    39005 | ИНОСТРАН       |     10 |     1043 |      3 | 1043 | varchar
    39005 | КТО_СОЗДАЛ     |     11 |     1043 |     40 | 1043 | varchar
    39005 | КОГДА_СОЗДАЛ   |     12 |     1114 |        | 1114 | timestamp
    39005 | КТО_ИЗМЕНИЛ    |     13 |     1043 |     40 | 1043 | varchar
    39005 | КОГДА_ИЗМЕНИЛ  |     14 |     1114 |        | 1114 | timestamp
    39005 | ДАТА_СМЕРТИ    |     15 |     1114 |        | 1114 | timestamp
    39005 | ФИО            |     16 |     1043 |     80 | 1043 | varchar
```

### Comments

```sql
select * from pg_description where objoid = 39005;
```

```txt
 objoid | classoid | objsubid |                description
--------+----------+----------+-------------------------------------------
  39005 |     1259 |        1 | Уникальный номер человека
  39005 |     1259 |        2 | Фамилия человека
  39005 |     1259 |        3 | Имя человека
  39005 |     1259 |        4 | Отчество человека
  39005 |     1259 |        5 | Номер страхового свидетельства ГПС
  39005 |     1259 |        6 | Идентификационный номер налогоплательщика
  39005 |     1259 |        7 | Дата рождения человека
  39005 |     1259 |        8 | Пол человека
  39005 |     1259 |        9 | Сведения из паспорта
```

### Constraints

```sql
select * from pg_constraint;
```

SET CONSTRAINTS определяет, когда будут проверяться ограничения в текущей транзакции. Ограничения IMMEDIATE проверяются в конце каждого оператора, а ограничения DEFERRED откладываются до фиксации транзакции.

How to get normal description of constraint: `pg_get_constraintdef`

```sql
SELECT table_constraints.constraint_name, check_clause, table_name FROM information_schema.table_constraints JOIN information_schema.check_constraints ON table_constraints.constraint_name = check_constraints.constraint_name;
```

```sql
select * from information_schema.check_constraints;
select * from information_schema.table_constraints;
```

Using pg_catalog

```sql
SELECT con.*
FROM pg_catalog.pg_constraint con
INNER JOIN pg_catalog.pg_class rel
   ON rel.oid = con.conrelid
INNER JOIN pg_catalog.pg_namespace nsp
   ON nsp.oid = connamespace
where rel.relname = 'prices';
```

```sql
select conname, pg_get_constraintdef(oid) from pg_catalog.pg_constraint where conname='price_nominal_count';
```

```txt
                   pg_get_constraintdef
-----------------------------------------------------------
 CHECK (((min_count <= max_count) OR (max_count IS NULL)))
```

The catalog pg_attrdef stores column default values. The main information about columns is stored in pg_attribute. Only columns for which a default value has been explicitly set will have an entry here.

If column has `pg_attribute.atthasdef = true` then check `pg_attrdef`. Definition of column is not check.

```sql
select pg_get_constraintdef(oid), * from pg_catalog.pg_attrdef;

select pg_get_expr(adbin, adrelid) from pg_catalog.pg_attrdef; -- returns type with `nextval(...)` 
```

Get contraints on rows

```sql
select  oid,  -- row identifier
        conname,
        conrelid,
        contype,
        pg_get_constraintdef(oid)
from pg_catalog.pg_constraint where conrelid = 892658;
```

### Combine


```sql
with TABLE_OID as (
    select 'prices'::regclass::oid
) 
select 
    attrelid, 
    attname,
    attnum,
    maxlen,
    atttypid,
    attnotnull,
    typname
from 
    (select attrelid, -- table identifier
            attname, 
            attnum, 
            atttypid, 
            nullif(information_schema._pg_char_max_length(atttypid, atttypmod), -1) as maxlen,
            attnotnull
    from pg_catalog.pg_attribute 
    where attrelid = TABLE_OID and attnum > 0
    ) as a 
join 
    (select oid as typeoid, 
            typname
    from pg_catalog.pg_type
    ) as b on a.atttypid = b.typeoid;
left join
    (select conname,
            contype,
            pg_get_constraintdef(oid) as condesc
    from pg_catalog.pg_constraint

```

### Procedure

You cannot assign multiple rows to a record type variable. 

```sql
create or replace procedure rshd_perform_lab1(table_name name)
language plpgsql
as $$
    declare 
        table_oid oid;
        trec record;
        buffer text;
        rbuf text;
    begin
        -- Get table oid to use in future
        select table_name::regclass::oid into table_oid;

        buffer := format(E'\nTaблица: %s\n\n', table_name);
        buffer := buffer || format(E'%-5s %-20s %-50s\n', 'No.', 'Имя столбца', 'Атрибуты');
        buffer := buffer || format(E'%-5s %-20s %-50s\n', '---', '-----------', '--------');

        -- Get information about table columns (common table expression)
        FOR trec in (
            with from_pg_attr as (
                select  
                    attrelid, -- table identifier
                    attname, 
                    attnum, 
                    atttypid, 
                    nullif(information_schema._pg_char_max_length(
                        atttypid, atttypmod), -1) as maxlen,
                    attnotnull
                from pg_catalog.pg_attribute 
                where attrelid = table_oid and attnum > 0
            ),
            -- Get information about column types
            from_pg_type as (
                select distinct  
                    oid as typeoid,
                    typname
                from pg_catalog.pg_type
                join
                    from_pg_attr on from_pg_attr.atttypid = pg_catalog.pg_type.oid
            ),
            -- Get information about column constraints
            from_pg_constr as (
                select   
                    -- oid, -- row identifier
                    conrelid, -- table
                    unnest(conkey) unnested_key, -- column number
                    conname,
                    -- contype,
                    pg_get_constraintdef(oid) constraintdef
                from pg_catalog.pg_constraint 
                where 
                    conrelid = table_oid and contype = 'c' -- Select only checks
                order by unnested_key
            ), 
            -- Get description
            from_pg_descr as (
                select
                    objoid, -- refers to the table
                    objsubid, -- column
                    description
                from pg_catalog.pg_description
                where objoid = table_oid
            )
            -- Combine received result
            -- select * from from_pg_type
            select 
                -- attrelid,
                attnum,
                attname,
                -- atttypid,
                typname,
                maxlen,
                attnotnull,
                array_agg(conname) conname_list,
                array_agg(constraintdef) constraintdef_list,
                description
            from 
                from_pg_attr pgattr
            join
                from_pg_type pgtype on pgattr.atttypid = pgtype.typeoid
            left join
                from_pg_constr pgconstr on 
                    pgattr.attrelid = pgconstr.conrelid and 
                    pgattr.attnum = pgconstr.unnested_key
            left join
                from_pg_descr pgdescr on 
                    pgattr.attrelid = pgdescr.objoid and
                    pgattr.attnum = pgdescr.objsubid
            group by attnum, attname, typname, maxlen, attnotnull, description
            order by attnum
            )
        -- Loop for each table_record
        LOOP
            rbuf := format('%-5s %-20s', trec.attnum, trec.attname);
            -- Type
            rbuf := rbuf || format(E' %-10s: %s', 'Type', trec.typname);
            if trec.maxlen is not null then
                rbuf := rbuf || format('(%s)', trec.maxlen);
            end if;
            -- not nulL
            if trec.attnotnull is true then
                rbuf := rbuf || ' NOT NULL';
            end if;
            -- Constraints
            -- Comments
            if trec.descriptions is not null then
                rbuf := rbuf || format(E'%-5s %-20s %-10s: %s', 
                '', '', 'Description', trec.description);
            end if;

            buffer := buffer || rbuf || E'\n';
        end LOOP;

        raise notice '%', buffer;
    end;
$$;
```
