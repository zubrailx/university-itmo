## Репозиторий для задания №2 (Buffer)

### Makefile

На этом этапе необходимо явным образом задавать компилятор/линтер. Это можно сделать
таким образом:

```shell
-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

> Заменить `clang`, `clang++` на требуемые.


#### Сборка
```shell
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=<Profile>
cmake --build build
```

>Вместо `<Profile>` необходимо указать одно из перечисленного: `Debug`, `Release`, `Sanitized`.


#### Тестирование
```shell
./build/buffer_test
```
