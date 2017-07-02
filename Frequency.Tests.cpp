#include "Frequency.hpp"

#include <iostream>

using namespace Types;
using namespace Types::Literals;

int
main()
{
    Frequency<int, std::kilo> F{-1};
    std::cout << F.Count << std::endl;
    // auto f0 = 1000._Hz;
    // auto f1 = 125.1_kHz;
    // auto f2 = 100000_Hz;
    // auto f3 = 100_kHz;
    // Frequency<int> f4{1_Hz};
    // Frequency<int> f5;

    // std::cout << f0.Count << std::endl;
    // std::cout << f1.Count << std::endl;
    // std::cout << f2.Count << std::endl;
    // std::cout << f3.Count << std::endl;
    // std::cout << f4.Count << std::endl;

    // std::cout << f5.Count << std::endl;
    // f5 = 100.5_Hz;
    // std::cout << f5.Count << std::endl;

    return 0;
}
