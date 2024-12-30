#include <iostream>
#include <string>
#include "calculator.h"
#include "windows.h"

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Calculator calc;

    std::cout << "Please, choose the mode (programmer/engineering): ";
    std::string mode;
    std::cin >> mode;

    calc.setMode(mode);

    std::cin.ignore();
    std::cout << "Please, enter an expression: ";
    std::string expr;
    std::getline(std::cin, expr);

    try {
        double res = calc.evaluate(expr);
        std::cout << "Result: " << std::fixed << std::setprecision(2) << res << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}