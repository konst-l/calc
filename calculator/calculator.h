#pragma once
#include<unordered_map>
#include<string>
#include<memory>
#include<stack>
#include<fstream>
#include<sstream>

class Command {
public:
	virtual ~Command() = default;
	virtual double execute(double a, double b) = 0;
};

class AddCommand : public Command{
public:
	double execute(double a, double b) {
		return a + b;
	}
};
class SubtractCommand : public Command {
public:
	double execute(double a, double b) {
		return a - b;
	}
};
class MultiplyCommand : public Command{
public:
	double execute(double a, double b) {
		return a * b;
	}
};
class DivideCommand : public Command{
public:
	double execute(double a, double b) {
		return a / b;
	}
};

class Calculator {
private:
	std::unordered_map <std::string, std::unique_ptr<Command>> commands;
public:
	Calculator() {
		commands["+"] = std::make_unique<AddCommand>();
		commands["-"] = std::make_unique<SubtractCommand>();
		commands["*"] = std::make_unique<MultiplyCommand>();
		commands["/"] = std::make_unique<DivideCommand>();
	}

	bool isOperator(char x) {
		return x == '/' || x == '*' || x == '+' || x == '-';
	}

	int priority(char x) {
		if (x == '+' || x == '-')
			return 1;
		if (x == '*' || x == '/')
			return 2;
		return 0;
	}
	std::string infixToPostfix(std::string& expression) {

		std::stack<char> operators;
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
		std::stack<double> st;
		std::istringstream iss(expression);
		std::string token;
		while (iss >> token) {
			if (commands.find(token) != commands.end()) {
				double a1 = st.top();
				st.pop();
				double a2 = st.top();
				st.pop();
				double result = commands[token]->execute(a2, a1);
				std::cout << "!" << result << std::endl;
				st.push(result);
			}
			else {
				
				st.push(std::stod(token));
			}
		}
		return st.top();
	}
};