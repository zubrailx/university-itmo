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
    "key.converter": "io.confluent.connect.avro.AvroConverter",
    "key.converter.schema.registry.url": "http://schema-registry:8081",
    "value.converter": "io.confluent.connect.avro.AvroConverter",
    "value.converter.schema.registry.url": "http://schema-registry:8081",
    "internal.key.converter": "org.apache.kafka.connect.json.JsonConverter",
    "internal.value.converter": "org.apache.kafka.connect.json.JsonConverter"
  }
}
' \
http://kafka-connect:8083/connectors -w "\n"
