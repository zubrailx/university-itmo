# Asm. Транслятор и модель.

* Кулаков Никита Васильевич, P33312
* `asm | acc | neum | hw | tick | struct | stream | port | prob2 `

## Язык программирования

<!-- EBNF: -->
<!-- ``` ebnf -->
<!-- newline = <unicode code point U+000A>; -->
<!-- ascii_char = <arbitrary ASCII character except newline>; -->

<!-- letter = ascii_char; -->
<!-- digit = "0" ... "9"; -->

<!-- line_comment = ";", { ascii_char }, newline; -->

<!-- int_lit = [ "+", "-" ], { digit }; -->
<!-- char_lit = "'", ascii_char, "'"; -->

<!-- program = { { newline }, line, { newline } }; -->

<!-- line = [ label_decl ], [ instruction ], [ line_comment ]; -->

<!-- label_decl = label ":"; -->
<!-- label = identifier; -->

<!-- identifier = letter, { letter | digit }; -->

<!-- instruction = operator, [ operand ]; -->
<!-- operator = no_arg_op | one_arg_op; -->

<!-- no_arg_op = "inc" | "dec" | "itoc" | "ctoi";  -->
<!-- one_arg_op = "add" | "sub" | "div" | "mod" | "mul" | "ld" | "st" | "cmp" | "je" | "jne" | "js" | "jmp" } "in" | "out"; -->

<!-- operand = direct_operand | indirect_operand; -->
<!-- direct_operand = label | immediate; -->
<!-- indirent_operand = "[", direct_operand, "]"; -->

<!-- immediate = int_lit | char_lit; -->
<!-- ``` -->

BNF:

```ebnf
<program> ::= <EOF> | <line_list> <EOF>
<line_list> ::= <line> <line_list> | <line>
<line> ::= <line_not_empty> | <line_not_empty> <EOL> | <EOL>
<line_not_empty> ::= <line_instruction> <comment> | <line_instruction> | <comment>
<line_instruction> ::= <label_decl> <instruction> | <label_decl> | <instruction>
<label_decl> ::= <label> ":"
<label> ::= <identifier>
<identifier> ::= <letter> | <letter> <letter_or_digit_list>
<comment> ::= ";" <char_not_eol_list>

<instruction> ::= <no_arg_instr> | <one_arg_instr>
<no_arg_instr> ::= <no_arg_op>
<one_arg_instr> ::= <one_arg_op> <operand>
<operand> ::= <indirect_operand> | <direct_operand>
<direct_operand> ::= <label> | <immediate>
<indirect_operand> ::= "[" <direct_operand> "]"
<immediate> ::= <int_lit> | <char_lit>

<int_lit> ::= <digit_list> | "-" <digit_list>
<digit_list> ::= <digit> <digit_list> | <digit>
<char_lit> ::= "'" <letter> "'" | "'" <digit> "'"

<letter_or_digit_list> ::= <letter_or_digit> | <letter_or_digit> <letter_or_digit_list>
<letter_or_digit> ::= <letter> | <digit>
<char_not_eol_list> ::= <letter_digit_space> | <letter_digit_space> <char_not_eol_list>
<letter_digit_space> ::= <letter> | <space>
<space> ::= " " | "\t"
<letter> ::= [a-z] | [A-Z]
<digit> ::= [0-9]
<EOF> ::= "1A"
<EOL> ::= "\n"

<no_arg_op> ::= "inc" | "dec" | "itoc" | "ctoi" 
<one_arg_op> ::= "add" | "sub" | "div" | "mod" | "mul" | "ld" | "st" | "cmp" | "je" | "jne" | "js" | "jmp" | "in" | "out"
```

### Метки
- `label: immediate` - создание метки на данные
- `label:` - создание метки (для выполнения иструкций перехода)

### Команды

* `inc` - инкрементировать значения в аккумуляторе.

|Instruction|Description |
|-----------|------------|
|`inc`      |AC := AC + 1|

* `dec` - декрементировать значение в аккумуляторе.

|Instruction|Description |
|-----------|------------|
|`dec`      |AC := AC - 1|

* `itoc` - перевести integer в код символа ASCII.

|Instruction|Description        |
|-----------|-------------------|
|`itoc`     |AC := AC + ord('0')|

* `ctoi` - перевести код символа ASCII в integer.

|Instruction|Description        |
|-----------|-------------------|
|`itoc`     |AC := AC - ord('0')|

* `add` - сложить значение в аккумуляторе с операндом.

|Instruction|Description     |
|-----------|----------------|
|`add m24`  |AC := AC + m24|
|`add imm24`|AC := AC + imm24|

* `sub` - вычесть значение в аккумуляторе с операндом.

|Instruction|Description     |
|-----------|----------------|
|`sub m24`  |AC := AC - m24|
|`sub imm24`|AC := AC - imm24|

* `div` - беззнаковое деление.

|Instruction|Description     |
|-----------|----------------|
|`div m24`  |AC := AC / m24|
|`div imm24`|AC := AC / imm24|

* `mod` - остаток от беззнакового деления.

|Instruction|Description     |
|-----------|----------------|
|`mod m24`  |AC := AC % m24|
|`mod imm24`|AC := AC % imm24|

* `mul` - беззнаковое умножение.

|Instruction|Description     |
|-----------|----------------|
|`mul m24`  |AC := AC * m24|
|`mul imm24`|AC := AC * imm24|

* `ld` - загрузить значение в аккумулятор (флаги не выставляются).

|Instruction|Description|
|-----------|-----------|
|`ld m24`   |AC := m24  |
|`ld imm24` |AC := imm24|

* `st` - загрузить значение в ячейку из аккумулятора.

|Instruction|Description  |
|-----------|-------------|
|`st imm24` |\*imm24 := AC|

* `cmp` - сравнить числа. Работает как `sub`, однако значение в аккумуляторе не модифицируется.

|Instruction|Description|
|-----------|-----------|
|`cmp m24`  |AC - m24   |
|`cmp imm24`|AC - imm24 |

* `je` - переход, если равно.

|Instruction|Description   |
|-----------|--------------|
|`je imm24` |Jump if equals|

* `jne` - переход, если не равно.

|Instruction|Description|
|-----------|-----------|
|`jne imm24`|           |

* `js` - переход, если знак (отрицательное число).

|Instruction|Description|
|-----------|-----------|
|`js imm24`|           |

* `jmp` - безусловный переход.

|Instruction|Description|
|-----------|-----------|
|`jmp imm8` |           |

* `in` - считать байт с порта в младший байт аккумулятора.

|Instruction|Description|
|-----------|-----------|
|`in imm8` | `imm8` - номер порта |

* `out` - записать младший байт аккумулятора на порт.

|Instruction|Description|
|-----------|-----------|
|`out imm8` | `imm8` - номер порта |

#### Примечание
* `m24` - адрес в памяти, из которого мы берем значение. Здесь также можно использовать метки, поскольку в отличие от обычного `asm`, в данной ЛР они имеют абсолютный адрес. Синтаксически на asm:

```asm
op [label]
op [addr]
```
* op - operation

## Организация памяти

Модель памяти процессора (фон-Неймана):

1. Память команд и данных. Машинное слово -- 24 бит, знаковое. Линейное адресное пространство. Реализуется списком чисел.
```text
     Memory (for data and instructions)
+-----------------------------+
|    ...                      |
|    ...                      |
+-----------------------------+
```

## Система команд


### Набор инструкции

| Syntax | Mnemonic | Кол-во тактов           | Comment |
|--------|----------|-------------------------|---------|
|        |          |                         |         |

### Кодирование инструкций

- Машинный код сериализуется в список JSON.
- Один элемент списка, одна инструкция.
- Адрес инструкции -- индекс списка + `0x4000`. Используется для команд перехода.

Пример:

```json
```

## Транслятор

## Модель процессора

### DataPath

``` text
```

### ControlUnit

``` text
```

## Апробация

В качестве тестов использованы алгоритмы:

1. fdskfaj

Интеграционные тесты: [integration_test]()

CI (Github):

``` yaml
# ./github/workflows/ci.yml
docker-test:
  name: Run tests & display coverage on Docker
  runs-on: ubuntu-latest
  container: zubrailx/ca-lab3:first
  steps:
    - uses: actions/checkout@v3
      with:
        fetch-depth: '1'

    - name: Test
      run: |
        ./test-run.sh


# ./test-run.sh
coverage run -m pytest --verbose
find src test -type f -name "*.py" | xargs -t python3 -m coverage report
find src test -type f -name "*.py" | xargs -t python3 -m pycodestyle --ignore=E501
find src test -type f -name "*.py" | xargs -t python3 -m pylint
```

где:

- `coverage` -- формирование отчёта об уровне покрытия исходного кода.
- `pytest` -- утилита для запуска тестов.
- `pep8` -- утилита для проверки форматирования кода. `E501` (длина строк) отключено, но не следует этим злоупотреблять.
- `pylint` -- утилита для проверки качества кода. Некоторые правила отключены в отдельных модулях с целью упрощения кода.
- Docker image `python-tools` включает в себя все перечисленные утилиты. Его конфигурация: [Dockerfile](./Dockerfile).

Пример использования и журнал работы процессора на примере `cat`:

``` console
```

| ФИО           | алг.  | LoC       | code байт | code инстр. | инстр. | такт. | вариант |
|---------------|-------|-----------|-----------|-------------|--------|-------|---------|
| Преподавателя | hello | ...       | -         | ...         | ...    | ...   | ...     |
