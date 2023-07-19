# Описание

## Предварительная настройка

Создание overlay сети:

```sh
docker swarm init
```

И пользовательской сети, которую будут использовать сервисы:

```sh
docker network create --driver overlay --attachable my-overlay
```

## Запуск `docker-compose.yml`

```sh
docker compose up
```

В случае, если не проинициализировался ReplicationSet, то перезапустить `mongodb-source-setup`.

```sh
docker compose restart mongodb-source-setup
```

Аналогично для ReplicationSet `mongodb-target-setup`.

## Добавление коннекторов и проверка их наличия

Добавляем коннекторы с помощью сервиса `basic-client`:

```sh
docker compose exec -it basic-client sh
cd ~
./source.sh
./target.sh
```

Для этого предназначен сервис `basic-client`. Внутри него маунтится директория connectors, если `docker-compose.yml` был запущен из той же директории, в которой он сам и находится.

Подключаемся к запущенному сервису:

```sh
docker compose exec -it basic-client sh
```

Проверяем коннекторы:

```sh
~ $ ./get-topics.sh
["mongodb-test-users-source","mongo-test-users-sink"]
```

## Работа с базой данных

Вход в базу данных

```sh
docker compose exec -it mongodb-<source/target> bash
```

* `mongodb-<source/target>` - база данных на выбор, где `source` источник

Подключение к `mongosh` аналогично:

```sh
mongosh
```

При входе появится:

```
rs0 [direct: primary] test>
```

### Вставка

Добавление данных в коллекцию users базы данных test:

```
db.users.insertOne({
  "_id": Number(1234131),
  "firstname": String("nikit"),
  "lastname": String("keadf"),
  "age": Number(99),
  "email": String("ndfaf")
})

db.users.insertOne({
  "firstname": String("nikit"),
  "lastname": String("keadf"),
  "age": Number(99),
  "email": String("ndfaf")
})
```

Для `target` должно отобразиться ожидаемые результаты: дублирование.

```
db.users.find()
```

### Удаление

Операция удаления `drop` не работает (ограничение драйвера mongoDB connector)

```
WARN Unsupported change stream operation: drop (com.mongodb.kafka.connect.sink.cdc.mongodb.ChangeStreamHandler)
```

### Изменение

```
db.users.updateOne( {},
[
  { $set: { firstname: "kesha" } },
])
```

### Прочие запросы

Агрегирование для получения типов и ключей элементов коллекции (без изменения бд):

```sh
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

## `kafka-ui`

Работает на порту `8080`.

## Приберемся за собой

Остановка, очистка volumes:

```sh
docker compose down --volumes
```

Покидаем swarm сеть:

```sh
docker swarm leave [--force]
```
