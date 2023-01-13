# Lab-2 QPG (query plan generator)

### Условия

* На равенство и неравенство для чисел, строк и булевских значений
* На строгие и нестрогие сравнения для чисел
* Существование подстроки
* Логическую комбинацию произвольного количества условий и булевских значений
* В качестве любого аргумента условий могут выступать литеральные значения (константы) или ссылки на значения, ассоциированные с элементами данных (поля, атрибуты, свойства)
* 

## Build
```sh
# Default build type - build

# Some aliases for cmake usage (read it to get actual commands)
./cmake.sh -h 

# Basic commands:
./cmake.sh gen [(debug|released|sanitized)] # generate build-tree + build (default to debug)

# Tests
./cmake.sh [build-type] {({test [sep]}|valgrind)}
# Sanitized
./cmake.sh gen sanitized && ./cmake.sh sanitized test

# Run executable (if app/main.c is present):
./build/(debug|released|sanitized)/bin/dbms_exec

# Crossdev
./cmake.sh released gen reset -v -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++

# Clean project:
./cmake.sh {build-type} clean

# Reset project (rm -rf):
./cmake.sh {build-type} reset

# Commands can be combined.
```

## Отчет

### Цели

Использовать средство синтаксического анализа по выбору, реализовать модуль для разбора некоторого достаточного подмножества языка запросов по выбору в соответствии с вариантом формы данных. Должна быть обеспечена возможность описания команд создания, выборки, модификации и удаления элементов данных.

### Задачи

* Выбор и изучение средства синтаксического анализа.
* Изучить синтаксис языка запросов и записать спецификацию для средства синтаксического анализа.
* Реализовать модуль, использующий средство синтаксического анализа для разбора языка запросов
* Реализовать тестовую программу для демонстрации работоспособности созданного модуля, принимающую на стандартный ввод текст запроса и выводящую на стандартный вывод результирующее дерево разбора или сообщение об ошибке
* Результаты тестирования представить в виде отчёта.

### Описание работы

Программа реализована в виде модуля: запрашивает у пользователя строку на ввод и обертку над Ast деревом для возвращения результаты. Код возврата — int. Не нуль — все плохо.

Пример реализации пользовательского ввода до EOF через stdin, stdout:

```c++
// main.cpp

int main() {
  std::string buf;
  std::string line;
  std::cout << "> ";
  while(getline(std::cin, line)) {
    buf.append(line);
    buf.append("\n");
    if (line.find(';') != std::string::npos) {
      AstWrapper ast_wr;
      int code = parse_input(buf, ast_wr);
      if (code) {
        std::cout << "ret_code: " << code << std::endl;
      } else {
        std::cout << ast_wr.ast->repr() << std::endl;
      }
      buf.clear();
      std::cout << "> ";
    }
  }
  return 0;
}
```
Пример использования:

```console
> select * from table1 where name = "first";
{ ast_type: 12, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 8, has_alias: false, name: table1 }, has_cond: true, cond: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 3, column: { ast_type: 2, table: '', column: 'name' } }, op_right: { ast_type: 10, stype: 0, res: { ast_type: 0, dtype: 0, val: 'first' } }, op_type: 0 } }
> skfjaksfjas dfkjsaf safas faskf;
1: error: syntax error
ret_code: 1
> select * from table1 where name = "first";
```

### Аспекты реализации

#### Структура модуля:

* `parse.l` — файл токенайзера (flex)
* `parse.y` — файл парсера (bison)
* `qpg.hpp` `qpg.cpp` — модуль, через который передает запрос и получаем дерево
* `ast.hpp` — реализация термов.

Каждый терм в результате разбора запроса выражается как объект абстрактного класса Ast, и содержит в себе какую-то из его реализаций.

#### Типы термов:

```c++
enum class AstType {
  VARIABLE,
  VARIABLE_LIST,

  COLUMN,
  COLUMN_LIST,

  COLUMN_VALUE,
  COLUMN_VALUE_LIST,

  COLUMN_TYPE,
  COLUMN_TYPE_LIST,

  TABLE,
  JOIN,
  STATEMENT,
  SUBQUERY,
  SELECT,
  UPDATE,
  DELETE,
  INSERT,
  DROP,
  CREATE
};
```

Подтипы термов для `WHERE`:

```c++
enum class StatementType { CONST, UNARY, BINARY, COLUMN };
```

Типы операций:

```c++
enum class OperationType { EQ, NEQ, LO, GR, LEQ, GEQ, NOT, AND, OR, IN };
```

Типы данных:

```c++
enum class DataType { STR, BOOL, DOUBLE, INT32 };
```

#### Пример реализации одного из терма:

```c++
class AstCreate : public Ast {
private:
  std::string m_table;
  std::unique_ptr<AstList<AstColumnType>> m_collist;

public:
  AstCreate(const char *table, AstList<AstColumnType> *collist)
      : Ast(AstType::CREATE) {
    m_table = std::string(table);
    m_collist = std::unique_ptr<AstList<AstColumnType>>(collist);
  }

  std::string repr() const {
    std::string out;
    out += "table: ";
    out += m_table;
    out += ", ";
    out += "collist: ";
    out += m_collist.get()->repr();
    return repr_extend(out);
  }
};
```

#### Из важного

Все операторы левоассоциативные.

### Примеры запросов

Select c cross_join:
```console
> select * from t1 cross_join t2;
{ ast_type: 12, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 9, jtype: 0, lsv: { ast_type: 8, has_alias: false, name: t1 }, rsv: { ast_type: 8, has_alias: false, name: t2 } }, has_cond: false }
```

Select c cross_join в виде подзапроса:
```console
> select * from t1 cross_join (select c1,c2 from t2 ) as v1;
{ ast_type: 12, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 9, jtype: 0, lsv: { ast_type: 8, has_alias: false, name: t1 }, rsv: { ast_type: 11, query: { ast_type: 12, column_list: { ast_type: 3, do_all: false, list: [ { ast_type: 2, table: '', column: 'c1' }, { ast_type: 2, table: '', column: 'c2' } ] }, table_ref: { ast_type: 8, has_alias: false, name: t2 }, has_cond: false }, has_alias: true, alias: v1 } }, has_cond: false }
```

Select с условиями:
```console
>  select * from t1 where c1 > t1.c1 and (c2 < 4.1 or ( c1 > 4 and c3 in "str" ));             
{ ast_type: 12, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 8, has_alias: false, name: t1 }, has_cond: true, cond: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 3, column: { ast_type: 2, table: '', column: 'c1' } }, op_right: { ast_type: 10, stype: 3, column: { ast_type: 2, table: 't1', column: 'c1' } }, op_type: 3 }, op_right: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 3, column: { ast_type: 2, table: '', column: 'c2' } }, op_right: { ast_type: 10, stype: 0, res: { ast_type: 0, dtype: 2, val: '4.100000' } }, op_type: 2 }, op_right: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 2, op_left: { ast_type: 10, stype: 3, column: { ast_type: 2, table: '', column: 'c1' } }, op_right: { ast_type: 10, stype: 0, res: { ast_type: 0, dtype: 3, val: '4' } }, op_type: 3 }, op_right: { ast_type: 10, stype: 3, column: { ast_type: 2, table: '', column: 'c3' } }, op_type: 7 }, op_right: { ast_type: 10, stype: 0, res: { ast_type: 0, dtype: 0, val: 'str' } }, op_type: 9 }, op_type: 8 }, op_type: 7 } }
```

Update:
```console
> update t1 set c1 = 'val' where true;
{ ast_type: 13, table: t1, column_list: { ast_type: 5, list: [ { ast_type: 4, column: { ast_type: 2, table: '', column: 'c1' }, value: { ast_type: 0, dtype: 0, val: 'val' } } ] }, has_cond: true, cond: { ast_type: 10, stype: 0, res: { ast_type: 0, dtype: 1, val: '1' } } }
```

Delete:
```console
> delete from t1;
{ ast_type: 14, table: t1, has_cond: false }
```

Insert:
```console
> insert into t1 values ("abc", true, 4.1, 4);
{ ast_type: 15, table: t1, collist: { ast_type: 3, do_all: true, list: [  ] }, vallist: { ast_type: 1, list: [ { ast_type: 0, dtype: 0, val: 'abc' }, { ast_type: 0, dtype: 1, val: '1' }, { ast_type: 0, dtype: 2, val: '4.100000' }, { ast_type: 0, dtype: 3, val: '4' } ] } }
> insert into t1 (c1, c2) values ("abc", true);
{ ast_type: 15, table: t1, collist: { ast_type: 3, do_all: false, list: [ { ast_type: 2, table: '', column: 'c1' }, { ast_type: 2, table: '', column: 'c2' } ] }, vallist: { ast_type: 1, list: [ { ast_type: 0, dtype: 0, val: 'abc' }, { ast_type: 0, dtype: 1, val: '1' } ] } }
```

Drop table:
```console
> drop table t1;
{ ast_type: 16, table: t1 }
```

Create table:
```console
{ ast_type: 16, table: t1 }
> create table t1 ( c1 bool, c2 int32, c3 double, c4 str);
{ ast_type: 17, table: t1, collist: { ast_type: 7, list: [ { ast_type: 6, column: { ast_type: 2, table: '', column: 'c1' }, type: 1 }, { ast_type: 6, column: { ast_type: 2, table: '', column: 'c2' }, type: 3 }, { ast_type: 6, column: { ast_type: 2, table: '', column: 'c3' }, type: 2 }, { ast_type: 6, column: { ast_type: 2, table: '', column: 'c4' }, type: 0 } ] } }
```

### Вывод

В ходе выполнения данной лабораторной работы я постарался разобраться с тем, как использовать bison, flex, а также особенностями различных парсеров, такими как парсеры Top-down, Bottom-up, LL, LR, LL, LL(k), что значат эти аббревиатуры.

P.S. Реализации находятся в директориях `src`, `include`.

P.P.S. Обновил код, чтобы можно было в одну строчку несколько запросов писать.

```console
> select * from t1; select * from t2;
{ ast_type: 8, list: [ { ast_type: 13, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 9, has_alias: false, name: t1 }, has_cond: false }, { ast_type: 13, column_list: { ast_type: 3, do_all: true, list: [  ] }, table_ref: { ast_type: 9, has_alias: false, name: t2 }, has_cond: false } ] }
```
