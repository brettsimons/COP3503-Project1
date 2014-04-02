/*
 * Variable.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: Brett
 */

#include "Variable.h"
#include <typeinfo>
#include <sstream>
#include <vector>

Variable::Variable(std::string variable) {
	var = variable;
}

Variable::~Variable() {
	// TODO Auto-generated destructor stub
}

std::string Variable::operator+(Number& rhs) {
	if (Variable * rhsCast = dynamic_cast<Variable*>(&rhs)) {
		if (rhsCast->var == this->var) {
			std::string result = *rhsCast->multiplier + *this->multiplier + "*" + rhsCast->var;
			return result;
		} else {
			std::string result =  this->multiplier->toString() + "*" + this->var + "+" + rhsCast->multiplier->toString() + "*" + rhsCast->var;
			return result;
		}
	} else {
		return this->toString() + "+" + rhs.toString();
	}
}

std::string Variable::operator-(Number& rhs) {
	if (Variable * rhsCast = dynamic_cast<Variable*>(&rhs)) {
		if (rhsCast->var == this->var) {
			std::string result = *rhsCast->multiplier - *this->multiplier + "*" + rhsCast->var;
			return result;
		} else {
			std::string result =  this->multiplier->toString() + "*" + this->var + "-" + rhsCast->multiplier->toString() + "*" + rhsCast->var;
			return result;
		}
	} else {
		return this->toString() + "-" + rhs.toString();
	}
}

std::string Variable::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCastVar = dynamic_cast<Variable*>(&rhs);
		if (rhsCastVar->var == this->var) {
			Number * integer = new Integer(2);
			Exponent * exponent = new Exponent(rhs, *integer);
			std::string result = *rhsCastVar->multiplier * *this->multiplier + "*" + exponent->toString();
			delete integer;
			delete exponent;
			delete rhsCastVar;
			return result;
		} else {
			std::string result =  *this->multiplier * *rhsCastVar->multiplier + "*" + this->var + "*" + rhsCastVar->var;
			delete rhsCastVar;
			return result;
		}
	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if (typeid(rhs) == typeid(Variable)) {
			Variable * rhsCastVar = dynamic_cast<Variable*>(&rhsCastExp->getBase());
			if (rhsCastVar->var == this->var) {
				if (typeid(rhsCastExp->getExponent()) == typeid(Integer)) {
					Integer * integer = dynamic_cast<Integer*>(&rhsCastExp->getExponent());
					Number * newExpInt = new Integer(integer->getInt() + 1);
					Exponent * exponent = new Exponent(rhs, *newExpInt);
					std::string result = *rhsCastVar->multiplier * *this->multiplier + "*" + exponent->toString();
					delete integer;
					delete exponent;
					delete newExpInt;
					delete rhsCastVar;
					delete rhsCastExp;
					return result;
				}
			}

			std::string result = *rhsCastVar->multiplier * *this->multiplier + "*" + this->var + "*" + rhsCastExp->toString();
			delete rhsCastVar;
			delete rhsCastExp;
			return result;
		} else {
			std::string rhsExpString = rhsCastExp->toString();
			delete rhsCastExp;
			return this->toString() + "*" + rhsExpString;
		}
	} else {
		std::string newMultiplier = *this->multiplier * rhs;
		return newMultiplier + "*" + this->var;
	}
}

std::string Variable::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCastVar = dynamic_cast<Variable*>(&rhs);
		if (rhsCastVar->var == this->var) {
			std::string result = *this->multiplier / *rhsCastVar->multiplier;
			delete rhsCastVar;
			return result;
		} else {
			std::string result =  *this->multiplier / *rhsCastVar->multiplier;
			if (result.find('/') != std::string::npos) {
				result = "(" + result.substr(0, result.find('/')) + "*" + this->var + ")/(" + result.substr(result.find('/') + 1) + "*" + rhsCastVar->var + ")";
			} else {
				result = "(" + result + "*" + this->var + ")/" + rhsCastVar->var;
			}
			delete rhsCastVar;
			return result;
		}
	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if (typeid(rhs) == typeid(Variable)) {
			Variable * rhsCastVar = dynamic_cast<Variable*>(&rhsCastExp->getBase());
			if (rhsCastVar->var == this->var) {
				if (typeid(rhsCastExp->getExponent()) == typeid(Integer)) {
					Integer * integer = dynamic_cast<Integer*>(&rhsCastExp->getExponent());
					Number * newExpInt = new Integer(integer->getInt() - 1);
					Exponent * exponent = new Exponent(rhs, *newExpInt);

					std::string result =  *this->multiplier / *rhsCastVar->multiplier;
					if (result.find('/') != std::string::npos) {
						result = "(" + result.substr(0, result.find('/')) + "*" + exponent->toString() + ")/" + result.substr(result.find('/') + 1);
					} else {
						result = "(" + result + "*" + this->var + ")/" + rhsCastVar->var;
					}

					delete integer;
					delete exponent;
					delete newExpInt;
					delete rhsCastVar;
					delete rhsCastExp;
					return result;
				}
			}

			std::string result = *this->multiplier /  *rhsCastVar->multiplier;
			if (result.find('/') != std::string::npos) {
				result = "(" + result.substr(0, result.find('/')) + "*" + this->var + ")/(" + result.substr(result.find('/') + 1) + "*" + rhsCastExp->toString() + ")";
			} else {
				result = "(" + result + "*" + this->var + ")/" + rhsCastExp->toString();
			}

			delete rhsCastVar;
			delete rhsCastExp;
			return result;
		} else {
			std::string rhsExpString = rhsCastExp->toString();
			delete rhsCastExp;
			return this->toString() + "/" + rhsExpString;
		}
	} else {
		std::string newMultiplier = *this->multiplier / rhs;
		return newMultiplier + "*" + this->var;
	}
}

std::string Variable::toString() {
	return multiplier->toString() + "*" + var;
}

std::vector<std::pair<Number*, char> > Variable::simplify() {
	std::vector<std::pair<Number*, char> > vectorPair;
	Number * num = this;
	vectorPair.push_back(std::make_pair<Number*, char>(num, NULL));

	return vectorPair;
}

std::string Variable::getVariable() {
	return var;
}

Number& Variable::getMultiplier() {
	return *multiplier;
}
