/*
 * Calculator.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Calculator.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Calculator::Calculator() {
	operators = new vector<char>();
	numbers = new vector<Number*>();
}

Calculator::~Calculator() {
	// TODO Auto-generated destructor stub
}

string Calculator::SimplifyExpression(string equation) {
	equation.erase(std::remove(equation.begin(), equation.end(), ' '), equation.end());
	int indexOfLastOp = -1;

	for (int i = 0; i < equation.length(); i++) {
		string equationSegment = equation;

		if (i + 1 == equation.length()) {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equationSegment.length()));
			numbers->push_back(lhs);
		} else if (equation[i] == '(') {
			bool foundClose = false;
			bool isEnd = false;
			char nextOp;
			for (int x = i; x < equation.length(); x++) {
				//if (equation[x] == ')') {
					//i = x;
				//}

				if ((equation[x] == '*' || equation[x] == '/' || equation[x] == '+' || equation[x] == '-' || x + 1 == equation.length()) && foundClose) {
					if (x + 1 == equation.length()) {
						isEnd = true;
					} else {
						nextOp = equation[x];
					}
				}
			}

			if (isEnd) {
				Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equation.length()));
				numbers->push_back(lhs);

				indexOfLastOp = i;
			} else {
				Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equation.length()));
				numbers->push_back(lhs);
				operators->push_back(nextOp);

				indexOfLastOp = i;
			}
		} else if (equation[i] == '*') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(lhs);
			operators->push_back('*');

			indexOfLastOp = i;
		} else if (equation[i] == '/') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(lhs);
			operators->push_back('/');

			indexOfLastOp = i;
		} else if (equation[i] == '+') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(lhs);
			operators->push_back('+');

			indexOfLastOp = i;
		} else if (equation[i] == '-') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(lhs);
			operators->push_back('-');

			indexOfLastOp = i;
		}
	}

	Number * result = &(PerformCalculations());
	string stringResult = "";
	try {
		stringResult = result->toString();
	} catch (exception e) {
		cout << e.what();
	}
	delete result;
	return stringResult;
}

Number& Calculator::PerformCalculations() {
	for (int i = 0; i < operators->size(); i++) {
		if (operators->at(i) == '*') {
			Number * result = &(*numbers->at(i) * *numbers->at(i + 1));
			numbers->at(i) = result;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		} else if (operators->at(i) == '/') {
			Number * result = &(*numbers->at(i) / *numbers->at(i + 1));
			numbers->at(i) = result;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < operators->size(); i++) {
		if (operators->at(i) == '+') {
			Number * result = &(*numbers->at(i) + *numbers->at(i + 1));
			numbers->at(i) = result;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		} else if (operators->at(i) == '-') {
			Number * result = &(*numbers->at(i) - *numbers->at(i + 1));
			numbers->at(i) = result;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		}
	}

	if (!operators->empty()) {
		Number * placeholder = new Placeholder(*this->numbers, *this->operators);
		return *placeholder;
	} else {
		numbers->at(0) = &(numbers->at(0)->simplify());
		return *numbers->at(0);
	}
}

Number& Calculator::Calculate(string equationSegment) {
	int numType = CheckNumberType(equationSegment);

	if (equationSegment[0] == '(' && equationSegment[equationSegment.length() - 1] == ')') {
		std::vector<Number*> * localNumbers = new vector<Number*>();
		std::vector<char> * localOperators = new vector<char>();

		int indexOfLastOp = 0;
		for (int i = 1; i < equationSegment.length(); i++) {
			string segment = equationSegment;

			if (i + 1 == equationSegment.length()) {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, segment.length() - indexOfLastOp - 2));
				localNumbers->push_back(lhs);
			} else if (equationSegment[i] == '*') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(lhs);
				localOperators->push_back('*');

				indexOfLastOp = i;
			} else if (equationSegment[i] == '/') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(lhs);
				localOperators->push_back('/');

				indexOfLastOp = i;
			} else if (equationSegment[i] == '+') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(lhs);
				localOperators->push_back('+');

				indexOfLastOp = i;
			} else if (equationSegment[i] == '-') {
				if (indexOfLastOp + 1 == i) {
					Placeholder * lhs = new Placeholder();
					lhs->getNumbers().push_back(new Integer(-1));
					lhs->getNumbers().push_back(new Integer(atoi(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1).c_str())));
				} else {
					Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
					localNumbers->push_back(lhs);
					localOperators->push_back('-');
				}

				indexOfLastOp = i;
			}
		}

		Number * placeholder = new Placeholder(*localNumbers, *localOperators);
		return *placeholder;
	} else {
		if (numType == 0) {
			Number * integer = new Integer(atoi(equationSegment.c_str()));
			return *integer;
		} else if (numType == 1) {
			int indexOfCarrot = 0;
			for (int i = 0; i < equationSegment.length(); i++) {
				if (equationSegment[i] == '^') {
					indexOfCarrot = i;
				}
			}
			std::string beforeCarrot = equationSegment;
			std::string afterCarrot = equationSegment;
			beforeCarrot = beforeCarrot.substr(0, indexOfCarrot);
			afterCarrot = afterCarrot.substr(indexOfCarrot + 1, equationSegment.length() - indexOfCarrot - 1);

			Number * exponent = new Exponent(Calculate(beforeCarrot), Calculate(afterCarrot));
			return *exponent;
		} else if (numType == 2) {
			int indexOfColon = 0;
			for (int i = 0; i < equationSegment.length(); i++) {
				if (equationSegment[i] == ':') {
					indexOfColon = i;
				}
			}
			std::string base = equationSegment;
			std::string argument = equationSegment;
			base = base.substr(4, indexOfColon - 4);
			argument = argument.substr(indexOfColon + 1, equationSegment.length() - indexOfColon - 1);

			Number * log = new Log(Calculate(base), Calculate(argument));
			return *log;
		} else if (numType == 3 || numType == 4) {
			if (numType == 3) {
				int indexOfRT = equationSegment.find("rt:");
				if (indexOfRT > 0) {
					std::string base = equationSegment;
					std::string argument = equationSegment;
					base = base.substr(0, indexOfRT + 1);
					argument = argument.substr(indexOfRT + 3, equationSegment.length() - indexOfRT - 4);
					Number * root = new Root(Calculate(base), Calculate(argument));
					return *root;
				} else {
					throw std::runtime_error("An unrecognized root was supplied.");
				}
			} else if (numType == 4) {
				int indexOfRT = equationSegment.find("sqrt:");
				if (indexOfRT >= 0) {
					std::string argument = equationSegment;
					argument = argument.substr(indexOfRT + 5, equationSegment.length() - indexOfRT - 6);
					Number * integer = new Integer(2);
					Number * root = new Root(*integer, Calculate(argument));
					return *root;
				} else {
					throw std::runtime_error("An unrecognized quare root was supplied.");
				}
			}
		} else if (numType == 6) {
			for (int i = 0; i < equationSegment.length(); i++) {
				if (isalpha(equationSegment.at(i))) {
					if (i + 1 == equationSegment.length() && isalpha(equationSegment.at(i))) {
						std::string variable = "";
						variable += equationSegment.at(i);
						Variable * var = new Variable(variable);
						return *var;
					} else if (equationSegment.at(i) == 'p' && equationSegment.at(i+1) == 'i' && (i + 2 == equationSegment.length() || !isalpha(equationSegment.at(i+2)) || equationSegment.at(i+2) != ':' || equationSegment.at(i+2) != '^')) {
						std::string variable = "";
						variable += equationSegment.at(i);
						variable += equationSegment.at(i + 1);
						Variable * var = new Variable(variable);
						return *var;
					} else if (isalpha(equationSegment.at(i)) && (!isalpha(equationSegment.at(i+1)) || equationSegment.at(i+1) != ':' || equationSegment.at(i+1) != '^')) {
						std::string variable = "";
						variable += equationSegment.at(i);
						Variable * var = new Variable(variable);
						return *var;
					} else {
						throw std::runtime_error("An unrecognized variable was supplied. Please try again with one letter variables or pi.");
					}
				}
			}
		}
	}
}

int Calculator::CheckNumberType(string number) {
	int indexOfColon = 0;
	int indexOfCarrot = 0;

	if (IsPlaceholder(number)) return 5;

	for (int i = 0; i < number.length(); i++) {
		if (isalpha(number.at(i))) {
			if (i + 1 == number.length() && isalpha(number.at(i))) {
				return 6;
			}
			else if ((number.at(i) == 'p' && number.at(i+1) == 'i' && (i + 2 == number.length() || !isalpha(number.at(i+2)) || number.at(i+2) != ':' || number.at(i+2) != '^')) || (isalpha(number.at(i)) && (!isalpha(number.at(i+1)) || number.at(i+1) != ':' || number.at(i+1) != '^'))) {
				return 6;
			}
		}
	}

	for (int i = 0; i < number.length(); i++) {
		if (number[i] == ':') {
			indexOfColon = i;
		} else if (number[i] == '^') {
			indexOfCarrot = i;
		}
	}

	if (indexOfColon > 0 && (indexOfColon < indexOfCarrot || indexOfCarrot == 0 || indexOfCarrot == -1)) {
		string priorColon = number;
		priorColon = priorColon.substr(indexOfColon);

		int rtPosition = number.find("rt:");
		int logPosition = number.find("log_");

		if (rtPosition > 0 && (rtPosition < logPosition || logPosition == 0 || logPosition == -1)) {
			int sqrtPosition = number.find("sqrt:");

			if (sqrtPosition >= 0 && sqrtPosition < rtPosition) {
				return 4;
			}

			return 3;
		} else if (logPosition >= 0 && (logPosition < rtPosition || rtPosition == 0 || rtPosition == -1)) {
			return 2;
		}

	} else if (indexOfCarrot > 0 && (indexOfCarrot < indexOfColon || indexOfColon == 0 || indexOfColon == -1)) {
		return 1;
	} else {
		return 0;
	}
}

bool Calculator::IsPlaceholder(string number) {
	int openParanthases = 0;
	int closeParanthases = 0;
	bool isPlaceholder = false;

	if (number[0] == '(' && number[number.length() - 1] == ')') {
		number = number.substr(1, number.length() - 2);
	}

	for (int i = 0; i < number.length(); i++) {
		if (number[i] == '(') {
			openParanthases++;
		}

		if (number[i] == ')') {
			closeParanthases++;
		}

		if (openParanthases == closeParanthases && (number[i] == '*' || number[i] == '/' || number[i] == '+' || number[i] == '-')) {
			isPlaceholder = true;
			i = number.length();
		}
	}

	return isPlaceholder;
}
