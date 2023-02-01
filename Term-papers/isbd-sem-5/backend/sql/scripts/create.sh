#!/bin/sh

CREATE_FILES="create-scheme.sql create-indexes.sql create-functions.sql create-procedures.sql init.sql"
for file in $CREATE_FILES
do
        echo "Executing $file:"
        psql -h pg -d studs -f $file
done
