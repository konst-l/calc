
#include <iostream>
#include <string>
#include <iostream>
#include "calculator.h"
int main()
{
    std::string s;
    std::getline(std::cin, s);
    Calculator calc;
    double res = calc.evaluate(s);
    std::cout << res;
}
