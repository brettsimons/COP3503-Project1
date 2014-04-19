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
            Log * log = new Log(*base, *answer);
            return * log;
        }
	}

	if (typeid(rhs) == typeid(Placeholder)) {
		return rhs + *this;
	}

	else {
		std::vector<Number*> numbers;
		std::vector<char> operators;
		numbers.push_back(this);
		numbers.push_back(&rhs);
		operators.push_back('+');
		Placeholder * placeholder = new Placeholder(numbers, operators);
		return *placeholder;
	}
}

Number& Log::operator-(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Number * answer = &(rhsCast->getArgument() / *this->argument);
            Log * log = new Log(*base, *answer);
            return * log;
        }

	}
	if (typeid(rhs) == typeid(Placeholder)) {
		Integer * negativeOne = new Integer(-1);
		Number * negativePlaceholder = &(rhs * *negativeOne);
		return *negativePlaceholder + *this;
	}

	else {
		std::vector<Number*> numbers;
		std::vector<char> operators;
		numbers.push_back(this);
		numbers.push_back(&rhs);
		operators.push_back('-');
		Placeholder * placeholder = new Placeholder(numbers, operators);
		return *placeholder;
	}
}

Number& Log::operator*(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if((rhsCast->getBase() == *this->base) && (rhsCast->getArgument() == *this->argument)) {
    		Integer * raisedTo = new Integer(2);
            Exponent * exp = new Exponent(*this, *raisedTo);
            return * exp;
        }
    }

    else if(typeid(rhs) == typeid(Exponent)) {
        Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);

        if(rhsCast->getBase() == *this) {
        		Integer * toAdd = new Integer(1);
            	Number * raisedTo = &(rhsCast->getExponent() + *toAdd);
                Exponent * exp = new Exponent(*this, *raisedTo);
                return * exp;
            }
           else {
               std::vector<Number*> numbers;
               std::vector<char> operators;
               numbers.push_back(this);
               numbers.push_back(&rhs);
               operators.push_back('*');
               Placeholder * placeholder = new Placeholder(numbers, operators);
               return *placeholder;
           }
        }

    else {

		if (typeid(rhs) == typeid(Placeholder)) {
			return rhs * *this;
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('*');
			Placeholder * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}
}

Number& Log::operator/(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Log * log = new Log(this->getArgument(), rhsCast->getArgument());
            return * log;
        }
	} else {

		if (typeid(rhs) == typeid(Placeholder)) {
			Integer * reciprical = new Integer(1);
			return (*reciprical / (rhs / *this));
		}

		else {
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(&rhs);
			operators.push_back('/');
			Placeholder * placeholder = new Placeholder(numbers, operators);
			return *placeholder;
		}
	}
}

std::string Log::toString() {
    return "log_" + this->base->toString() + ":(" + this->argument->toString() + ")";
}

Number& Log::simplify() {
	if (typeid(*argument) == typeid(Integer) && typeid(*base) == typeid(Integer)) {
		Integer * argumentInt = dynamic_cast<Integer*>(argument);
		Integer * baseInt = dynamic_cast<Integer*>(base);

		if((int)(log10(argumentInt->getInt())/log10(baseInt->getInt())) == (log10(argumentInt->getInt())/log10(baseInt->getInt()))) {   //checks to see if the log operator reduces to an integer
			int ans = log10(argumentInt->getInt())/log10(baseInt->getInt());
    		Integer *answer = new Integer(ans);
    		return *answer;
		} else {
			int n = 0;
			int tempMax = 0;
			for(int i = 1; i < argumentInt->getInt(); i++) {             //breaks down the log into different logs if possible
				n = pow(baseInt->getInt(), i);
				if((argumentInt->getInt() % n) == 0) {
					if(tempMax < n) {
						tempMax = n;
					}
				}
			}
			int aRHS = argumentInt->getInt() / tempMax;
			int ans1 = log10(tempMax)/log10(baseInt->getInt());

			int powMax = -1;
			int logBase = -1;
			for(int i = 2; i < aRHS; i++) {                   //breaks down the argument into exponents and puts in front of log
			   for(int j = 2; pow(i,j) <= aRHS; j++) {
				   if((aRHS % (int)pow(i,j)) == 0) {
					   powMax = j;
					   logBase = i;
				   }
			   }
			}

			Integer*  ebase;
			Integer* ans1Int;

			if (powMax == -1 && logBase == -1) {
				ebase = new Integer(aRHS);
				ans1Int = new Integer(ans1);

				Log* secondHalf = new Log(*base, *ebase);

				std::vector<Number*> * numbs = new std::vector<Number*>();
				std::vector<char> * ops = new std::vector<char>();
				numbs->push_back(ans1Int);
				ops->push_back('+');
				numbs->push_back(secondHalf);
				Placeholder * placeholder2 = new Placeholder(*numbs, *ops);
				return * placeholder2;
			}

			else {
				ebase = new Integer(logBase);

				Integer* powMaxNum = new Integer(powMax);
				ans1Int = new Integer(ans1);

				Log* secondHalf = new Log(*base, *ebase);

				std::vector<Number*> * numbs = new std::vector<Number*>();
				std::vector<char> * ops = new std::vector<char>();
				numbs->push_back(ans1Int);
				ops->push_back('+');
				numbs->push_back(powMaxNum);
				numbs->push_back(secondHalf);
				ops->push_back('*'); //If there is a way to just have, for example, 2log2 instead of 2*log2 then do DAT.
				Placeholder * placeholder2 = new Placeholder(*numbs, *ops);
				return * placeholder2;
			}
		}
	}
	else if (typeid(*argument) == typeid(Variable) && typeid(*base) == typeid(Variable)) {
		Variable * argumentVar = dynamic_cast<Variable*>(argument);
		Variable * baseVar = dynamic_cast<Variable*>(base);

		if (argumentVar->getVariable() == baseVar->getVariable()) {
			return *(new Integer(1));
		}
	}
	else if (typeid(*argument) == typeid(Exponent) && typeid(*base) == typeid(Variable)) {
		Exponent * argumentExp = dynamic_cast<Exponent*>(argument);
		Variable * baseVar = dynamic_cast<Variable*>(base);

		if (typeid(argumentExp->getBase()) == typeid(Variable)) {
			Variable * argumentVar = dynamic_cast<Variable*>(&argumentExp->getBase());

			if (argumentVar->getVariable() == baseVar->getVariable()) {
				return argumentExp->getExponent();
			}
		}
	}
	return *this;
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
