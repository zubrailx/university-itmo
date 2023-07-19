curl -X POST \
     -H "Content-Type: application/json" \
     --data '
{
  "name": "mongodb-test-users-source",
  "config": {
    "connector.class": "com.mongodb.kafka.connect.MongoSourceConnector",
    "connection.uri": "mongodb://mongodb-source:27017/?replicaSet=rs0",
    "change.stream.full.document": "updateLookup",
    "startup.mode": "copy_existing",
    "database": "test",
    "collection": "users",
    "publish.full.document.only": "true",
    "output.format.keys": "json",
    "output.format.value": "json"
  }
}
' \
http://kafka-connect:8083/connectors -w "\n"
