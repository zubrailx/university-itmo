#!/bin/sh

DROP_FILES="drop-scheme.sql drop-procedures.sql drop-functions.sql"

for file in $DROP_FILES
do
        echo "Executing $file:"
        psql -h pg -d studs -f $file
done
