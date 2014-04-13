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
		if((typeid(rhsCast->base) == typeid(this->base) == typeid(Variable)) || (typeid(rhsCast->exponent) == typeid(this->exponent) == typeid(Variable))) {
			if ((rhsCast->base == this->base) && (rhsCast->exponent == this->exponent)) {
				delete rhsCast;
				Number * integer = new Integer(2);
				std::vector<Number*> numbers;
				std::vector<char> operators;
				numbers.push_back(this);
				numbers.push_back(integer);
				operators.push_back('*');
				Number * result = new Placeholder(numbers, operators);
				return *result;
			}
		}
		delete rhsCast;
	}
    
	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('+');
	Number * result = new Placeholder(numbers, operators);
	return *result;
}

Number& Exponent::operator-(Number& rhs) {
	if (Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs)) {
		if((typeid(rhsCast->base) == typeid(this->base) == typeid(Variable)) || (typeid(rhsCast->exponent) == typeid(this->exponent) == typeid(Variable))) {
			if ((rhsCast->base == this->base) && (rhsCast->exponent == this->exponent)) {
				delete rhsCast;
				Number * integer = new Integer(0);
				return *integer;
			}
            
			delete rhsCast;
		}
	}
	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('-');
	Number * result = new Placeholder(numbers, operators);
	return *result;
}

Number& Variable::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if((typeid(rhsCast->getBase()) == typeid(this->base) == typeid(Variable)) && ((typeid(rhsCast->getExponent()) && typeid(this->exponent)) == typeid(Integer))) {
            if (rhsCastExp->getBase() == this->base) {
				delete rhsCastVar;
				Number * integer = new Integer((rhsCast->getExponent()) +(this->exponent));
				Exponent * exponent = new Exponent(*this, *integer);
				return *exponent;
			}
            else {
				std::vector<Number*> numbers;
				std::vector<char> operators;
				numbers.push_back(this);
				numbers.push_back(&rhs);
				operators.push_back('*');
				Number * result = new Placeholder(numbers, operators);
				delete rhsCastVar;
				return *result;
			}
		}
        else if((typeid(rhsCast->getExponent()) && typeid(this->exponent) == typeid(Variable)) && (typeid(rhsCast->getBase()) == typeid(this->base) == (typeid(Integer))||(typeid(Variable)))) {
            std::vector<Number*> numbers;
			std::vector<char> operators;
            numbers.push_back(rhsCast->getExponent());
            numbers.push_back(this->exponent);
            operators.push_back('+');
            Number * result = new Placeholder(numbers, operators);
            Exponent * exponent = new Exponent(*this->base, *result);
            return *exponent;
		}
        
		delete rhsCastExp;
	}
    
	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('*');
	Number * result = new Placeholder(numbers, operators);
    return *result;
}

Number& Variable::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);
		if ((rhsCastExp->getExponent() == this->exponent) && (rhsCastExp->getBase() == this->base)) {
			Number * result = new Integer(1);
			delete rhsCastExp;
			return *result;
		}
        
        else if((typeid(rhsCast->base) == typeid(this->base) == typeid(Variable)) && ((typeid(rhsCast->exponent) && typeid(this->exponent)) == typeid(Integer))) {
                if (rhsCastExp->base == this->base)  {
                    delete rhsCastVar;
                    Number * integer = new Integer((rhsCast->exponent) - (this->exponent));
                    Exponent * exponent = new Exponent(*this, *integer);
                    return *exponent;
                } else {
                    std::vector<Number*> numbers;
                    std::vector<char> operators;
                    numbers.push_back(this);
                    numbers.push_back(&rhs);
                    operators.push_back('*');
                    Number * result = new Placeholder(numbers, operators);
                    delete rhsCastVar;
                    return *result;
                }
            }
            
            else if((typeid(rhsCast->getExponent()) && typeid(this->exponent) == typeid(Variable)) && (typeid(rhsCast->getBase()) == typeid(this->base) == (typeid(Integer))||(typeid(Variable)))) {
                std::vector<Number*> numbers;
                std::vector<char> operators;
                numbers.push_back(rhsCast->getExponent());
                numbers.push_back(this->exponent());
                operators.push_back('-');
                Number * result = new Placeholder(numbers, operators);
                Exponent * exponent = new Exponent(*this->base, *result);
                return *exponent;
            }
            
            delete rhsCastExp;
        }
        
        std::vector<Number*> numbers;
        std::vector<char> operators;
        numbers.push_back(this);
        numbers.push_back(&rhs);
        operators.push_back('*');
        Number * result = new Placeholder(numbers, operators);
        return *result;
    }

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Variable::operator==(Number& rhs) {
	if (typeid(this) == typeid(rhs)) {
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
            
			if (rhsCast->getNumbers().capacity() == lhsCast->getNumbers().capacity()) {
				for (int i = 0; i < lhsCast->getNumbers().capacity(); i++) {
					bool matched = false;
                    
					for (int y = 0; y < rhsCast->getNumbers().capacity(); y++) {
						if (lhsCast[i] == rhsCast[y]) {
							matched = true;
						}
					}
                    
					if (!matched) {
						return false;
					}
				}
                
				for (int i = 0; i < lhsCast->getOperators().capacity(); i++) {
					bool matched = false;
                    
					for (int y = 0; y < rhsCast->getOperators().capacity(); y++) {
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