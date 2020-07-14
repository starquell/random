# random
Header-only library for generating containers with random values and other STL-like random structures

For usage just include `include/Random.hpp`. Requires `C++17`.

With this library you can various objects with random data.
Let's start from easy level:
``` c++
   const auto integer = strq::random<int>();
   const auto pair = strq::random<std::pair<double, int>>();
   const auto tuple = strq::random<std::tuple<float, int, std::complex>>();
```
Some random int will be assigned to `integer`, pair of random double and random int will be in `pair`, tuple of random float, float int and random complex number will be in `tuple`.

Also it can work with any STL-like container (that have `begin()`, `end()`, nested `value_type`):
```c++
   const auto vector = strq::random<std::vector<int>>();
   const auto map = strq::random<std::map<int, double>>();
   const auto set = strq::random<std::unordered_set<std::size_t, 
                                                    std::pair<int, int>>>();
   const auto flat_map = strq::random<boost::container::flat_map<int, int>>();
```

In all these examples containers will contain some random objects of their `value_type`.
You can manually set size of containers and distributions of arithmetic types using `Schema`.

### Schemas

Schema is a kind of hint used for generating more complicated random objects. Some examples: 
```c++
const auto vector = strq::random<std::vector<int>> (
        strq::Container {
            strq::Arithmetic {std::poisson_distribution{2.0}},
            20
        } 
);
```
In this examples `vector` will contain 20 ints distributed using Poisson distribution with parameter 2.


Schema for any arithmetic type (`std::is_arithmetic_v<T> == true` or type that satisfies concept `std::arithmeric` (> C++20)) is `strq::Arithmetic`.

if `strq::Ariphmetic` сonstructed without arguments, accordant value will be distributed using `std::uniform_int_distribution{}` for integral types and `std::uniform_real_distribution{}` for types that represent numbers with floating point.

Schema for containers is `strq::Container`. It takes 2 arguments - Schema for contained value and size of container.

Some other example:
```c++
const auto pair = strq::random<std::pair<double, double>> (
        strq::Compound {
            strq::Arithmetic{std::normal_distribution{}},
            strq::Arithmetic{}
        }        
)
```
Pair of normal distributed double and uniform distributed double will be in `pair`.

`strq::Compound` used as schema for types that contain some nested types.
It takes arbitrary number of schemas (for every nested type).
More complicated example:
```c++
 auto vec = strq::random<
            std::vector<std::tuple<double, std::string, int>>>
            (
                strq::Container{
                    strq::Compound{
                        strq::Arithmetic{std::normal_distribution{}},
                        strq::Container{
                            strq::Arithmetic{},
                            5
                        },
                        strq::Arithmetic{std::uniform_int_distribution{34, 42}}
                    },
                        100
                }
            );
```
Schema tells to `strq::random` that outer `std::vector` must contain 100 instances of `std::tuple` of normal distributed `double`, `std::string` of 5 characters and uniform distributed `int` from 34 to 42.

Supported compound types are `std::tuple`, `std::pair`, `std::variant`, `std::optional`, `std::complex`, `std::atomic`, pointers and smart pointers.

Now imagine that we don't want specific distrubutions for objects in tuple in last example. 
In this cast, schema for inner tuple will be like `strq::Compound {strq::Arithmetic{}, strq::Container {strq::Arithmeric, ....bla  bla bla` or just `strq::Default`.

`strq::Default` schema for type T tells that we want random T like result of `strq::random<T>()`.

Other usefull examples can be found in `examples.cpp`.
