curl -X POST \
     -H "Content-Type: application/json" \
     --data '
{
  "name": "mongo-test-users-sink",
  "config": {
    "connector.class":"com.mongodb.kafka.connect.MongoSinkConnector",
    "connection.uri":"mongodb://mongodb-target:27017/?replicaSet=rs0",
    "database":"test",
    "collection":"users",
    "topics":"test.users",
    "writemodel.strategy":"com.mongodb.kafka.connect.sink.writemodel.strategy.ReplaceOneDefaultStrategy",
    "change.data.capture.handler": "com.mongodb.kafka.connect.sink.cdc.mongodb.ChangeStreamHandler"
  }
}
' \
http://kafka-connect:8083/connectors -w "\n"
