# Заметки автора 

Дисклеймер: данный файл является обычным файлом-заметкой, создаваемой в процессе выполнения лабораторной работы. Можно считать, черновое руководство по фичам.

## Установка

* `docker exec` - execute command in running container
* `docker run` - create and run a new container from an image
* `docker pull` - download image from registry

You can choose not to set a value and pass the environment variables from your shell straight through to a service’s containers. It works in the same way as docker run -e VARIABLE ...:

```yaml
web:
  environment:
    - DEBUG
```

The value of the DEBUG variable in the container is taken from the value for the same variable in the shell in which Compose is run.

Substitution from shell:

```yaml
db:
  image: "postgres:${POSTGRES_VERSION}"
```

Для того, чтобы поддерживать overlay необходим Swarm mode Engine Docker.

https://docs.docker.com/engine/swarm/


If you want your containers to join a pre-existing network, use the external option

```yaml
services:
  # ...
  networks:
    network1:
        name: my-pre-existing-network
            external: true

```

## Connect to run shell in [service]

```sh
docker compose run bash [service]
docker compose exec -it mongodb-source bash
```

```sh
docker compose up -d
```

## Connect to mongodb

```sh
mongosh --username root --password secret
```

## List databases

```
test> use admin
switched to db admin
admin> db.adminCommand({listDatabases: 1})
{
  databases: [
    { name: 'admin', sizeOnDisk: Long("102400"), empty: false },
    { name: 'config', sizeOnDisk: Long("61440"), empty: false },
    { name: 'local', sizeOnDisk: Long("73728"), empty: false }
  ],
  totalSize: Long("237568"),
  totalSizeMb: Long("0"),
  ok: 1
}
```

or 

```
test> show dbs
```

## Create collection

```
db.createCollection("mycollection")
```

## Commands

```js
db.listCommands()
```

```js
use sample_mflix

db.movies.updateOne( { title: "Tag" },
{
  $set: {
    plot: "One month every year, five highly competitive friends hit the ground running for a no-holds-barred game of tag"
  }
  { $currentDate: { lastUpdated: true } }
})
```

Insert user:

```js
db.users.insertOne({
  "_id": Number(1234131),
  "firstname": String("nikit"),
  "lastname": String("keadf"),
  "age": Number(21),
  "email": String("ndfaf")
})
```

```js

```

Or use basic field type without decoration.

Get field types: key + keyType

```js
db.users.aggregate([
  { $sample: { size: 1 } },
  { $project: { 
    fields: { $objectToArray: "$$ROOT" } 
  } },
  { $unwind: "$fields" },
  { $group: {
    _id: "$fields.k",
    types: { $addToSet: { $type: "$fields.v" } }
  } }
]);
```

Result:

```
[
  { _id: 'age', types: [ 'int' ] },
  { _id: 'firstname', types: [ 'string' ] },
  { _id: 'email', types: [ 'string' ] },
  { _id: 'lastname', types: [ 'string' ] },
  { _id: '_id', types: [ 'int' ] }
]
```

Update:

```js```

Drop(unsupported by )

## Creating swarm network

```sh
docker swarm init
```

To join overlay network on other nodes use:

```sh
docker swarm join --token SWMTKN-1-0pscm94roochkymq20mv6ciz0fzkuxsq7e8wxa0xjiowj7l58e-b1fkhapij0h1xc152603mdtpc 192.168.197.72:2377
```

```sh
docker node ls
```

```
ID                            HOSTNAME   STATUS    AVAILABILITY   MANAGER STATUS   ENGINE VERSION
3klbdz5vu72c6xllcsn56ouuh *   host       Ready     Active         Leader           23.0.3
```

```sh
docker network ls
```

M

```
NETWORK ID     NAME                                    DRIVER    SCOPE
1983dcc26607   bridge                                  bridge    local
5d34907fc839   composetest_default                     bridge    local
8a55a2204244   docker_gwbridge                         bridge    local
d237864790e5   host                                    host      local
nyz6yciboa5z   ingress                                 overlay   swarm
e5597b1438fd   none                                    null      local
8f741fb787da   ravino-task_default                     bridge    local
f08aa9b82ae6   robot-platform_tinkoff-backend-bridge   bridge    local
```

* `docker service` - Manage swarm services:

```sh
docker service ps
```

```sh
docker inspect <container-id>
```

Under network we can see `ingress`. So other nodes can address containers that runs on different nodes.

To run containers in `n` nodes run:

```sh
docker service scale <name>=<N>
```

Id we access all services we will get different results, because network is balancing requests.

Ingress network by default doesn't allow standalone containers to connect to its network. To solve it we will use user-defined overlay network.

```sh
docker network create --driver overlay --attachable my-overlay
```

Now we can use it for standalone containers.

And also these containers can communicate with each-other. Test it:

```sh
docker exec app1 curl web
```

To clean everything:
```sh
docker rm -f app1
docker service rm web whoami
docker network rm my-overlay
docker swarm leave
```

## Configuring connectors

Connectors and tasks are logical units of work that run as a process. The process is called a worker in Kafka Connect. There are two modes for running workers:

* Standalone mode: Useful for development and testing Kafka Connect on a local machine. It can also be used for environments that typically use single agents (for example, sending web server logs to Kafka).
* Distributed mode: Runs Connect workers on multiple machines (nodes), which form a Connect cluster. Kafka Connect distributes running connectors across the cluster. You can add or remove nodes as your needs evolve.

So we select distributed mode becausse of using overlay network.

To do that let's install kafka-rest-

## Links

* https://github.com/provectus/kafka-ui/blob/master/documentation/compose/kafka-ui.yaml

* https://github.com/mongodb-university/kafka-edu/blob/main/docs-examples/mongodb-kafka-base/source_connector/simple_source.json
