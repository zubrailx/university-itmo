# Asm. Транслятор и модель.

* Кулаков Никита Васильевич, P33312
* `asm | acc | neum | hw | tick | struct | stream | port | prob2 `

## Язык программирования

``` ebnf
```

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
name: CI

on:
  push:
    branches:
      - 'main'

jobs:
  test:
    name: Run tests & display coverage
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
        with:
          fetch-depth: '1'

      - name: Setup python
        uses: actions/setup-python@master
        with:
          python-version: '3.10'

      - name: Pip install
        run: |
          pip install -r requirements.txt

      - name: Test
        run: |
          coverage run -m pytest --verbose
          find . -type f -name "*.py" | xargs -t python3 -m coverage report
          find . -type f -name "*.py" | xargs -t python3 -m pycodestyle --ignore=E501
          find . -type f -name "*.py" | xargs -t python3 -m pylint
```

где:

- `coverage` -- формирование отчёта об уровне покрытия исходного кода.
- `pytest` -- утилита для запуска тестов.
- `pep8` -- утилита для проверки форматирования кода. `E501` (длина строк) отключено, но не следует этим злоупотреблять.
- `pylint` -- утилита для проверки качества кода. Некоторые правила отключены в отдельных модулях с целью упрощения кода.
- Docker image `python-tools` включает в себя все перечисленные утилиты. Его конфигурация: [Dockerfile](./Dockerfile).

Пример использования и журнал работы процессора на примере `cat`:

``` console
> cd src/brainfuck
> cat examples/foo_input.txt
foo
> cat examples/cat.bf 
,[.,]
> ./translator.py examples/cat.bf target.out
source LoC: 1 code instr: 6
> cat target.out 
[
    {
        "opcode": "input",
        "term": [
            1,
            1,
            ","
        ]
    },
    {
        "opcode": "jz",
        "arg": 5,
        "term": [
            1,
            2,
            "["
        ]
    },
    {
        "opcode": "print",
        "term": [
            1,
            3,
            "."
        ]
    },
    {
        "opcode": "input",
        "term": [
            1,
            4,
            ","
        ]
    },
    {
        "opcode": "jmp",
        "arg": 1,
        "term": [
            1,
            5,
            "]"
        ]
    },
    {
        "opcode": "halt"
    }
]
> ./machine.py target.out examples/foo_input.txt
DEBUG:root:{TICK: 0, PC: 0, ADDR: 0, OUT: 0, ACC: 0} input  (',' @ 1:1)
DEBUG:root:input: 'f'
DEBUG:root:{TICK: 2, PC: 1, ADDR: 0, OUT: 102, ACC: 0} jz 5 ('[' @ 1:2)
DEBUG:root:{TICK: 4, PC: 2, ADDR: 0, OUT: 102, ACC: 102} print  ('.' @ 1:3)
DEBUG:root:output: '' << 'f'
DEBUG:root:{TICK: 6, PC: 3, ADDR: 0, OUT: 102, ACC: 102} input  (',' @ 1:4)
DEBUG:root:input: 'o'
DEBUG:root:{TICK: 8, PC: 4, ADDR: 0, OUT: 111, ACC: 102} jmp 1 (']' @ 1:5)
DEBUG:root:{TICK: 9, PC: 1, ADDR: 0, OUT: 111, ACC: 102} jz 5 ('[' @ 1:2)
DEBUG:root:{TICK: 11, PC: 2, ADDR: 0, OUT: 111, ACC: 111} print  ('.' @ 1:3)
DEBUG:root:output: 'f' << 'o'
DEBUG:root:{TICK: 13, PC: 3, ADDR: 0, OUT: 111, ACC: 111} input  (',' @ 1:4)
DEBUG:root:input: 'o'
DEBUG:root:{TICK: 15, PC: 4, ADDR: 0, OUT: 111, ACC: 111} jmp 1 (']' @ 1:5)
DEBUG:root:{TICK: 16, PC: 1, ADDR: 0, OUT: 111, ACC: 111} jz 5 ('[' @ 1:2)
DEBUG:root:{TICK: 18, PC: 2, ADDR: 0, OUT: 111, ACC: 111} print  ('.' @ 1:3)
DEBUG:root:output: 'fo' << 'o'
DEBUG:root:{TICK: 20, PC: 3, ADDR: 0, OUT: 111, ACC: 111} input  (',' @ 1:4)
DEBUG:root:input: '\n'
DEBUG:root:{TICK: 22, PC: 4, ADDR: 0, OUT: 10, ACC: 111} jmp 1 (']' @ 1:5)
DEBUG:root:{TICK: 23, PC: 1, ADDR: 0, OUT: 10, ACC: 111} jz 5 ('[' @ 1:2)
DEBUG:root:{TICK: 25, PC: 2, ADDR: 0, OUT: 10, ACC: 10} print  ('.' @ 1:3)
DEBUG:root:output: 'foo' << '\n'
DEBUG:root:{TICK: 27, PC: 3, ADDR: 0, OUT: 10, ACC: 10} input  (',' @ 1:4)
WARNING:root:Input buffer is empty!
INFO:root:output_buffer: 'foo\n'
foo

instr_counter:  15 ticks: 28
```

| ФИО           | алг.  | LoC       | code байт | code инстр. | инстр. | такт. | вариант |
|---------------|-------|-----------|-----------|-------------|--------|-------|---------|
| Преподавателя | hello | ...       | -         | ...         | ...    | ...   | ...     |
| Преподавателя | cat   | 1         | -         | 6           | 15     | 28    | ...     |
