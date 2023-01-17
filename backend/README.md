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

## Python
### Install
```shell
# cwd is in backend
pip install -r requirements.txt
```

### Config
Config is done via environment variables, which can be set system-wide or using a "dotenv"-file, named `.env` in the root of the backend folder.

`DB_LINK` is a URL to connect to the database:
```shell
postgresql+psycopg2://username:password@host:port/database
```

### Run
To launch the main server in dev-mode use:
```shell
uvicorn medlenno.main:app --reload
```
