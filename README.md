# Asm. Транслятор и модель.

* Кулаков Никита Васильевич, P33312
* `asm | acc | neum | hw | tick | struct | stream | port | prob2 `

## Язык программирования

BNF:

```ebnf
<program> ::= <EOF> | <section_list> <EOF>
<section_list> ::= <section> <section_list> | <section>
<section> ::= "section" <identifier> <line_list>
<line_list> ::= <line> <line_list> | <line>
<line> ::= <line_not_empty> | <line_not_empty> <EOL> | <EOL>
<line_not_empty> ::= <line_instruction> <comment> | <line_instruction> | <comment> | <section>
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

<no_arg_op> ::= "inc" | "dec" | "itoc" | "ctoi" | "halt"
<one_arg_op> ::= "add" | "sub" | "div" | "mod" | "mul" | "ld" | "st" | "cmp" | "je" | "jne" | "js" | "jmp" | "in" | "out"
```

### Секции

- `section <name>` - создать секцию с именем `<name>`. Данные внутри секций идут последовательно.
- `.text` - секция для кода
- `.data` - секция для данных

### Метки
- `label:` - метка (для выполнения иструкций перехода).
- `_start:` - обязательная метка, сигнализирующая начало программы, может находиться где угодно, главное чтобы указывала на инструкцию.
- На одни и те же инструкции (команды и данные) могут быть указаны несколько меток, однако метки не могут дублироваться, и одни и те же метки указывать на разные инструкции.
- Область видимости глобальная.

### Команды

* `inc` - инкрементировать значения в аккумуляторе.

| Instruction | Description  |
|-------------|--------------|
| `inc`       | AC := AC + 1 |

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
|`add m32`  |AC := AC + m32|
|`add imm32`|AC := AC + imm32|

* `sub` - вычесть значение в аккумуляторе с операндом.

|Instruction|Description     |
|-----------|----------------|
|`sub m32`  |AC := AC - m32|
|`sub imm32`|AC := AC - imm32|

* `div` - беззнаковое деление.

|Instruction|Description     |
|-----------|----------------|
|`div m32`  |AC := AC / m32|
|`div imm32`|AC := AC / imm32|

* `mod` - остаток от беззнакового деления.

|Instruction|Description     |
|-----------|----------------|
|`mod m32`  |AC := AC % m32|
|`mod imm32`|AC := AC % imm32|

* `mul` - беззнаковое умножение.

|Instruction|Description     |
|-----------|----------------|
|`mul m32`  |AC := AC * m32|
|`mul imm32`|AC := AC * imm32|

* `ld` - загрузить значение в аккумулятор (флаги не выставляются).

|Instruction|Description|
|-----------|-----------|
|`ld m32`   |AC := m32  |
|`ld imm32` |AC := imm32|

* `st` - загрузить значение в ячейку из аккумулятора.

|Instruction|Description  |
|-----------|-------------|
|`st imm32` |\*imm32 := AC|

* `cmp` - сравнить числа. Работает как `sub`, однако значение в аккумуляторе не модифицируется.

| Instruction | Description |
|-------------|-------------|
| `cmp m32`   | AC - m32    |
| `cmp imm32` | AC - imm32  |

* `je` - переход, если равно.

|Instruction|Description   |
|-----------|--------------|
|`je imm32` |Jump if equals|

* `jne` - переход, если не равно.

|Instruction|Description|
|-----------|-----------|
|`jne imm32`|           |

* `jb` - переход, если знак (отрицательное число).

|Instruction|Description|
|-----------|-----------|
|`jb imm32`|           |

* `jbe` - переход, если не знак.

|Instruction|Description|
|-----------|-----------|
|`jbe imm32`|           |

* `jg` - переход, если знак (отрицательное число).

|Instruction|Description|
|-----------|-----------|
|`jg imm32`|           |

* `jge` - переход, если не знак.

|Instruction|Description|
|-----------|-----------|
|`jge imm32`|           |

* `jmp` - безусловный переход.

|Instruction|Description|
|-----------|-----------|
|`jmp imm32` |           |

* `in` - считать байт с порта в младший байт аккумулятора.

|Instruction|Description|
|-----------|-----------|
|`in imm32` | `imm32` - номер порта |

* `out` - записать младший байт аккумулятора на порт.

|Instruction|Description|
|-----------|-----------|
|`out imm32` | `imm32` - номер порта |

* `halt` - остановка программы.

|Instruction|Description|
|-----------|-----------|
|`halt`     |           |

#### Примечание
* `m32` - адрес в памяти, из которого мы берем значение. Здесь можно использовать метки, в отличие от обычного `asm` в данной ЛР они имеют абсолютный адрес. Синтаксически на asm:

```asm
cmd [label] ; [label] - m32
cmd label ; label - imm32

cmd - команда
```

* `imm32` - неизменяемое число, будь то константа или адрес метки, который может быть получен во время генерации кода.

### Комментарии
* `; здесь комментарий` - однострочный комментарий до конца строки

### Константы
* `cmd/var: <int_lit>` - команда с аргументом или переменная с целочисленным значением
* `cmd/var: '<char_lit>'` - символ ASCII. Поддерживаются управляющие символы `\n`, `\q`, `\r`, `\t`.

### Массивы

* Не реализованы, но и не требовалось.

## Организация памяти

### Работа с памятью

Модель памяти процессора (фон-Неймана):

1. Память команд и данных. Машинное слово -- 32 бит, знаковое. Линейное адресное пространство. Реализуется списком чисел и инструкций, идущих друг за другом с интервалом в 32 бита.

2. Отдельного разделения на память команд и данных нет. При попытке исполнения инструкции переменной программа падает на исключении.

3. Scope в языке глобальный, вся память является статической.


### Модель памяти

1. Размер машинного слова - 32 бит. Адресация абсолютная, поскольку нет ограничения на размер регистра из-за представления struct.

2. Структура памяти однородная, так как port-mapped. Запись на порт осуществляется посредством отдельных команд.

3. Пользователю доступен только аккумуляторный регистр.

### Отображение конструкций языка на модель памяти процессора

* Память отображается модулем, генерирующим код, статически. Инструкции не перемешиваются. Пробелов между инструкциями нет. Каждая инструкция занимает 1 машинное слово.

```text
     Memory (for data and instructions)
+-----------------------------+
|    ...                      |
| 0x00: var1                  | <- var_label: value1
| 0x20: cmd1                  | <- cmd1: args
| 0x40: cmd2                  | w- cmd2: args
| 0x60: var2                  | w- var_label: value2
|    ...                      |
+-----------------------------+
```

## Система команд

Особенности процессора:

* Машинное слово -- 32 бит, знаковое.
* Память данных и команд:
    * Адресуется через регистр data_register, program_counter.
    * Может быть записана из аккумулятора.
    * Может быть прочитана в регистр data_register или instruction_register.


* Ввод-вывод - port-mapped. Осуществляется посредством взаимодействия с accumulator. Номер порта - контанта.
* Program_counter - счетчик команд. Инкрементируется с каждой инструкцией, может быть перезаписан командой перехода.
* Tick_counter - счетчик номера такта с начала обработки инструкции.
* Инструкции обрабатываются последовательно друг за другом. У каждой иструкции есть этапы `FETCH`, `DECODE`, `EXECUTE`.
* Прерываний нет.
* Машина поддерживает только абсолютную адресацию.
* Присутствуют флаги Z (zero), N (neg) для команд перехода.


### Набор инструкции

* Комментарии по командам выше. Одинаковые команды имеют разный опкод в зависимости от принимаемых аргументов. Аргументы бывают `Indirect`, `Direct`.

* Реализуется в модуле [isa](./isa.py).

|Syntax       |Mnemonic |Кол-во тактов|Comment|
|-------------|---------|-------------|-------|
|`inc`        |`inc`    |3            |  -    |
|`dec`        |`dec`    |3            |  -    |
|`itoc`       |`itoc`   |3            |  -    |
|`ctoi`       |`ctoi`   |3            |  -    |
|`add <m32>`  |`add_m`  |4            |  -    |
|`add <imm32>`|`add_imm`|3            |  -    |
|`sub <m32>`  |`sub_m`  |4            |  -    |
|`sub <imm32>`|`sub_imm`|3            |  -    |
|`div <m32>`  |`div_m`  |4            |  -    |
|`div <imm32>`|`div_imm`|3            |  -    |
|`mod <m32>`  |`mod_m`  |4            |  -    |
|`mod <imm32>`|`mod_imm`|3            |  -    |
|`mul <m32>`  |`mul_m`  |4            |  -    |
|`mul <imm32>`|`mul_imm`|3            |  -    |
|`ld <m32>`   |`ld_m`   |4            |  -    |
|`ld <imm32>` |`ld_imm` |3            |  -    |
|`st <m32>`   |`st_imm` |3            |  -    |
|`cmp <m32>`  |`cmp_m`  |4            |  -    |
|`cmp <imm32>`|`cmp_imm`|3            |  -    |
|`je <imm32>` |`je`     |3            |  -    |
|`jne <imm32>`|`jne`    |3            |  -    |
|`jb <imm32>` |`js`     |3            |  -    |
|`jbe <imm32>`|`js`     |3            |  -    |
|`jg <imm32>` |`js`     |3            |  -    |
|`jge <imm32>`|`js`     |3            |  -    |
|`jmp <imm32>`|`jmp`    |3            |  -    |
|`in <imm32>` |`in_imm` |3            |  -    |
|`out <imm32>`|`out_imm`|3            |  -    |
|`halt`       |`halt`   |3            |  -    |

* Примечание: лишний такт для некоторых команд, работающих с прямыми адресами, наблюдается из-за декодирования операндов инструкции.

#### Этапы инструкций

* `FETCH` - 1 такт
* `DECODE` - 1 или более тактов
* `EXECUTE` - 1 или более тактов (условимся что 1 на все операции, даже `div`, `mul`)

### Кодирование инструкций

- Машинный код сериализуется в список JSON.
- Один элемент списка, одна инструкция.
- Адрес инструкции -- индекс списка + `base_address`. Используется для команд перехода и выбора следующей инструкции.

Пример:

* Variable:
```json
{
  "address": 0,
  "value": 2
}
```

* Instruction:
```json
{
  "address": 608,
  "args": [
    64
  ],
  "opcode": "st_imm"
}
```
где:
* `address` - адрес, по которому в памяти расположена данная инструкция;
* `value` - значение переменной;
* `args` - аргументы команды, соответствующие opcode;
* `opcode` - код операции;

Все команды десериализуются в `class Instruction`, определенный в модуле [isa](./isa.py).

## Транслятор

* Интерфейс командной строки: `translator.py <input_file> <target_file>`

* Реализовано в модуле: [translator](./translator.py).

* Этапы трансляции (функция translate):
    * Токенизирование: преобразование исходного кода в последовательность лексем (`LexerNode`).
    * Парсинг: на основании токенов строится дерево AST. Парсер умеет бросать ошибки и намекать, в чем проблема, в случае невалидных исходников (`class Parser`).
    * Проверка дерева, сгенерированного парсером, на соответствие аргументов инструкций и наличия самих инструкций в словаре `ISACommands`, типа инструкций в соответствующих секциях (так в секции `.data` нельзя определить команду), проверка границ чисел.
    * Генерация машинного кода на основании дерева AST парсера.

* Парсер и лексер реализуют обработку исходных кодов в соответствие с `ebnf`;

* Один символ языка -- одна инструкция;

* Инструкции и их аргументы прямо отображаются на опкод.

### Генерируемый формат лексера

```
[
    {
        "ltype": "EOL",
        "off": 18,
        "raw": "\n"
    },
    {
        "ltype": "Identifier",
        "off": 19,
        "raw": "_start"
    },
    {
        "ltype": "Syntax",
        "off": 25,
        "raw": ":"
    },
    {
        "ltype": "EOF",
        "off": 26,
        "raw": null
    }
]

```

### Генерируемый формат парсера

```
{
  "type": "Program",
  "sections": [
    {
      "type": "Section",
      "value": ".text",
      "instructions": [
        {
          "type": "Command",
          "labels": [
            "_start"
          ],
          "cmd": "ld",
          "args": [
            {
              "type": "DirectArgument",
              "value": 0
            }
          ]
        },
        {
          "type": "Command",
          "labels": [
            ".loop"
          ],
          "cmd": "in",
          "args": [
            {
              "type": "DirectArgument",
              "value": 0
            }
          ]
        },
        {
          "type": "Command",
          "labels": [],
          "cmd": "out",
          "args": [
            {
              "type": "DirectArgument",
              "value": 1
            }
          ]
        },
        {
          "type": "Command",
          "labels": [],
          "cmd": "jmp",
          "args": [
            {
              "type": "DirectArgument",
              "value": ".loop"
            }
          ]
        }
      ]
    }
  ]
}
```


### Формат ошибки парсера
```
Exception: Parser
{
  "type": "Error",
  "context": "Program",
  "childs": [
    {
      "type": "Error",
      "context": "Section",
      "childs": [
        {
          "type": "Error",
          "context": "Keyword",
          "message": "Got Identifier",
          "lexem": {
            "ltype": "Identifier",
            "off": 19,
            "raw": "_start"
          }
        }
      ]
    }
  ]
}
```

## Модель процессора

Реализована в [machine](./machine.py).

### Схема DataPath and ControlUnit

#### Изображение

![schema](./schema.jpg)

#### Текстовое
``` text                                                        
                                                                   +---------------------------------+       
      cdev cdev                                                    |                                 |       
        ^   ^                                                     (+1)                      +---+    |       
        |   |                                                      |       +--------+       |   |<---+       
      +-------+                                    +---------------+--+----|   PC   |<------|MUX|            
      | IMUX  |                        dr_out o    |                  |    +--------+       |   |<---o       
   +->+-------+                               |    |                  |                     +---+  dr_out    
   |      ^               +--------+          |    |                  +-----------+           ^              
   |  out |          +--->|   AC   |          |    | addr +-------------+         |           |              
+--|---->[ ]         |    +--------+          +----+-----w|             |         |           +-------+      
|  |      |          |           |            |   data_in |             |       ==(0)                 |      
|  |      +----------------------+------------|----------w|             |         | +--------+        |      
|  |  cdev cdev      |           |            |           |     MEM     | oe      | |        |        |      
|  |    |   |        |           |            |           |             |<---+    v v        |        |      
|  |    v   v     +-----+        |     +--------+         |             | wr |  +-----+      |        |      
|  |  +-------+   | MUX |<-+     |     |   DR   |<----+   |             |<-+ |  | MUX |<-----|----+   |      
|  +->|  MUX  |   +-----+  |     |     +--------+     |   |             |  | |  +-----+      |    |   |      
|  |  +-------+     ^ ^    |     |       |            |   |             |  | |     |         |    |   |      
|  |      |         | |    |     |       |            |   +-------------+  | |     v         |    |   |      
|  |      |         | |    |     v       v            |             |      | |  +--------+   |    |   |      
|  |      +-[ ]-----+ |    |  +-----+-------+       +---+           |      | |  |   TC   |   |    |   |      
|  |       in^        |    |  | ALU / FLAGS |       |   |<----------+      | |  +--------+   |    |   |      
|  |         |        |    |  +-----+-------+       |MUX|           |      | |     |         |    |   |      
|  |         |        |    |     |      |           |   |<--+       |      | |     |         |    |   |      
|  |         |        |    |     |      |           +---+   |       |      | |     +----(+1)-+    |   |      
|  |         |        +----------+      |             ^     |       v      | |     |              |   |      
|  |         |             |            |             |     |  +---------+ | |     |  tc_latch/sel|   |      
|  |         |             |            |             |     |  |   IR    | | |     |       +------+   |      
|  |io_sel   |             |            |             |     |  +---------+ | |     |       |          |      
|  |         | ac_latch/sel|            |             |     |       |      | |     |       |  +-------+       
|  |         |             |            v dr_latch/sel|     |       v      | |     v       |  | pc_latch/sel 
+-------------------------------------------------------------------------------------------------------+
|                                                                                                       |
|                                  Instruction Decoder and Dispatcher                                   |
|                                                                                                       |
+-------------------------------------------------------------------------------------------------------+
```

### DataPath

Реализована в классе DataPath. Отдельно выделен класс Alu, являющийся арифметико-логическое устройство.

MEM (memory) - основная память.
* data_in - входные данные, которые мы хотим записать.
* addr - адрес, с которого хотим прочитать или куда хотим записать.
* oe - чтение.
* wr - запись.

DR (data_register) - регистр данных. Предназначен для косвенных операций. Также в него пишется прямой аргумент, чтобы не вести отдельную линию к аккумулятору.
* dr_latch/sel - выбор линии мультиплексора, захват значения, результат в начале следующего такта.

AC - аккумуляторный регистр.
* ac_latch/sel - выбор линии мультиплексора, захват значения, результат в начале следующего такта.

ALU/FLAGS - объединенная на схеме структура. 
* ALU - арифметико-логическое устройство.
* FLAGS - флаги. N (neg) - после выполнения операции с АЛУ результат отрицательный, Z (zero) - после выполнения 0.

IO - ввод вывод.
* cdev - порты к символьным устройствам ввода-вывода, которые принимают за раз машинное слово. На схеме отделены порты ввода и порты вывода.
* io_sel - выбор линии, куда производится запись.
* in - сигнализирует чтение.
* out - сигнализирует запись.
* Выставление in и out сразу - неопределенное поведение.

Прочее:
* IMUX - Inverse Multiplexor.
* MUX - Multiplexor.

### ControlUnit

Реализован в классе ControlUnit.
* Hardwired.
* Моделируется на уровне тактов.

Instruction Decoder and Dispatcher - декодировщик инструкций и установщик сигналов.
*  При выполнении очередного такта выставляет значения на линии.

IR (instruction_register) - регистр инструкций, в которой хранится текущая выполняемая инструкция.

TC (tick_counter) - счетчик тиков с начала выполнения инструкции.
* tc_latch/sel - выбор линии, захват значения до начала следующего такта.

PC (program_counter) - счетчик инструкций.
* pc_latch/sel - выбор линии, захват значения до начала следующего такта.
* dr_out - выходящее значение с регистра данных для установки в PC.

#### Особенности работы модели

* Для журнала состояния используется стандартный модуль logging.
* Текущее состояние (`RUNNABLE`, `HALTED`) - отражает в каком состоянии сейчас находится машина.
* При выполнении инструкции `halt` машина переходит в состояние `HALTED`.
* При выполнении исключительной ситуации бросается `Exception`.
* Управление симуляцией реализовано в функции `Simulate`.

## Апробация

### Интеграционное тестирование

Реализованы в [`test/integrational_test.py`](./test/integrational_test.py).

Алгоритмы, на которых производится тестирование:

1. cat
2. hello

#### Пример использования и журнал работы процессора на примере `cat`:

``` console
$ cat cat.asm
section .text

_start:
  ld 0

.loop:
  in 0 ; read from port 0
  cmp 0
  je end
  out 1
  jmp .loop

end:
  halt

$ cat cat_in-foo
{
  "0": {
    "in": [
      "foo",
      0
    ]
  },
  "1": {
    "out": []
  }
}


$ ./machine.py cases/cat cases/cat_in-foo cases/cat_out
DEBUG:root:TICK: 0, PC: 0, TC: -1, DP: { AC: -1, DR: -1, FLG: { Z: 0, N: 0 } }, IR: {'address': -1, 'opcode': None, 'args': None, 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 1, PC: 0, TC: -1, DP: { AC: -1, DR: -1, FLG: { Z: 0, N: 0 } }, IR: {'address': 0, 'opcode': ld_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 2, PC: 32, TC: 0, DP: { AC: -1, DR: -1, FLG: { Z: 0, N: 0 } }, IR: {'address': 0, 'opcode': ld_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 3, PC: 32, TC: 1, DP: { AC: -1, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 0, 'opcode': ld_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 4, PC: 32, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 5, PC: 64, TC: 0, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[0].in >> 102 ('f')
DEBUG:root:TICK: 6, PC: 64, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 7, PC: 64, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 8, PC: 96, TC: 0, DP: { AC: 102, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 9, PC: 96, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 10, PC: 96, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 11, PC: 128, TC: 0, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 12, PC: 128, TC: 1, DP: { AC: 102, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 13, PC: 128, TC: 1, DP: { AC: 102, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 14, PC: 160, TC: 0, DP: { AC: 102, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[1].out << 102 ('f')
DEBUG:root:TICK: 15, PC: 160, TC: 1, DP: { AC: 102, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 16, PC: 160, TC: 1, DP: { AC: 102, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 17, PC: 192, TC: 0, DP: { AC: 102, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 18, PC: 192, TC: 1, DP: { AC: 102, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 19, PC: 32, TC: 1, DP: { AC: 102, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 20, PC: 64, TC: 0, DP: { AC: 102, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[0].in >> 111 ('o')
DEBUG:root:TICK: 21, PC: 64, TC: 1, DP: { AC: 102, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 22, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 23, PC: 96, TC: 0, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 24, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 25, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 26, PC: 128, TC: 0, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 27, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 28, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 29, PC: 160, TC: 0, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[1].out << 111 ('o')
DEBUG:root:TICK: 30, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 31, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 32, PC: 192, TC: 0, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 33, PC: 192, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 34, PC: 32, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 35, PC: 64, TC: 0, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[0].in >> 111 ('o')
DEBUG:root:TICK: 36, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 37, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 38, PC: 96, TC: 0, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 39, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 40, PC: 96, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 41, PC: 128, TC: 0, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 42, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 43, PC: 128, TC: 1, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 44, PC: 160, TC: 0, DP: { AC: 111, DR: 192, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[1].out << 111 ('o')
DEBUG:root:TICK: 45, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 128, 'opcode': out_imm, 'args': [1], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 46, PC: 160, TC: 1, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 47, PC: 192, TC: 0, DP: { AC: 111, DR: 1, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 48, PC: 192, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 160, 'opcode': jmp, 'args': [32], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 49, PC: 32, TC: 1, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 50, PC: 64, TC: 0, DP: { AC: 111, DR: 32, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:ports[0].in >> 0 ('')
DEBUG:root:TICK: 51, PC: 64, TC: 1, DP: { AC: 111, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 32, 'opcode': in_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 52, PC: 64, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 53, PC: 96, TC: 0, DP: { AC: 0, DR: 0, FLG: { Z: 0, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 54, PC: 96, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 64, 'opcode': cmp_imm, 'args': [0], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 55, PC: 96, TC: 1, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 56, PC: 128, TC: 0, DP: { AC: 0, DR: 0, FLG: { Z: 1, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 57, PC: 128, TC: 1, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 96, 'opcode': je, 'args': [192], 'value': None}, STAGE: Stage.FETCH, STAT: Status.RUNNABLE
DEBUG:root:TICK: 58, PC: 192, TC: 1, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 192, 'opcode': halt, 'args': [], 'value': None}, STAGE: Stage.DECODE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 59, PC: 224, TC: 0, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 192, 'opcode': halt, 'args': [], 'value': None}, STAGE: Stage.EXECUTE, STAT: Status.RUNNABLE
DEBUG:root:TICK: 60, PC: 224, TC: 1, DP: { AC: 0, DR: 192, FLG: { Z: 1, N: 0 } }, IR: {'address': 192, 'opcode': halt, 'args': [], 'value': None}, STAGE: Stage.FETCH, STAT: Status.HALTED
DEBUG:root:Operations count: 60
```

### Модульное тестирование

Для каждого модуля по файлу с модульными тестами.

1. [`test/translator.py`](./test/translator_test.py)
2. [`test/isa.py`](./test/isa_test.py)
3. [`test/machine_test.py`](./test/machine_test.py)

## CI

* [gitlab-ci](./.gitlab-ci.yml)

* [github-ci](./.github/workflows/ci.yml)

* [test-run](./test-run.sh)

Используемые утилиты:

- `coverage` -- формирование отчёта об уровне покрытия исходного кода.
- `pytest` -- утилита для запуска тестов.
- `pep8` -- утилита для проверки форматирования кода. `E501` (длина строк) отключено, но не следует этим злоупотреблять.
- `pylint`, `mypy` -- утилиты для проверки качества кода. Некоторые правила отключены в отдельных модулях с целью упрощения кода.

Docker:

- Gitlab `python-tools` - выделенный image. Включает в себя все перечисленные утилиты.
- Github `ca-lab3:rlatest`. Конфигурация: [Dockerfile](./Dockerfile) 

## Табличка

| ФИО                     | алг.   | LoC       | code байт | code инстр. | инстр. | такт. | вариант |
|--------------------------|-------|-----------|-----------|-------------|--------|-------|---------|
| Кулаков Никита Васильевич | cat | 11        | 683       | 7           | 20     | 60   | `asm | acc | neum | hw | tick | struct | stream | port | prob2 ` |
| Кулаков Никита Васильевич | hello | 28        | 2621       | 27           | 27     | 81   | `asm | acc | neum | hw | tick | struct | stream | port | prob2 ` |
| Кулаков Никита Васильевич | prob2 | 58        | 5057       | 56           | 644     | 2163   | `asm | acc | neum | hw | tick | struct | stream | port | prob2 ` |

* LoC считаю чистыми, то есть удаляя комментарии и ставя метки в одну строчку с командами.
