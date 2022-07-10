## Шаблоны + концепты и ограничения + замыкания

```c++
template <typename Container, typename Value = Container::value_type>
bool contains(const Container & container, const Value & value);

template <typename Value>
class SparseVector;

template <typename Value>
using Dictionary = std::unordered_map<std::string, Value>;

template <typename T>
T my_var;
```

### Мономорфизация

При использовании шаблона компилятор генерирует под конкретный тип истанциацию:
```c++
template <typename Value>
class SparseVector {
public:
    Value & operator[](size_t index);
};

SparseVector<std::string> names;
names[1337] = “Ivan”;
```

Код выше превращается, например, в:
```c++
class SparseVector_std_string {
public:
    std::string & operator[](size_t index);
};

SparseVector_std_string names;
names[1337] = “Ivan”;
```

> Шаблоны отмечаются как <b>weak-символы</b>.

### Значения как параметры шаблонов
```c++
template<uint32_t Id>
class Message;

template<typename T, typename DeleterFn, DeleterFn Deleter>
class CPtr;

using Archive = CPtr<archive *,
                     decltype(archive_close), // тип функции
                     &archive_close>;         // сама функция (указатель)
```

### Типы

* Должны быть известны на этапе компиляции
* В определении шаблона являются константами
* Ограничены структурными типами:
  * lvalue-ссылка
  * интегральный тип
  * указатель, в т. ч. указатель-на-член-класса
  * перечисление
  * … (список расширяется в новых стандартах)


### Вложенные шаблоны
```c++
template <typename Value>
class SparseVector
{
public:
    template <typename Container>
    SparseVector(Container from)
    { ... }
};
```

#### При определении снаружи
```c++
template <typename Value>
class SparseVector
{
public:
    template <typename Container>
    SparseVector(Container from);
};
template <typename Value>
template <typename Container>
SparseVector<Value>::SparseVector() { ... }
```

### Шаблоны с шаблонными параметрами

```c++
template <typename Container, typename Value = Container::value_type>
bool contains(const Container & container, const Value & value);
```

Допустим мы хотим, чтобы в контейнере, который мы передаем, лежали элементы типа, который
мы хотим проверить на наличие в данном контейнере.

```c++
template <template <typename> typename Container, typename Value>
bool contains(const Container<Value> & container,
        const Value & value);
```

В такой записи `Value` привязан к контейнеру.

### Полная специализация шаблонов

```c++
template <typename T>
void print(T value)
{ std::cout << value; }

template <>
void print<std::chrono::seconds>(std::chrono::seconds sec) 
{ std::cout << sec << “s”; } // определяем шаблон отдельно для seconds, для остальных выше исп.
```

Для `std::chrono::seconds` используется истанциация ниже, 
для остальных будет использоваться то, что выше.

```c++
template <typename T>
using Reply = void;

template <>
using Reply<BestPriceRequest> = BestPriceReply;

template <typename Request>
void handle_request(Request req)
{
    Reply<Request> reply; ...
```

Для каждого `Reply` сопоставляется `Request`, по умолчанию `Reply = void`. Таким образом, мы 
можем жанглироать символами.

### Явное истанциирование шаблонов

```c++
// .cpp
template <typename T>
void print(T value)
{ std::cout << value; }

template void print<int>(int value);
template void print<double>(double value);
template void print<std::string>(std::string value);

// .h
template <typename T>
void print(T value);

template void print<int>(int value);
template void print<double>(double value);
template void print<std::string>(std::string value);
```

Компилятор поймет, что уже где-то есть инстанциация `<int>, <double>, <std::string>`, и он для 
них не будет создавать шаблон.

### Частичная специализация шаблонов

```c++
// only classes and variables
template <typename T, typename Alloc>
struct Container;

template <typename Alloc>
struct Container<int, Alloc>;

class MyAlloc;

template <typename T>
struct Container<T, MyAlloc>;
```

### Автоматический вывод параметров шаблонов

Шаблоны функций
  * https://en.cppreference.com/w/cpp/language/template_argument_deduction

Шаблоны классов
  * https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

```c++
template <typename T>
T add(T a, T b);

double a = 0.6;
double b = 304.0;
double c = add<double>(a, b);
```

```c++
template <typename T>
T add(T a, T b);

double a = 0.6;
double b = 304.0;
double c = add(a, b);
```
Параметризуются типы передаваемых значений.

```c++
std::vector<int> years = { 2019, 2020, 2021 }; // up to C++17
std::vector years = { 2019, 2020, 2021 }; // C++17

std::vector month_names = { “January”, “February”, ... };
// most probably expected std::vector<std::string>, got
// std::vector<const char *>
```

### auto

auto - Placeholder type specifier

```c++
auto id = UUID::create(); // auto -> UUID

auto name = “Alex”; // auto -> const char *
const auto * name = “Alex”; // auto -> char

int * id = new auto(1337); // auto -> int
```
Лучше для `auto` явно указывать `const`.

```c++
template<typename A, typename B>
auto add(A a, B b) -> decltype(a + b) { return a + b; } // C++11

template<typename A, typename B>
auto add(A a, B b) { return a + b; } // C++14

auto add(auto a, auto b) { return a + b; } // C++20
```

```c++
// C++14
template<typename T, typename DeleterFn, DeleterFn Deleter>
class CPtr;

using Archive = CPtr<archive *,
                     decltype(archive_close),
                     &archive_close>;

// C++17
template<typename T, auto Deleter>
class CPtr;

using Archive = CPtr<archive *,
                     &archive_close>;
```

#### decltype(auto)

* Два набора правил для вывода типов:
  * Вывод типов для шаблонов и auto
  * Вывод типов для decltype(expr)
* auto использует правила вывода типов для шаблонов + работает с initializer list
* decltype(auto) использует правила вывода типов для decltype(expr)

http://scottmeyers.blogspot.com/2013/07/when-decltype-meets-auto.html

### Универсальные ссылки и perfect-forwarding

http://thbecker.net/articles/rvalue_references/section_01.html

```c++
template <class T>
void foo(T && t) {
    bar(std::forward<T>(t));
}
```

```c++
#include <list>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

class ArgSample
{
public:
    ArgSample()
    {
        std::cout << "constructed" << std::endl;
    }

    ArgSample(ArgSample &&)
    {
        std::cout << "moved" << std::endl;
    }

    ArgSample(const ArgSample &)
    {
        std::cout << "copied" << std::endl;
    }
};

class Sample
{
public:
    Sample(ArgSample, ArgSample) {};
};

// r-value ссылка l-value внутри функции. Она r-value с точки зрения параметра функции
// Вам извне обязаны передать r-value, она внутри как локальная переменная
// Мы можем ее использовать не один раз.
void emplace(ArgSample && arg) {
    Sample s(std::move(arg));
}

// Для двух аргументов
void emplace(const ArgSample & arg, const ArgSample & arg2) { ... }
void emplace(ArgSample &&arg, ArgSample && arg2) { ... }
void emplace(const ArgSample & arg, ArgSample && arg2) { ... }
...


int main() {
    ArgSample a;
    const ArgSample & a_ref = a;
    // различные способы вызова методов
    emplace(a, a);
    emplace(a_ref, a_ref);
    emplace(ArgSample{}, ArgSample{});
    emplace(a_ref, ArgSample{}); 
    return 0;
}
```
Для каждого метода было бы $2^n$ перегрузок, если метод принимает $n$ параметров. Если оставить 
только `r-value` версию, то ее нельзя вызывать с `const &`.

```c++
template <typename ArgSample, typename ArgSample2>
void emplace(ArgSample && arg, ArgSample2 && arg2)
{
    // move использовать не можем, так как не знаем r-value или l-value
    Sample s(arg, arg2);
}
```
В шаблонах `&&` означают не r-value reference, а некую универсальную ссылку (universal reference), 
которая ведет себя как мы этого хотим. 

#### Как резолвятся универсальные ссылки
```c++
template <typename T>
T && arg;
 
T = int;
T && = int &&;

T = int &;
T && = int &; // reference collapsing (int & && -> int &) r-value ссылка на l-value ссылку

T = const int &;
T && = const int &;

T = int &&;
T && = int &&:
```
Аналогия с логическим `or` r-value = 0, l-value = 1. r-value всегда коллапсится в l-value.

```c++
template <typename ArgSample, typename ArgSample2>
void emplace(ArgSample && arg, ArgSample2 && arg2)
{
    // r-value reference
    Sample s(arg, arg2);
}
```

```c++
template <typename ArgSample, typename ArgSample2>
void emplace(ArgSample && arg, ArgSample2 && arg2)
{
    // просим у компилятора тип, который изначально был.
    Sample s(static_cast<decltype(arg)>(arg), static_cast<decltype(arg2)>(arg2));
}

// способ номер 2
template <typename ArgSample, typename ArgSample2>
void emplace(ArgSample && arg, ArgSample2 && arg2)
{
    // perfect-forwarding (std::forward - helper). Прокидываем то, что передали раннее
    Sample s(std::forward<ArgSample>(arg), std::forward<ArgSample2>(arg2));
}

// или
void emplace(auto && arg, auto && arg2)
{
    Sample s(std::forward<decltype(arg)>(arg), std::forward<decltype(arg2)>(arg2));
}
```

### Шаблоны с переменным числом параметров

```c++
// variadic-template-arguments
print(“Hello, “, name, “! We’ve last seen you “, date);

$ ./hello Alex
Hello, Alex! We’ve last seen you 03.05.2021 ```

```c++
template <typename T>
void print(T value)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl; // выводит текущий вызываемый шаблон
  std::cout << value;
}

template <typename T, typename... Args>
void print(T value, Args... args)
{ 
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::cout << value; print(args...); 
}

$ ./hello Alex
void print(T, Args ...) [with T = const char*; Args = {int, const char*, double}]
abcdevoid print(T, Args ...) [with T = int; Args = {const char*, double}]
101void print(T, Args ...) [with T = const char*; Args = {double}]
cdefvoid print(T) [with T = double]
102.1
```

#### Pack-expansion

```c++
// pattern -> pack expansion
f(args...) -> f(arg1, arg2, arg3...)

// Args — template parameter pack, args — function parameter pack
void f(Args... args); -> void f(Arg1 arg1, Arg2 arg2, ...);

struct C : Args... {}; -> struct C : Arg1, Arg2, ... {};

std::vector<int *> vec(&args...) -> vec(&arg1, &arg2, ...);

auto vec = { ++args... } -> auto vec = { ++arg1, ++arg2, ... };

Templ<Args...> my_templ -> Templ<Arg1, Arg2, ...> my_templ;
```

### std::tuple

```c++
#include <iostream>
#include <tuple>
#include <utility>
#include <type_traits>

// MY_TUPLE
// std::tuple<int, std::string, std::vector<...>>;
// std::tuple<int>;
// std::tuple<int, int>;

// основной шаблон
template <typename ... Args>
struct my_tuple {};

// частичная истанциация
template <typename T, typename ... Args>
struct my_tuple<T, Args...> : my_tuple<Args...> {
    T arg;
};

// TYPEHELPER - получить тип по индексу
template <int I, typename ... List>
struct type_helper {};

template <typename T, typename ... List>
struct type_helper<0, T, List...>
{
    using type = T;
};

template <int I, typename T, typename ... List>
struct type_helper<I, T, List...>
{
    using type = typename type_helper<I - 1, List...>::type;
};

// MY_GET
template <int I, typename ... List>
std::enable_if_t<I == 0, typename type_helper<I, List...>::type &> my_get(
        my_tuple<List...> & tuple)
{
    return tuple.arg;
}

template <int I, typename T, typename ... List>
std::enable_if_t<I != 0, typename type_helper<I, T, List...>::type &> my_get(
    my_tuple<T, List...> & tuple)
{
    // отрезание
    my_tuple<List...> & base = tuple;
    return my_get<I - 1>(base);
}


int main()
{
    my_tuple<int, int, std::string> t;
    t.arg = 0 // (первый тип)
    // можно идентифицировать
    t.my_tuple<std::string>::arg = "str"; // пишем в строчку

    // static_assert(std::is_same_v<type_helper<0, int, int, std::string>::type, int>);
    my_get<0>(t) = 1;
    my_get<1>(t) = 2;
    my_get<2>(t) = "abc";
    std::cout << my_get<0>(t) << std::endl;
    std::cout << my_get<1>(t) << std::endl;
    std::cout << my_get<2>(t) << std::endl;
    return 0;
}

/*
struct my_tuple<>;
struct my_tuple<std::string> : my_tuple<>;
struct my_tuple<int, std::string> : my_tuple<std::string>;
struct my_tuple<int, int, std::string> : my_tuple<int, std::string>;
*/
```

#### std::apply 
```c++
// try to implement std::apply for my_tuple
// try to implement std::tie
```

### Раскрытие набора параметров (продолжение pack-expansion)

```c++
// pattern -> pack expansion
f(args...) -> f(arg1, arg2, arg3...)

f(args)...; -> f(arg1), f(arg2), f(arg3), ...; // не работает!
```

Светка набора параметров:
```c++
// fold expression -> fold pack expansion
(args op ...) -> (arg1 op arg2 op arg3 op ...)

int sum(int ... args)
{ return (args + ...); }

(f(args), ...) -> (f(arg1), f(arg2), f(arg3), ...) // раскрытие для оператора запятой

// как бы выглядел print
template <typename ... Args>
void print(Args ... args)
{
    ((std::cout << args), ...);
}
```

### Forward с вариадиками

```c++
void print(auto a)
{}

template <auto Fn, typename ... Args>
void invoke_fn(Args && ... args)
{
  // не работает
  (print<Args>(args)...);
  // работает
  (print<Args>(args),...);
  print(print<Args>(args)...);
  Fn(std::forward<Args>(args)...); // -> Fn(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2))
}
```

### Разрешение имен функций

* Составление набора кандидатов
  * Простой поиск
  * Поиск, зависящий от аргументов
  * Подстановка аргументов шаблонов
* Вычеркивание нежизнеспособных кандидатов
* Выбор лучшего кандидата из жизнеспособных

### SFINAE

* Substitution Failure Is Not An Error*
* *-terms and conditions apply, acts only in overload resolution

Ошибка подстановки шаблонного параметра в объявлении функции при создании набора кандидатов для разрешения имени функции не является ошибкой компиляции, если в наборе кандидатов есть другая подходящая функция.

```c++
template<>
bool contains(const Combination & container,
              const Combination::value_type & val);

bool contains(const Combination & combo,
              const Instrument & leg);


Combination butterfly;
Instrument AAPL_call_option;
if (contains(butterfly, AAPL_call_opt)) {
    ...
}
```

```c++
template <typename T,
          std::enable_if_t<std::is_integral_v<T>,
                           bool> = true>
T add(T arg1, T arg2); //если bool - то работает эта функция

template <typename T,
          std::enable_if_t<!std::is_integral_v<T>,
                           bool> = true>
T add(T arg1, T arg2); // в противном случае работает эта
```
https://en.cppreference.com/w/cpp/types/enable_if


### SFINAE не нужно:
```c++
template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T>
concept NotIntegral = !Integral<T>;

auto add(Integral auto arg1, Integral auto arg2)
{
   return arg1 + arg2;
}

template <NotIntegral T>
T add(T arg1, T arg2)
{
   return arg1.append(arg2);
}
```
```c++
template <typename T>
concept Substractable = requires (T a, T b) { T(a - b); };

template <typename T>
T sub(T arg1, T arg2) requires Substractable<T>
{
   return arg1 - arg2;
}

template <typename T>
T add(T arg1, T arg2) requires requires { T(arg1 + arg2); }
{
   return arg1 + arg2;
}

std::cout << sub(std::string("abcd"), std::string("abc")) << std::endl; // ошибка при компиляции
std::cout << add(std::string("abcd"), std::string("abc")) << std::endl; // все хорошо
```

### Концепты и constraints/ограничения

* Constraint — набор атомарных ограничений на шаблонный параметр, связанный логическими операциями && и ||.
* Концепт — именованный constraint.

```c++
template <template_parameter_list>
concept concept_name = constraint_expression;

template <typename T>
concept Integral = std::is_integral_v<T>;
```

### Атомарные ограничения

Константное выражение типа bool
```c++
std::is_integral_v<T>;
(I > 0) // I is a non-type template parameter
```
requires-выражение
```c++
requires ( parameter-list(optional) ) { requirement-seq }
```

### requires-выражения
Простое требование, проверяется только корректность с точки зрения языка.
```c++
requires (T a, T b)
{
    a + b;
    a - b;
}
```

Требование к типу, проверяется существование типа.
```c++
requires (T a, U b)
{
    typename CommonType<T, U>;
}
```

Составное требование, проверяется языковая корректность и свойства выражения.
```c++
requires (T a, U b)
{
    {a + b} -> std::same_as<int>;
    {a * b} noexcept -> std::same_as<int>;
}
```

Вложенное требование.
```c++
requires (T a, U b)
{
    requires Addable<T, U>;
}
```

Комбинация всего перечисленного.
```c++
requires (T a, U b)
{
    a + b;
    {a + b} -> std::same_as<int>;
    typename CommonType<T, U>;
    requires Addable<T, U>;
}
```

### Где можно использовать концепты
```c++
// template type parameters
  template <Addable T>
  int add(T a, T b);
// template non-type parameters
  template <Addable auto Value>
// с auto
  int add(Addable auto a, Addable auto b);
// в requires-clause
```

### requires-clause
После списка параметров шаблонов.
```c++
template <typename T>
    requires Addable<T>
int add(T a, T b);
```
После объявления функции.
```c++
template <typename T>
int add(T a, T b) requires Addable<T>;
```
В requires-clause можно использовать не только концепты, но и constraints.

```c++
template <typename T>
concept Substractable = requires (T a, T b) { T(a - b); };

template <typename T>
T sub(T arg1, T arg2) requires Substractable<T>
{
   return arg1 - arg2;
}

template <typename T>
T add(T arg1, T arg2) requires requires { T(arg1 + arg2); }
{
   return arg1 + arg2;
}

template <int I>
void get() requires (I > 0)
{
    std::cout << "I is not zero" << std::endl;
}
```

### Замыкания
```c++
auto lambda = [capture] (args) specifiers -> return type {
    ... // code
};


auto age_less = [] (const auto & lhs, const auto & rhs) {
    return lhs.age < rhs.age;
};
std::sort(humans.begin(), humans.end(), age_less);


std::vector<std::vector<double>> huge_matrix = generate();
auto heavy_calculations = [&huge_matrix] {
    // do things
};

auto result_future = std::async(heavy_calculations);
// do other things
auto result = result_future.get();
```
