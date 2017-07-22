#include "SIUnits.hpp"

#include <iostream>

using namespace SI::Literals;

int
main()
{
    SI::Frequency<int, std::kilo> f1{1};
    SI::Frequency<int> f2{900};

    std::cout << "f1 < f2: " << std::boolalpha << (f1 < f2) << std::endl;
    std::cout << "f1 + f2: " << (f1 + f2).Count() << std::endl;
    std::cout << "f1 - f2: " << (f1 - f2).Count() << std::endl;
    std::cout << "f2 * 0.5: " << (f2 * 0.5).Count() << std::endl;
    std::cout << "0.1 * f1: " << (0.1 * f1).Count() << std::endl;
    std::cout << "f2 / 15: " << (f2 / 15).Count() << std::endl;

    auto f3 = 1000._Hz;
    auto f4 = 1000_Hz;
    auto f5 = 125.1_kHz;
    auto f6 = 100000_Hz;
    auto f7 = 100_kHz;

    std::cout << f3.Count() << std::endl;
    std::cout << f4.Count() << std::endl;
    std::cout << f5.Count() << std::endl;
    std::cout << f6.Count() << std::endl;
    std::cout << f7.Count() << std::endl;

    return 0;
}
