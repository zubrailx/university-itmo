curl -X POST \
  -H "Content-Type: application/vnd.schemaregistry.v1+json" \
     --data '
 {
     "schema": "{ \"type\": \"record\", \"name\": \"user\", \"fields\": [ { \"name\": \"_id\", \"type\": \"int\"}, { \"name\": \"firstname\", \"type\": \"string\"}, { \"name\": \"lastname\", \"type\": \"string\"}, { \"name\": \"age\", \"type\": \"int\"}, { \"name\": \"email\", \"type\": \"string\"}, { \"name\": \"sex\", \"type\": \"string\"} ] }"
 }
' \
http://schema-registry:8081/subjects/test.users/versions
