#include "include/Random.hpp"

#include <iostream>

int main () {

    auto vec = stq::random()
                .size(10)
                .generate<std::vector<std::string>>();

    for (const auto& i : vec)
        std::cout << i << ' ';

    return 0;
}


