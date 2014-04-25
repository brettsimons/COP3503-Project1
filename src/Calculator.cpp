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
	for (int i = 0; i < this->numbers->size(); i++) {
		delete this->numbers->at(i);
	}

	this->numbers->clear();
	this->operators->clear();

	delete this->numbers;
	delete this->operators;
}

string Calculator::SimplifyExpression(string equation) {
	equation.erase(std::remove(equation.begin(), equation.end(), ' '), equation.end());
	int indexOfLastOp = -1;
	bool hasOperators = true;

	for (int i = 0; i < equation.length(); i++) {
		string equationSegment = equation;

		if (i == 0 && equation[i] == '-') {
			Number * lhs = new Integer(-1);
			numbers->push_back(lhs);
			operators->push_back('*');
			indexOfLastOp = i;
		} else if (i + 1 == equation.length()) {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equationSegment.length()));
			numbers->push_back(&lhs->simplify());
		}
		else if (equation[i] == '(') {
			hasOperators = false;
			bool foundClose = false;
			bool isEnd = false;
			int closeBracket = -1;
			int indexOfNextOp = -1;
			int paranthases = 0;

			for (int x = i + 1; x < equation.length(); x++) {
				if (equation[x] == ')' && x + 1 == equation.length()) {
					closeBracket = x;
					i = x;
				} else if (equation[x] == ')' && paranthases == 0) {
					i = x;
					closeBracket = x;
				}

				if (equation[x] == '(') {
					paranthases++;
				}
				else if (equation[x] == ')' && paranthases > 0) {
					paranthases--;
				}

				if (paranthases == 0 && closeBracket >= i && (equation[x] == '*' || equation[x] == '/' || equation[x] == '+' || (i > 0 && (equation[i] == '-' && i != 0 && equation[i - 1] != ':' && equation[i - 1] != '^')))) {
					hasOperators = true;
					indexOfNextOp = x;
					break;
				}
			}

			if (isEnd) {
				Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equation.length()));
				numbers->push_back(&lhs->simplify());

				indexOfLastOp = i;
			} else if (closeBracket != -1) {
				if (equation.length() > closeBracket + 3 && equation[closeBracket + 1] == 'r' && equation[closeBracket + 2] == 't' && equation[closeBracket + 3] == ':') {
					Number * root = &Calculate(equationSegment.substr(indexOfLastOp + 1, indexOfLastOp + 1 + closeBracket + 1));

					if (indexOfNextOp > -1) {
						Number * base = &Calculate(equationSegment.substr(closeBracket + 4, indexOfNextOp - closeBracket + 4));
						Number * lhs = new Root(*base, *root);
						numbers->push_back(&lhs->simplify());
						operators->push_back(equationSegment.substr(indexOfNextOp, 1).c_str()[0]);
					}
					else {
						Number * base = &Calculate(equationSegment.substr(closeBracket + 4, equation.length() - closeBracket + 4));
						Number * lhs = new Root(*base, *root);
						numbers->push_back(&lhs->simplify());
					}

					indexOfLastOp = i + 1;
					if (indexOfNextOp != -1) {
						i = indexOfNextOp;
					}
					else {
						i = equation.length();
					}
				}
				else if (indexOfNextOp > -1) {
					Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, indexOfNextOp - indexOfLastOp - 1));
					numbers->push_back(&lhs->simplify());
					operators->push_back(equationSegment.substr(indexOfNextOp, 1).c_str()[0]);

					indexOfLastOp = i + 1;
					if (indexOfNextOp != -1) {
						i = indexOfNextOp;
					}
				}
				else if (hasOperators || i + 1 == equation.length()) {
					Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, indexOfLastOp + 1 + closeBracket + 1));
					numbers->push_back(&lhs->simplify());

					indexOfLastOp = i + 1;
					if (indexOfNextOp != -1) {
						i = indexOfNextOp;
					}
				}
			} else {
				Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, equation.length()));
				numbers->push_back(&lhs->simplify());

				indexOfLastOp = i;
			}
		}
		else if (equation[i] == '*') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(&lhs->simplify());
			operators->push_back('*');
			hasOperators = true;

			if (equation[i + 1] == '-') {
				Number * lhs = new Integer(-1);
				numbers->push_back(lhs);
				operators->push_back('*');
				i++;
			}

			indexOfLastOp = i;
		} else if (equation[i] == '/') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(&lhs->simplify());
			operators->push_back('/');
			hasOperators = true;

			if (equation[i + 1] == '-') {
				Number * lhs = new Integer(-1);
				numbers->push_back(lhs);
				operators->push_back('*');
				i++;
			}

			indexOfLastOp = i;
		}
		else if (equation[i] == '+') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(&lhs->simplify());

			if (equation[i + 1] == '-') {
				operators->push_back('-');
				i++;
			}
			else {
				operators->push_back('+');
			}

			hasOperators = true;
			indexOfLastOp = i;
		}
		else if (equation[i] == '-' && equation[i - 1] != ':' && equation[i - 1] != '^') {
			Number * lhs = &Calculate(equationSegment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
			numbers->push_back(&lhs->simplify());

			if (equation[i + 1] == '-') {
				operators->push_back('+');
				i++;
			}
			else {
				operators->push_back('-');
			}

			hasOperators = true;
			indexOfLastOp = i;
		}
	}

	Number * result = &(PerformCalculations());

	string stringResult = "";
	stringResult = result->toString();

	delete result;
	return stringResult;
}

Number& Calculator::PerformCalculations() {
	for (int i = 0; i < operators->size(); i++) {
		if (operators->at(i) == '*') {
			Number * result = &(*numbers->at(i) * *numbers->at(i + 1));
			Number * simplified = &result->simplify();
			delete numbers->at(i);
			delete numbers->at(i + 1);
			delete result;
			numbers->at(i) = &simplified->simplify();
			delete simplified;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		} else if (operators->at(i) == '/') {
			Number * result = &(*numbers->at(i) / *numbers->at(i + 1));
			Number * simplified = &result->simplify();
			delete numbers->at(i);
			delete numbers->at(i + 1);
			delete result;
			numbers->at(i) = &simplified->simplify();
			delete simplified;
			numbers->erase(numbers->begin() + i + 1);
			operators->erase(operators->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < operators->size(); i++) {
		if (operators->at(i) == '+') {
			if (typeid(*numbers->at(i)) != typeid(*numbers->at(i + 1))) {
				Number * result = NULL;
				int x = 0;
				for (x = i + 1; x < numbers->size(); x++) {
					if (typeid(*numbers->at(i)) == typeid(*numbers->at(x)) && operators->at(x - 1) == '-') {
						result = &(*numbers->at(i) - *numbers->at(x));
						delete numbers->at(i);
						delete numbers->at(x);
					}
					else if (typeid(*numbers->at(i)) == typeid(*numbers->at(x)) && operators->at(x - 1) == '+') {
						result = &(*numbers->at(i) + *numbers->at(x));
						delete numbers->at(i);
						delete numbers->at(x);
					}
				}
				x--;
				if (result != NULL) {
					numbers->at(i) = &result->simplify();
					delete result;
					numbers->erase(numbers->begin() + x);
					operators->erase(operators->begin() + x - 1);
				}
				else if (typeid(*numbers->at(i)) == typeid(Integer) && typeid(*numbers->at(x)) == typeid(Placeholder)) {
					Number * result = &(*numbers->at(i) + *numbers->at(i + 1));
					delete numbers->at(i);
					delete numbers->at(i + 1);
					numbers->at(i) = &result->simplify();
					delete result;
					numbers->erase(numbers->begin() + i + 1);
					operators->erase(operators->begin() + i);
					i--;
				}
			} else {
				Number * result = &(*numbers->at(i) + *numbers->at(i + 1));
				delete numbers->at(i);
				delete numbers->at(i + 1);
				numbers->at(i) = &result->simplify();
				delete result;
				numbers->erase(numbers->begin() + i + 1);
				operators->erase(operators->begin() + i);
				i--;
			}
		} else if (operators->at(i) == '-') {
			if (typeid(*numbers->at(i)) != typeid(*numbers->at(i + 1))) {
				Number * result = NULL;
				int x = 0;
				for (x = i + 1; x < numbers->size(); x++) {
					if (typeid(*numbers->at(i)) == typeid(*numbers->at(x))) {
						result = &(*numbers->at(i) - *numbers->at(x));
						delete numbers->at(i);
						delete numbers->at(x);
					}
				}
				x--;
				if (result != NULL) {
					numbers->at(i) = &result->simplify();
					delete result;
					numbers->erase(numbers->begin() + x);
					operators->erase(operators->begin() + x - 1);
				}
				else if (typeid(*numbers->at(i)) == typeid(Integer) && typeid(*numbers->at(x)) == typeid(Placeholder)) {
					Number * result = &(*numbers->at(i) - *numbers->at(i + 1));
					delete numbers->at(i);
					delete numbers->at(i + 1);
					numbers->at(i) = &result->simplify();
					delete result;
					numbers->erase(numbers->begin() + i + 1);
					operators->erase(operators->begin() + i);
					i--;
				}
			} else {
				Number * result = &(*numbers->at(i) - *numbers->at(i + 1));
				delete numbers->at(i);
				delete numbers->at(i + 1);
				numbers->at(i) = &result->simplify();
				delete result;
				numbers->erase(numbers->begin() + i + 1);
				operators->erase(operators->begin() + i);
				i--;
			}
		}
	}

	if (!operators->empty()) {
		std::vector<Number*> * localNumbers = new vector<Number*>();
		std::vector<char> * localOperators = new vector<char>();

		for (int i = 0; i < this->numbers->size(); i++) {
			localNumbers->push_back(this->numbers->at(i));

			if (i < this->operators->size()) {
				localOperators->push_back(this->operators->at(i));
			}
		}

		Number * placeholder = new Placeholder(*this->numbers, *this->operators);
		return *placeholder;
	} else {
		return numbers->at(0)->clone();
	}
}

Number& Calculator::PerformCalculations(Placeholder * toCalculate) {
	for (int i = 0; i < toCalculate->getOperators().size(); i++) {
		if (toCalculate->getOperators().at(i) == '*') {
			Number * result = &(*toCalculate->getNumbers().at(i) * *toCalculate->getNumbers().at(i + 1));
			delete toCalculate->getNumbers().at(i);
			delete toCalculate->getNumbers().at(i + 1);
			toCalculate->getNumbers().at(i) = &result->simplify();
			delete result;
			toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + i + 1);
			toCalculate->getOperators().erase(toCalculate->getOperators().begin() + i);
			i--;
		}
		else if (toCalculate->getOperators().at(i) == '/') {
			Number * result = &(*toCalculate->getNumbers().at(i) / *toCalculate->getNumbers().at(i + 1));
			delete toCalculate->getNumbers().at(i);
			delete toCalculate->getNumbers().at(i + 1);
			toCalculate->getNumbers().at(i) = &result->simplify();
			delete result;
			toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + i + 1);
			toCalculate->getOperators().erase(toCalculate->getOperators().begin() + i);
			i--;
		}
	}

	for (int i = 0; i < toCalculate->getOperators().size(); i++) {
		if (toCalculate->getOperators().at(i) == '+') {
			if (typeid(*toCalculate->getNumbers().at(i)) != typeid(*toCalculate->getNumbers().at(i + 1))) {
				Number * result = NULL;
				int x = 0;
				for (x = i + 1; x < toCalculate->getNumbers().size(); x++) {
					if (typeid(*toCalculate->getNumbers().at(i)) == typeid(*toCalculate->getNumbers().at(x)) && toCalculate->getOperators().at(x - 1) == '+') {
						result = &(*toCalculate->getNumbers().at(i) + *toCalculate->getNumbers().at(x));
						delete toCalculate->getNumbers().at(i);
						delete toCalculate->getNumbers().at(x);
					}
					else if (typeid(*toCalculate->getNumbers().at(i)) == typeid(*toCalculate->getNumbers().at(x)) && toCalculate->getOperators().at(x - 1) == '-') {
						result = &(*toCalculate->getNumbers().at(i) - *toCalculate->getNumbers().at(x));
						delete toCalculate->getNumbers().at(i);
						delete toCalculate->getNumbers().at(x);
					}
				}
				x--;
				if (result != NULL) {
					toCalculate->getNumbers().at(i) = &result->simplify();
					delete result;
					toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + x);
					toCalculate->getOperators().erase(toCalculate->getOperators().begin() + x - 1);
				}
			}
			else {
				Number * result = &(*toCalculate->getNumbers().at(i) + *toCalculate->getNumbers().at(i + 1));
				delete toCalculate->getNumbers().at(i);
				delete toCalculate->getNumbers().at(i + 1);
				toCalculate->getNumbers().at(i) = &result->simplify();
				delete result;
				toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + i + 1);
				toCalculate->getOperators().erase(toCalculate->getOperators().begin() + i);
				i--;
			}
		}
		else if (toCalculate->getOperators().at(i) == '-') {
			if (typeid(*toCalculate->getNumbers().at(i)) != typeid(*toCalculate->getNumbers().at(i + 1))) {
				Number * result = NULL;
				int x = 0;
				for (x = i + 1; x < toCalculate->getNumbers().size(); x++) {
					if (typeid(*toCalculate->getNumbers().at(i)) == typeid(*toCalculate->getNumbers().at(x))) {
						result = &(*toCalculate->getNumbers().at(i) - *toCalculate->getNumbers().at(x));
						delete toCalculate->getNumbers().at(i);
						delete toCalculate->getNumbers().at(x);
					}
				}
				x--;
				if (result != NULL) {
					toCalculate->getNumbers().at(i) = &result->simplify();
					delete result;
					toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + x);
					toCalculate->getOperators().erase(toCalculate->getOperators().begin() + x - 1);
				}
			}
			else {
				Number * result = &(*toCalculate->getNumbers().at(i) - *toCalculate->getNumbers().at(i + 1));
				delete toCalculate->getNumbers().at(i);
				delete toCalculate->getNumbers().at(i + 1);
				toCalculate->getNumbers().at(i) = &result->simplify();
				delete result;
				toCalculate->getNumbers().erase(toCalculate->getNumbers().begin() + i + 1);
				toCalculate->getOperators().erase(toCalculate->getOperators().begin() + i);
				i--;
			}
		}
	}

	if (!toCalculate->getOperators().empty()) {
		Number * placeholder = &toCalculate->clone();
		return *placeholder;
	}
	else {
		return toCalculate->getNumbers().at(0)->simplify();
	}
}

Number& Calculator::Calculate(string equationSegment) {
	int numType = CheckNumberType(equationSegment);

	if (equationSegment[0] == '(' && equationSegment[equationSegment.length() - 1] == ')' && numType == 5) {
		std::vector<Number*> * localNumbers = new vector<Number*>();
		std::vector<char> * localOperators = new vector<char>();

		int indexOfLastOp = 0;
		for (int i = 1; i < equationSegment.length(); i++) {
			string segment = equationSegment;

			if (i == 1 && equationSegment[i] == '-') {
				Number * lhs = new Integer(-1);
				localNumbers->push_back(lhs);
				localOperators->push_back('*');
				indexOfLastOp = i;
			}
			else if (i + 1 == equationSegment.length()) {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, segment.length() - indexOfLastOp - 2));
				localNumbers->push_back(&lhs->simplify());
			} else if (equationSegment[i] == '*') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(&lhs->simplify());
				localOperators->push_back('*');

				if (equationSegment[i + 1] == '-') {
					Number * lhs = new Integer(-1);
					localNumbers->push_back(lhs);
					localOperators->push_back('*');
					i++;
				}

				indexOfLastOp = i;
			} else if (equationSegment[i] == '/') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(&lhs->simplify());
				localOperators->push_back('/');

				if (equationSegment[i + 1] == '-') {
					Number * lhs = new Integer(-1);
					localNumbers->push_back(lhs);
					localOperators->push_back('*');
					i++;
				}

				indexOfLastOp = i;
			} else if (equationSegment[i] == '+') {
				Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
				localNumbers->push_back(&lhs->simplify());

				if (equationSegment[i + 1] == '-') {
					localOperators->push_back('-');
					i++;
				}
				else {
					localOperators->push_back('+');
				}

				indexOfLastOp = i;
			} else if (equationSegment[i] == '-') {
				if (indexOfLastOp + 1 == i) {
					Placeholder * lhs = new Placeholder();
					lhs->getNumbers().push_back(new Integer(-1));
					lhs->getNumbers().push_back(new Integer(atoi(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1).c_str())));
				} else {
					Number * lhs = &Calculate(segment.substr(indexOfLastOp + 1, i - indexOfLastOp - 1));
					localNumbers->push_back(&lhs->simplify());

					if (equationSegment[i + 1] == '-') {
						localOperators->push_back('+');
						i++;
					}
					else {
						localOperators->push_back('-');
					}
				}

				indexOfLastOp = i;
			}
		}

		Placeholder * placeholder = new Placeholder(*localNumbers, *localOperators);
		Number * result = &placeholder->clone();

		if (localOperators->size() > 0) {
			result = &PerformCalculations(placeholder);
		}

		delete placeholder;

		Number * simplified = &result->simplify();
		delete result;
		return *simplified;
	} else {
		if (equationSegment[0] == '(' && equationSegment[equationSegment.length() - 1] == ')' && numType != 5) {
			equationSegment = equationSegment.substr(1, equationSegment.length() - 2);
		}

		if (numType == 0) {
			Number * integer = new Integer(atoi(equationSegment.c_str()));
			return *integer;
		}
		else if (numType == 1) {
			int indexOfCarrot = 0;
			int paranthases = 0;
			for (int i = 0; i < equationSegment.length(); i++) {
				if (equationSegment[i] == '(') {
					paranthases++;
				}
				else if (equationSegment[i] == ')' && paranthases > 0) {
					paranthases--;
				}

				if (equationSegment[i] == '^' && paranthases == 0) {
					indexOfCarrot = i;
					break;
				}
			}
			std::string beforeCarrot = equationSegment;
			std::string afterCarrot = equationSegment;
			beforeCarrot = beforeCarrot.substr(0, indexOfCarrot);
			afterCarrot = afterCarrot.substr(indexOfCarrot + 1, equationSegment.length() - indexOfCarrot - 1);

			Number * raisedTo = &Calculate(afterCarrot);
			Number * toRaise = &Calculate(beforeCarrot);

			if (typeid(*raisedTo) == typeid(Placeholder)) {
				Placeholder * holder = dynamic_cast<Placeholder*>(&raisedTo->clone());
				if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
					Root * innerExponent = new Root(toRaise->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
					toRaise = &innerExponent->simplify();
					delete innerExponent;
					if (holder->getOperators().size() > 1) {
						holder->getNumbers().pop_back();
						holder->getOperators().pop_back();
						raisedTo = &holder->clone();
						delete holder;
					}
					else {
						raisedTo = &holder->getNumbers()[0]->clone();
						delete holder;
					}
				}
			}

			Number * exponent = new Exponent(toRaise->clone(), raisedTo->clone());
			Number * simplified = &exponent->simplify();
			delete exponent;
			delete toRaise;
			delete raisedTo;
			return *simplified;
		} else if (numType == 2) {
			int indexOfColon = 0;
			int paranthases = 0;
			for (int i = 0; i < equationSegment.length(); i++) {
				if (equationSegment[i] == '(') {
					paranthases++;
				}
				else if (equationSegment[i] == ')' && paranthases > 0) {
					paranthases--;
				}

				if (equationSegment[i] == ':' && paranthases == 0) {
					indexOfColon = i;
					break;
				}
			}
			std::string base = equationSegment;
			std::string argument = equationSegment;
			base = base.substr(4, indexOfColon - 4);
			argument = argument.substr(indexOfColon + 1, equationSegment.length() - indexOfColon - 1);

			Number * log = new Log(Calculate(base), Calculate(argument));
			Number * simplified = &log->simplify();
			delete log;
			return *simplified;
		} else if (numType == 3 || numType == 4) {
			if (numType == 3) {
				int indexOfRT = equationSegment.find("rt:");
				if (indexOfRT > 0) {
					std::string base = equationSegment;
					std::string argument = equationSegment;
					base = base.substr(0, indexOfRT);
					argument = argument.substr(indexOfRT + 3, equationSegment.length() - indexOfRT - 3);
					Number * root = new Root(Calculate(argument), Calculate(base));
					Number * simplified = &root->simplify();
					delete root;
					return *simplified;
				} else {
					throw std::runtime_error("An unrecognized root was supplied.");
				}
			} else if (numType == 4) {
				int indexOfRT = equationSegment.find("sqrt:");
				if (indexOfRT >= 0) {
					std::string argument = equationSegment;
					argument = argument.substr(indexOfRT + 5, equationSegment.length() - indexOfRT - 5);
					Number * integer = new Integer(2);
					Number * root = new Root(Calculate(argument), *integer);
					Number * simplified = &root->simplify();
					delete root;
					return *simplified;
				} else {
					throw std::runtime_error("An unrecognized square root was supplied.");
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
	int indexOfColon = -1;
	int indexOfCarrot = -1;

	if (IsPlaceholder(number)) return 5;

	for (int i = 0; i < number.length(); i++) {
		if (number[i] == ':') {
			indexOfColon = i;
		}
		else if (number[i] == '^') {
			indexOfCarrot = i;
		}
	}

	if (indexOfColon == -1 && indexOfCarrot == -1) {
		for (int i = 0; i < number.length(); i++) {
			if (isalpha(number.at(i))) {
				if (i + 1 == number.length() && isalpha(number.at(i))) {
					return 6;
				}
				else if ((number.at(i) == 'p' && number.at(i + 1) == 'i' && (i + 2 == number.length() || !isalpha(number.at(i + 2)) || number.at(i + 2) != ':' || number.at(i + 2) != '^')) || (isalpha(number.at(i)) && (!isalpha(number.at(i + 1)) || number.at(i + 1) != ':' || number.at(i + 1) != '^'))) {
					return 6;
				}
			}
		}
	}

	if (indexOfColon > 0 && (indexOfColon < indexOfCarrot || indexOfCarrot == 0 || indexOfCarrot == -1)) {
		string priorColon = number;
		priorColon = priorColon.substr(indexOfColon);

		int rtPosition = number.find("rt:");
		int logPosition = number.find("log_");

		if (rtPosition > 0 && (rtPosition < logPosition || logPosition == -1)) {
			int sqrtPosition = number.find("sqrt:");

			if (sqrtPosition >= 0 && sqrtPosition < rtPosition) {
				return 4;
			}

			return 3;
		} else if (logPosition >= 0 && (logPosition < rtPosition || rtPosition == -1)) {
			return 2;
		}

	} else if (indexOfCarrot > 0 && (indexOfCarrot < indexOfColon || indexOfColon == -1)) {
		return 1;
	} else {
		return 0;
	}
}

bool Calculator::IsPlaceholder(string number) {
	int openParanthases = 0;
	int closeParanthases = 0;
	bool isPlaceholder = false;

	for (int i = 0; i < number.length(); i++) {
		if (number[i] == '(') {
			openParanthases++;
		}

		if (number[i] == ')') {
			closeParanthases++;
		}

		if (openParanthases == closeParanthases && (number[i] == '^' || number[i] == ':')) {
			return false;
		}
		else if (openParanthases != closeParanthases && (number[i] == '*' || number[i] == '/' || number[i] == '+' || (number[i] == '-' && i != 0 && number[i - 1] != ':' && number[i - 1] != '^'))) {
			isPlaceholder = true;
		}
	}

	return isPlaceholder;
}
