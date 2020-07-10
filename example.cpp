#include "include/Random.hpp"

#include <iostream>
#include <map>

int main () {

//    auto vec = stq::random<
//            std::vector<std::variant<double, std::string, int>>>
//            (
//                    stq::Container{
//                        stq::Compound{
//                            stq::Arithmetic{std::normal_distribution{}},
//                            stq::Container{
//                                stq::Arithmetic{},
//                                5
//                                },
//                            stq::Arithmetic{std::uniform_int_distribution{34, 42}}
//                        },
//                        100
//                    }
//            );
//
//    for (const auto& i : vec) {
//        std::visit([](auto a) { std::cout << a << std::endl; }, i);
//    }

    const auto str = stq::random<std::string>(
                stq::Container {
                    stq::Arithmetic{std::uniform_int_distribution{48, 60}},
                    10
                }
            );
    std::cout << str << std::endl;

    auto map = stq::random<std::map<std::string, int>>(
                stq::Container {
                    stq::Compound {
                        stq::Container {
                            stq::Arithmetic{std::uniform_int_distribution{48, 60}},
                            20
                        },
                        stq::Arithmetic{std::uniform_int_distribution{0, 10}}
                    },
                    10
                }
            );

    std::cout << "\n\n\n";


    for (const auto& [key, value] : map) {
        std::cout << key << " : " << value << std::endl;
    }

    std::cout << "\n\n";

    auto ptr = stq::random<int*>(
                stq::Arithmetic{std::uniform_int_distribution{3444, 5000}}
            );

    std::cout << "\n\n\n" << *ptr;

    auto smart_ptr = stq::random<std::unique_ptr<double>>(
                stq::Arithmetic{std::normal_distribution{0.0, 100.0}}
            );

    std::cout << "\n\n\n" << *smart_ptr;


    return 0;
}


