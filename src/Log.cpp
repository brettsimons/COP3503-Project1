//
//  Log.cpp
//  Log
//
//  Created by John Calderaio on 4/7/14.
//  Copyright (c) 2014 John Calderaio. All rights reserved.
//

#include "Log.h"

Log::Log(Number& base, Number& argument) {
	this->base = &base;
	this->argument = &argument;
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

Number& Log::operator+(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Number * answer = &(rhsCast->getArgument() * *this->argument);
            delete rhsCast;
            Number * log = new Log(*base, *answer);
            return * log;
        }
	} else {
		delete rhsCast;

		if (typeid(rhs) == typeid(Placeholder)) {
			return rhs + *this;
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('+');
			Number * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}
}

Number& Log::operator-(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Number * answer = &(rhsCast->getArgument() / *this->argument);
            delete rhsCast;
            Number * log = new Log(*base, *answer);
            return * log;
        }
	} else {
		delete rhsCast;

		if (typeid(rhs) == typeid(Placeholder)) {
			Number * negativeOne = new Integer(-1);
            Number * negativePlaceholder = &(rhs * *negativeOne);
            return *negativePlaceholder + *this;
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('-');
			Number * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}

}

Number& Log::operator*(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if((rhsCast->getBase() == *this->base) && (rhsCast->getArgument() == *this->argument)) {
            delete rhsCast;
    		Number * raisedTo = new Integer(2);
            Number * exp = new Exponent(*this, *raisedTo);
            return * exp;
        }
    }

    else if(typeid(rhs) == typeid(Exponent)) {
        Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);

        if(rhsCast->getBase() == *this) {
        		Number * toAdd = new Integer(1);
            	Number * raisedTo = &(rhsCast->getExponent() + *toAdd);
                Number * exp = new Exponent(*this, *raisedTo);
                return * exp;
            }
           else {
               std::vector<Number*> numbers;
               std::vector<char> operators;
               numbers.push_back(this);
               numbers.push_back(&rhs);
               operators.push_back('*');
               Number * placeholder = new Placeholder(numbers, operators);
               return *placeholder;
           }
        }

    else {
		delete rhsCast;

		if (typeid(rhs) == typeid(Placeholder)) {
			return rhs * *this;
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('*');
			Number * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}
}

Number& Log::operator/(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Number * log = new Log(this->getArgument(), rhsCast->getArgument());
            return * log;
        }
	} else {
		delete rhsCast;

		if (typeid(rhs) == typeid(Placeholder)) {
			Number * reciprical = new Integer(1);
			return (*reciprical / (rhs / *this));
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('/');
			Number * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}
}

std::string Log::toString() {
    return "log_" + this->base->toString() + ":(" + this->argument->toString() + ")";
}

std::vector<std::pair<Number*, char> > Log::simplify() {
    std::vector<std::pair<Number*, char> > vectorPair;
    Number * num = this;
    vectorPair.push_back(std::make_pair<Number*, char>(num, NULL));

    return vectorPair;
}

Number& Log::getBase() {
    return *this->base;
}

Number& Log::getArgument() {
    return *this->argument;
}

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Log::operator==(Number& rhs) {
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
