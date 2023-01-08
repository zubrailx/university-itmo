# itmo_llp-lab3

База данных с клиент-серверным взаимодействием.

### Build system

```sh
# Default build type - build

# Some aliases for cmake usage (read it to get actual commands)
./cmake.sh -h 

# Basic commands:
./cmake.sh gen [(debug|release|sanitized)] # generate build-tree + build (default to debug)

# Tests
./cmake.sh [build-type] {({test [sep]}|valgrind)}
# Sanitized
./cmake.sh gen sanitized && ./cmake.sh sanitized test

# Run executable (if app/main.c is present):
./build/(debug|release|sanitized)/bin/dbms_exec

# Crossdev
./cmake.sh release gen reset -v -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++

# Clean project:
./cmake.sh {build-type} clean

# Reset project (rm -rf):
./cmake.sh {build-type} reset

# Commands can be combined.
```

### Цели

Используя созданные в результате выполнения заданий модули, разработать в виде консольного приложения две программы: клиентскую и серверную части. Серверная часть – получающая по сети запросы и операции описанного формата и последовательно выполняющая их над файлом данных с помощью модуля из первого задания. Имя фала данных для работы получать с аргументами командной строки, создавать новый в случае его отсутствия. Клиентская часть – в цикле получающая на стандартный ввод текст команд, извлекающая из него информацию о запрашиваемой операции с помощью модуля из второго задания и пересылающая её на сервер с помощью модуля для обмена информацией, получающая ответ и выводящая его в человеко- понятном виде в стандартный вывод.

### Задачи

* Изучить выбранную библиотеку.
* На основе существующей библиотеки реализовать модуль, обеспечивающий взаимодействие
* Реализовать серверную часть в виде консольного приложения
* Реализовать клиентскую часть в виде консольного приложения
* Результаты тестирования представить в виде отчёта

### Описание работы

Программа состоит из двух исполняемых модулей: клиентского и серверного. Передавая различные аргументы клиенту и серверу при старте приложения, можно конфигурировать их работу. С каждым запросом от клиента приходит строка, нераспаршенное ast дерево, затем на сервере она парсится, производится проверка на ошибки, создается, если не существует база данных, результат возвращается клиенту.

Аргументы командной строки для клиента:

```console
% ./build/sanitized/client/client_exec -h
Client options:
  -h [ --help ]                        Show help
  -a [ --address ] arg (=localhost)    Server address
  -p [ --port ] arg (=6543)            Server port
  -d [ --database ] arg (=.tmp-db.bin) Database file
```

Сервер:

```console
% ./build/sanitized/server/server_exec -h
Client options:
  -h [ --help ]                   Show help
  -p [ --port ] arg (=6543)       Server port
  -a [ --address ] arg (=0.0.0.0) Listened address
```

#### Проверка на ошибки

Реализованы следующие проверки на ошибки:

* Возможность открытия базы данных;
* Валидность запроса на уровне парсинга;
* Наличие таблицы в базе данных;
* Наличие соответствующих колонок в таблице;
* Соответствие требуемых типов для операторов, присваивании значения колонки;
* Возможность использования функциональности, реализованной в парсинге, но нереализованной в модуле из первой лр (такой как проекция);

### Аспекты реализации

Структура проекта:

* `client` - клиенсткое приложение
* `server` - серверное приложение
* `qpg` - (query plan generator) - получено в результате выполнения ЛР 2
* `dbms` - (database manager) - получено в результате выполнения ЛР 2
* `proto` - описанные protobuf сообщения

Для линковки модулей друг с другом используется CMake.

Поскольку обмен сообщениями происходит посредством server-side streaming, то опишу порядок сообщений, которые ожидает клиент.

1) 1 сообщение с описанием ошибки и/или хедером таблицы.
2) от 0 сообщений с содержимым таблицы.
3) Результат повторить, так как может быть в момент отправлено несколько сообщений за раз (почему - детали реализации ЛР-2).

#### Используемые сторонние модули

* `gRPC` (fetched) - библиотека, реализованная поверх HTTP/2, который в свою очередь реализуется поверх TCP/IP для осуществления клиент-серверного взаимодействия.

  * Поддерживает кодогенерацию на основе описанных protobuf сообщений.
  * Клиент-серверное взаимодействие реализуется поверх Server-side stream в качестве практики
  * Удаленный вызов функций.

* `fmt` (fetched) - форматирование сообщений.
* `protobuf` (installed) - генерация кода на C++ для сообщений.
* `boost` (installed) - форматирование аргументов командной строки, algorithm.

### Результаты

#### Ошибки

Парсер:

```console
Created channel on target: localhost:6543.
Working with db: .tmp-db.bin.
> create table t1 fkasjfsafsa fasfsafasf;
ERROR(code=3): 1: error: syntax error
>
```

Наименование колонок - должно быть указано имя таблицы:

```console
> select * from t1 where c1 > 42;
Error: Table name should be present
```

Тип колонок - не реализован оператор `bool (eq*)(bool, int32)`:

```console
> select * from t1 where t1.c1 > 42;
Error: Unsupported op type: 3, (4, 1)
```

Нереализованная функциональной, которую позволяет описывать парсер:

```console
> select c1 from t1;
Error: Projection in select is not supported
```

Несуществующие колонки в таблице:

```console
> select * from t1 where t1.not_present and true;
Error: Column not found in table.
```

> После возникновения ошибки сервер продолжает работать

#### Безошибочные запросы

Создание таблицы:

```console
> create table t2 (c1 bool, c2 int32, c3 double, c4 str);
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_CREATE",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "bool"
  },
  {
   "columnName": "c2",
   "columnType": "int32"
  },
  {
   "columnName": "c3",
   "columnType": "double"
  },
  {
   "columnName": "c4",
   "columnType": "string"
  }
 ]
}
```

Удаление таблицы:

```console
> drop table t1;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_DROP"
}
```

Вставка в таблицу:

```console
> insert into t1 values (52, "sdkaskfjsafsa");
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_INSERT",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "string"
  }
 ]
}
TABLE BODY:
{
 "columns": [
  {
   "columnValue": "52"
  },
  {
   "columnValue": "sdkaskfjsafsa"
  }
 ]
}
> insert into t1 (c1) values (555555);
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_INSERT",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "string"
  }
 ]
}
TABLE BODY:
{
 "columns": [
  {
   "columnValue": "555555"
  },
  {}
 ]
}
```

Выборка (select+join):

```console
> select * from t1 cross_join t2;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_SELECT",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "bool"
  },
  {
   "columnName": "c1",
   "columnType": "bool"
  },
  {
   "columnName": "c2",
   "columnType": "int32"
  },
  {
   "columnName": "c3",
   "columnType": "double"
  },
  {
   "columnName": "c4",
   "columnType": "string"
  }
 ]
}
TABLE BODY:
{
 "columns": [
  {
   "columnValue": "5555"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
{
 "columns": [
  {
   "columnValue": "1111"
  },
  {
   "columnValue": "true"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
```

Выборка с подзапросом:

```console
> select * from t1 cross_join (select * from t1 cross_join t2) as v1;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_SELECT",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "bool"
  },
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "bool"
  },
  {
   "columnName": "c1",
   "columnType": "bool"
  },
  {
   "columnName": "c2",
   "columnType": "int32"
  },
  {
   "columnName": "c3",
   "columnType": "double"
  },
  {
   "columnName": "c4",
   "columnType": "string"
  }
 ]
}
TABLE BODY:
{
 "columns": [
  {
   "columnValue": "5555"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "5555"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
{
 "columns": [
  {
   "columnValue": "5555"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "1111"
  },
  {
   "columnValue": "true"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
{
 "columns": [
  {
   "columnValue": "1111"
  },
  {
   "columnValue": "true"
  },
  {
   "columnValue": "5555"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
{
 "columns": [
  {
   "columnValue": "1111"
  },
  {
   "columnValue": "true"
  },
  {
   "columnValue": "1111"
  },
  {
   "columnValue": "true"
  },
  {
   "columnValue": "false"
  },
  {
   "columnValue": "0"
  },
  {
   "columnValue": "0.000000"
  },
  {}
 ]
}
```

Выборка колонка с колонкой:

```console
> select * from t3 cross_join t3 as t3a where t3.c1 in t3a.c1;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_SELECT",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "string"
  },
  {
   "columnName": "c1",
   "columnType": "string"
  }
 ]
}
TABLE BODY:
{
 "columns": [
  {
   "columnValue": "12345"
  },
  {
   "columnValue": "1234567"
  }
 ]
}
```

Изменение:

```console
> update t1 set c2=false where t1.c2 = true;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_UPDATE",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "int32"
  },
  {
   "columnName": "c2",
   "columnType": "bool"
  }
 ]
}
```

Удаление:

```console
> delete from t3;
TABLE HEADER:
{
 "queryType": "QUERY_TYPE_DELETE",
 "columns": [
  {
   "columnName": "c1",
   "columnType": "string"
  }
 ]
}
```

### Примечание

Грамматику и соответственно команды можно посмотреть в `qpg/src/parse.l` и `qpg/src/parse.y`.

### Вывод

В ходе выполнения данной лабораторной работы настрадался с move-семантикой и шаблонами, впервые поиспользовал `boost`, научился нормально писать (вроде бы) cmake, разобрался с библиотекой `gRPC`.
