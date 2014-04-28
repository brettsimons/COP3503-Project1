/*
 * Variable.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: Brett
 */

#include "Variable.h"

Variable::Variable(std::string variable) {
	var = variable;
}

Variable::~Variable() {

}

Number& Variable::operator+(Number& rhs) {
	if (Variable * rhsCast = dynamic_cast<Variable*>(&rhs)) {
		if (rhsCast->var == this->var) {
			Number * integer = new Integer(2);
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(integer);
			operators->push_back('*');
			Number * result = new Placeholder(*numbers, *operators);
			return *result;
		}
	}

	std::vector<Number*> * numbers = new std::vector<Number*>();
	std::vector<char> * operators = new std::vector<char>();
	numbers->push_back(&this->clone());
	numbers->push_back(&rhs.clone());
	operators->push_back('+');
	Number * result = new Placeholder(*numbers, *operators);
	return *result;
}

Number& Variable::operator-(Number& rhs) {
	if (Variable * rhsCast = dynamic_cast<Variable*>(&rhs)) {
		if (rhsCast->var == this->var) {
			Number * integer = new Integer(0);
			return *integer;
		}
	}

	std::vector<Number*> * numbers = new std::vector<Number*>();
	std::vector<char> * operators = new std::vector<char>();
	numbers->push_back(&this->clone());
	numbers->push_back(&rhs.clone());
	operators->push_back('-');
	Number * result = new Placeholder(*numbers, *operators);
	return *result;
}

Number& Variable::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCastVar = dynamic_cast<Variable*>(&rhs);
		if (rhsCastVar->var == this->var) {
			Number * integer = new Integer(2);
			Exponent * exponent = new Exponent(this->clone(), *integer);
			return *exponent;
		} else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('*');
			Number * result = new Placeholder(*numbers, *operators);
			return *result;
		}
	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if (typeid(rhsCastExp->getBase()) == typeid(Variable)) {
			Variable * rhsCastVar = dynamic_cast<Variable*>(&rhsCastExp->getBase());
			if (rhsCastVar->var == this->var) {
				if (typeid(rhsCastExp->getExponent()) == typeid(Integer)) {
					Integer * integer = dynamic_cast<Integer*>(&rhsCastExp->getExponent());
					Number * newExpInt = new Integer(integer->getInt() + 1);
					Exponent * exponent = new Exponent(rhs.clone(), *newExpInt);
					return *exponent;
				}
			}
		}
	}

	std::vector<Number*> * numbers = new std::vector<Number*>();
	std::vector<char> * operators = new std::vector<char>();
	numbers->push_back(&this->clone());
	numbers->push_back(&rhs.clone());
	operators->push_back('*');
	Number * result = new Placeholder(*numbers, *operators);
	return *result;
}

Number& Variable::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCastVar = dynamic_cast<Variable*>(&rhs);
		if (rhsCastVar->var == this->var) {
			Number * result = new Integer(1);
			return *result;
		} else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('/');
			Number * result = new Placeholder(*numbers, *operators);
			return *result;
		}
	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if (typeid(rhsCastExp->getBase()) == typeid(Variable)) {
			Variable * rhsCastVar = dynamic_cast<Variable*>(&rhsCastExp->getBase());
			if (rhsCastVar->var == this->var) {
				if (typeid(rhsCastExp->getExponent()) == typeid(Integer)) {
					Integer * integer = dynamic_cast<Integer*>(&rhsCastExp->getExponent());
					Number * newExpInt = new Integer(integer->getInt() - 1);
					Exponent * exponent = new Exponent(this->clone(), *newExpInt);

					return *exponent;
				}
			}
		}
	}
	else if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		Exponent * denominator = new Exponent(rhsCast->clone(), rhsCast->getRoot().clone());
		Number * numerator = &(rhs.clone() * this->clone());
		Number * result = &(numerator->clone() / denominator->simplify());
		delete numerator;
		delete denominator;
		return *result;
	}

	std::vector<Number*> * numbers = new std::vector<Number*>();
	std::vector<char> * operators = new std::vector<char>();
	numbers->push_back(&this->clone());
	numbers->push_back(&rhs.clone());
	operators->push_back('/');
	Number * result = new Placeholder(*numbers, *operators);
	return *result;
}

Number& Variable::clone() {
	return *(new Variable(this->var));
}

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Variable::operator==(Number& rhs) {
	if (typeid(*this) == typeid(rhs)) {
		if (typeid(rhs) == typeid(Exponent)) {
			Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);
			Exponent * lhsCast = dynamic_cast<Exponent*>(this);
			if (rhsCast->getBase() == lhsCast->getBase() && rhsCast->getExponent() == lhsCast->getExponent()) {
				return true;
			} else {
				return false;
			}
		} else if (typeid(rhs) == typeid(Log)) {
			Log * rhsCast = dynamic_cast<Log*>(&rhs);
			Log * lhsCast = dynamic_cast<Log*>(this);
			if (rhsCast->getBase() == lhsCast->getBase() && rhsCast->getArgument() == lhsCast->getArgument()) {
				return true;
			} else {
				return false;
			}
		} else if (typeid(rhs) == typeid(Placeholder)) {
			Placeholder * rhsCast = dynamic_cast<Placeholder*>(&rhs);
			Placeholder * lhsCast = dynamic_cast<Placeholder*>(this);

			if (rhsCast->getNumbers().size() == lhsCast->getNumbers().size()) {
				for (int i = 0; i < lhsCast->getNumbers().size(); i++) {
					bool matched = false;

					for (int y = 0; y < rhsCast->getNumbers().size(); y++) {
						if (lhsCast[i] == rhsCast[y]) {
							matched = true;
						}
					}

					if (!matched) {
						return false;
					}
				}

				for (int i = 0; i < lhsCast->getOperators().size(); i++) {
					bool matched = false;

					for (int y = 0; y < rhsCast->getOperators().size(); y++) {
						if (lhsCast->getOperators()[i] == rhsCast->getOperators()[y]) {
							if (lhsCast->getOperators()[i] == '-' || lhsCast->getOperators()[i] == '/') {
								if (lhsCast->getNumbers()[i] == rhsCast->getNumbers()[y] && lhsCast->getNumbers()[i+1] == rhsCast->getNumbers()[y+1]) {
									matched = true;
								} else {
									matched = false;
								}
							} else {
								if (lhsCast->getNumbers()[i] == rhsCast->getNumbers()[y] && lhsCast->getNumbers()[i+1] == rhsCast->getNumbers()[y+1]) {
									matched = true;
								} else if (lhsCast->getNumbers()[i+1] == rhsCast->getNumbers()[y] && lhsCast->getNumbers()[i] == rhsCast->getNumbers()[y+1]) {
									matched = true;
								} else {
									matched = false;
								}
							}
						}
					}

					if (!matched) {
						return false;
					}
				}

				return true;
			} else {
				return false;
			}
		} else if (typeid(rhs) == typeid(Root)) {
			Root * rhsCast = dynamic_cast<Root*>(&rhs);
			Root * lhsCast = dynamic_cast<Root*>(this);
			if (rhsCast->getBase() == lhsCast->getBase() && rhsCast->getRoot() == lhsCast->getRoot()) {
				return true;
			} else {
				return false;
			}
		} else if (typeid(rhs) == typeid(Integer)) {
			Integer * rhsCast = dynamic_cast<Integer*>(&rhs);
			Integer * lhsCast = dynamic_cast<Integer*>(this);

			if (rhsCast->getInt() == lhsCast->getInt()) {
				return true;
			} else {
				return false;
			}
		} else if (typeid(rhs) == typeid(Variable)) {
			Variable * rhsCast = dynamic_cast<Variable*>(&rhs);
			Variable * lhsCast = dynamic_cast<Variable*>(this);

			if (lhsCast->getVariable() == rhsCast->getVariable()) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

std::string Variable::toString() {
	return var;
}

Number& Variable::simplify() {
	return this->clone();
}

std::string Variable::getVariable() {
	return var;
}
