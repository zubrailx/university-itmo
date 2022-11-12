# Backend

## Database

```
$ psql -h pg -d studs

$ psql --version
psql (PostgreSQL) 14.2
```

### SQL naming

* tf - trigger function
* tr - trigger

### SQL script ordering

#### Creation:
* create-scheme.sql
* create-functions.sql
* create-procedures.sql
* create-triggers.sql

#### Dropping:
* drop-scheme.sql
* drop-triggers.sql
* drop-procedures.sql
* drop-functions.sql
