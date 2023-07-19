curl -X POST \
  -H "Content-Type: application/vnd.schemaregistry.v1+json" \
     --data '
 {
     "schema": "{ \"type\": \"object\", \"properties\": { \"_id\": { \"type\": \"integer\" }, \"firstname\": { \"type\": \"string\" }, \"lastname\": { \"type\": \"string\" }, \"age\": { \"type\": \"integet\" }, \"email\": { \"type\": \"string\" }, \"sex\": { \"type\": \"string\" } } }"
 }
' \
http://schema-registry:8081/subjects/test.users/versions
