#include "include/Random.hpp"

#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include <memory>
#include <variant>

int main () {

    auto vec = strq::random<
            std::vector<std::variant<double, std::string, int>>>
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

    for (const auto& i : vec) {
        std::visit([](auto a) { std::cout << a << std::endl; }, i);
    }

    const auto str = strq::random<std::string>(
            strq::Container {
                strq::Arithmetic{std::uniform_int_distribution{48, 60}},
                10
            });

    std::cout << str << std::endl;

    auto map = strq::random<std::map<std::string, int>>(
                 strq::Container {
                    strq::Compound {
                        strq::Container {
                            strq::Arithmetic{std::uniform_int_distribution{48, 60}},
                            20
                        },
                        strq::Arithmetic{std::uniform_int_distribution{0, 10}}
                    },
                    10
                }
            );

    std::cout << "\n\n\n";

    for (const auto& [key, value] : map) {
        std::cout << key << " : " << value << std::endl;
    }

    std::cout << "\n\n";

    const auto tuple = strq::random<std::tuple<int, std::shared_ptr<int>>>(
            strq::Compound {
                    strq::Default{},
                    strq::Default{}
            }
            );

    std::cout << "Tuple : " << std::get<0>(tuple) << " " << *std::get<1>(tuple) << "\n\n";

    auto ptr = strq::random<int*>(
                   strq::Compound{
                       strq::Arithmetic{std::uniform_int_distribution{3444, 5000}}
                   }
                   );

    std::cout << "\n\n\n" << *ptr;

    auto smart_ptr = strq::random<std::unique_ptr<double>>(
            strq::Compound{
                strq::Arithmetic{std::normal_distribution{0.0, 100.0}}
            }
            );

    std::cout << "\n\n\n" << *smart_ptr << "\n\n\n";


    auto stack = strq::random<std::stack<int>>(
                     strq::Container {
                         strq::Arithmetic{},
                         10
                     }
            );

    while (!stack.empty()) {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }

    std::cout << "\n\n\n";

    auto queue = strq::random<std::queue<int>>(
                    strq::Container {
                        strq::Arithmetic{},
                        10
                        }
    );

    while (!queue.empty()) {
        std::cout << queue.front() << std::endl;
        queue.pop();
    }

    auto default_schema1 = strq::random<std::vector<int>>();

    std::cout << "\n\n\n";

    for (auto i : default_schema1) {
        std::cout << i << ' ';
    }


    auto default_schema2 = strq::random<std::pair<double, double>>();

    std::cout << "\n\n\n" << default_schema2.first << ' ' << default_schema2.second;


    return 0;
}


