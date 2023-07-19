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

## Добавление коннекторов и проверка их наличия

Добавляем коннекторы с помощью сервиса `basic-client`:

```sh
docker compose exec -it basic-client sh
cd ~
./source.sh
```

Для этого предназначен сервис `basic-client`. Внутри него маунтится директория connectors, если `docker-compose.yml` был запущен из той же директории, в которой он сам и находится.

Подключаемся к запущенному сервису:

```sh
docker compose exec -it basic-client sh
```

Проверяем коннекторы:

```sh
~ $ ./get-topics.sh
["mongodb-test-users-source"]
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

## Типы Документов

Source:

```
{
  "_id": Number(),
  "firstname": String(),
  "lastname": String(),
  "age": Number(),
  "email": String(),
  "sex": String()
}
```

Target:

```
{
  "_id": String(),
  "firstname": String(),
  "lastname": String(),
  "age": LongInt(),
  "email": String(),
}
```

### Вставка

Добавление данных в коллекцию users базы данных test:

```
db.users.insertOne({
  "_id": 1234131,
  "firstname": String("nikit"),
  "lastname": String("keadf"),
  "age": Number(99),
  "email": String("ndfaf"),
  "sex": String("sex")
})
```

Для `target` должно отобразиться ожидаемые результаты: дублирование.

```
db.users.find()
```

```
[
  {
    _id: 1234131,
    firstname: 'nikit',
    lastname: 'keadf',
    age: 99,
    email: 'ndfaf',
    sex: 'sex'
  }
]
```

### Удаление

Операция `drop` реализована, но не включена в связи ограничение драйвера mongoDB.

### Изменение

Операция `update` реализована.

```
db.users.updateOne( {},
[
  { $set: { firstname: "kesha" } },
])
```

Результат:

```
[
  {
    _id: 1234131,
    firstname: 'kesha',
    lastname: 'keadf',
    age: 99,
    email: 'ndfaf',
    sex: 'sex'
  }
]
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
