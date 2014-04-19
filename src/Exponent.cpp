/*
 * Exponent.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Exponent.h"

Exponent::Exponent(Number& base, Number& exponent) {
	this->base = &base;
	this->exponent = &exponent;
}

Exponent::~Exponent() {
	// TODO Auto-generated destructor stub
}

Number& Exponent::getBase() {
	return *this->base;
}

Number& Exponent::getExponent() {
	return *this->exponent;
}

Number& Exponent::operator+(Number& rhs) {
	if (Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs)) {
		if((*rhsCast->base == *this->base) && (*rhsCast->exponent == *this->exponent)) {
            Number * integer = new Integer(2);
        	std::vector<Number*> * numbers = new std::vector<Number*>();
        	std::vector<char> * operators = new std::vector<char>();
            numbers->push_back(this);
            numbers->push_back(integer);
            operators->push_back('+');
            Number * result = new Placeholder(*numbers, *operators);
            return *result;
		}
	}
	else {
        if (typeid(rhs) == typeid(Placeholder)) {
            return rhs + *this;
        }

        else {
        	std::vector<Number*> * numbers = new std::vector<Number*>();
        	std::vector<char> * operators = new std::vector<char>();
            numbers->push_back(this);
            numbers->push_back(&rhs);
            operators->push_back('+');
            Number * placeholder = new Placeholder(*numbers, *operators);
            return *placeholder;
        }
    }
}

Number& Exponent::operator-(Number& rhs) {
	if (Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs)) {
		if((*rhsCast->base == *this->base) && (*rhsCast->exponent == *this->exponent)) {
            Number * integer = new Integer(0);
            return *integer;
		}
	}
	else {
        if (typeid(rhs) == typeid(Placeholder)) {
        	Integer * neg = new Integer(-1);
            return (*neg * rhs) + *this;
        }

        else {
        	std::vector<Number*> * numbers = new std::vector<Number*>();
        	std::vector<char> * operators = new std::vector<char>();
            numbers->push_back(this);
            numbers->push_back(&rhs);
            operators->push_back('-');
            Number * placeholder = new Placeholder(*numbers, *operators);
            return *placeholder;
        }
    }
}

Number& Exponent::operator*(Number& rhs) {
	if(typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);

		if(rhsCastExp->getBase() == *this->base){
			if(typeid(*this->exponent) != typeid(rhsCastExp->getExponent())){
				std::vector<Number*> * numbers = new std::vector<Number*>();
				std::vector<char> * operators = new std::vector<char>();
				numbers->push_back(this->exponent);
				numbers->push_back(&rhsCastExp->getExponent());
				operators->push_back('+');
				Number * placeholder = new Placeholder(*numbers, *operators);
				Number * expon = new Exponent(*base, *placeholder);
				return * expon;
			}
			else{
				Number * times = &(rhsCastExp->getExponent() + *this->exponent);
				Number * expon = new Exponent(*base, *times);
				return * expon;
			}
		}
	}
	else {
		if (typeid(rhs) == typeid(Placeholder)) {
			return rhs * *this;
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(this);
			numbers->push_back(&rhs);
			operators->push_back('*');
			Number * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Exponent::operator/(Number& rhs) {
    if(typeid(rhs) == typeid(Exponent)) {
    	Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);

        if(rhsCastExp->getBase() == *this->base){
            if(typeid(*this->exponent) != typeid(rhsCastExp->getExponent())){
            	std::vector<Number*> * numbers = new std::vector<Number*>();
            	std::vector<char> * operators = new std::vector<char>();
                numbers->push_back(this->exponent);
                numbers->push_back(&rhsCastExp->getExponent());
                operators->push_back('-');
                Number * placeholder = new Placeholder(*numbers, *operators);
                Number * expon = new Exponent(*base, *placeholder);
                return * expon;
            }
            else{
                Number * times = &(rhsCastExp->getExponent() + *this->exponent);
                Number * expon = new Exponent(*base, *times);
                return * expon;
            }
        }
            Number * times = &(rhsCastExp->getExponent() - *this->exponent);
            Number * expon = new Exponent(*base, *times);
            return * expon;
           }
        else {
            if (typeid(rhs) == typeid(Placeholder)) {
                return rhs / *this;
                  }

                else {
                	std::vector<Number*> * numbers = new std::vector<Number*>();
                	std::vector<char> * operators = new std::vector<char>();
                    numbers->push_back(this);
                    numbers->push_back(&rhs);
                    operators->push_back('/');
                    Number * placeholder = new Placeholder(*numbers, *operators);
                    return *placeholder;
                }
        }
}

std::string Exponent::toString() {
	return this->base->toString() + "^" + this->exponent->toString();
}

Number& Exponent::simplify() {
    if ((typeid(*base) == typeid(Integer)) &&  (typeid(*exponent) == typeid(Integer))) {
    	Integer * baseCast = dynamic_cast<Integer*>(base);
    	Integer * exponentCast = dynamic_cast<Integer*>(exponent);
        int powr = pow(baseCast->getInt(), exponentCast->getInt());
        Number *num = new Integer(powr);
        return *num;
    }
    return *this;
}

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Exponent::operator==(Number& rhs) {
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
