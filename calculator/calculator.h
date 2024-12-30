#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <sstream>
#include <cmath>
#include <iostream>
#include <iomanip>

class Command {
public:
    virtual ~Command() = default;
    virtual double execute(double a, double b) = 0;
};

class AddCommand : public Command {
public:
    double execute(double a, double b) override {
        return a + b;
    }
};

class SubtractCommand : public Command {
public:
    double execute(double a, double b) override {
        return a - b;
    }
};

class MultiplyCommand : public Command {
public:
    double execute(double a, double b) override {
        return a * b;
    }
};

class DivideCommand : public Command {
public:
    double execute(double a, double b) override {
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
};

class SinCommand : public Command {
public:
    double execute(double a, double) override {
        return std::sin(a);
    }
};

class CosCommand : public Command {
public:
    double execute(double a, double) override {
        return std::cos(a);
    }
};

class TanCommand : public Command {
public:
    double execute(double a, double) override {
        return std::tan(a);
    }
};

class BitwiseAndCommand : public Command {
public:
    double execute(double a, double b) override {
        return static_cast<int>(a) & static_cast<int>(b);
    }
};

class BitwiseOrCommand : public Command {
public:
    double execute(double a, double b) override {
        return static_cast<int>(a) | static_cast<int>(b);
    }
};

class BitwiseXorCommand : public Command {
public:
    double execute(double a, double b) override {
        return static_cast<int>(a) ^ static_cast<int>(b);
    }
};

class BitwiseNotCommand : public Command {
public:
    double execute(double a, double) override {
        return ~static_cast<int>(a);
    }
};

template <typename T>
class Stack {
private:
    T* arr_;
    int top_;
    int capacity_;

    void resize() {
        capacity_ *= 2;
        T* newArr = new T[capacity_];
        for (int i = 0; i <= top_; i++) {
            newArr[i] = arr_[i];
        }
        delete[] arr_;
        arr_ = newArr;
    }

public:
    Stack(int size = 2) : top_(-1), capacity_(size) {
        arr_ = new T[size];
    }

    ~Stack() {
        delete[] arr_;
    }

    void push(T x) {
        if (top_ == capacity_ - 1) {
            resize();
        }
        arr_[++top_] = x;
    }

    void pop() {
        if (top_ == -1) {
            throw std::underflow_error("Stack is empty");
        }
        top_--;
    }

    T top() {
        if (top_ == -1) {
            throw std::underflow_error("Stack is empty");
        }
        return arr_[top_];
    }

    bool empty() {
        return top_ == -1;
    }

    int size() {
        return top_ + 1;
    }
};

class Calculator {
private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;

public:
    Calculator() {
        setMode("engineering");
    }

    void setMode(const std::string& mode) {
        commands.clear();
        if (mode == "programmer") {
            commands["&"] = std::make_unique<BitwiseAndCommand>();
            commands["|"] = std::make_unique<BitwiseOrCommand>();
            commands["^"] = std::make_unique<BitwiseXorCommand>();
            commands["~"] = std::make_unique<BitwiseNotCommand>();
        }

        commands["+"] = std::make_unique<AddCommand>();
        commands["-"] = std::make_unique<SubtractCommand>();
        commands["*"] = std::make_unique<MultiplyCommand>();
        commands["/"] = std::make_unique<DivideCommand>();
        commands["sin"] = std::make_unique<SinCommand>();
        commands["cos"] = std::make_unique<CosCommand>();
        commands["tan"] = std::make_unique<TanCommand>();
    }

    bool isOperator(char x) {
        return x == '+' || x == '-' || x == '*' || x == '/' ||
            x == '&' || x == '|' || x == '^' || x == '~';
    }

    int priority(char x) {
        if (x == '+' || x == '-') return 1;
        if (x == '*' || x == '/') return 2;
        if (x == '&' || x == '^' || x == '|') return 0;
        return -1;
    }

    std::string infixToPostfix(std::string& expression) {
        Stack<char> operators;
        std::ostringstream output;

        for (char c : expression) {
            if (std::isspace(c)) {
                continue;
            }

            if (std::isdigit(c)) {
                output << c;
            }
            else if (isOperator(c)) {
                output << ' ';
                while (!operators.empty() && priority(operators.top()) >= priority(c)) {
                    output << operators.top();
                    output << ' ';
                    operators.pop();
                }
                operators.push(c);
            }
            else if (c == '(') {
                operators.push(c);
            }
            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    output << ' ' << operators.top();
                    operators.pop();
                }
                operators.pop();
            }
        }

        while (!operators.empty()) {
            output << ' ' << operators.top();
            operators.pop();
        }

        return output.str();
    }

    double evaluate(std::string& expression) {
        expression = infixToPostfix(expression);
        Stack<double> st;
        std::istringstream iss(expression);
        std::string token;

        while (iss >> token) {
            if (commands.find(token) != commands.end()) {
                if (token == "sin" || token == "cos" || token == "tan") {
                    double a1 = st.top(); st.pop();
                    double result = commands[token]->execute(a1, 0);
                    st.push(result);
                }
                else {
                    double a1 = st.top(); st.pop();
                    double a2 = st.top(); st.pop();
                    double result = commands[token]->execute(a2, a1);
                    st.push(result);
                }
            }
            else {
                st.push(std::stod(token));
            }
        }
        return st.top();
    }
};