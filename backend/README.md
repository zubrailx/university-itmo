# Backend

## Database

```
$ psql -h pg -d studs

$ psql --version
psql (PostgreSQL) 14.2
```

### SQL naming

* tf - trigger procedure
* tr - trigger

### SQL script ordering

#### Creation:
* create-scheme.sql
* create-indexes.sql
* create-functions.sql
* create-procedures.sql

#### Dropping:
* drop-scheme.sql
* drop-procedures.sql
* drop-functions.sql
