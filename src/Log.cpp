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

	if (typeid(*this->argument) == typeid(Placeholder) && (typeid(*this->base) == typeid(Placeholder) || typeid(*this->base) == typeid(Integer))) {
		Placeholder * cast = dynamic_cast<Placeholder*>(this->argument);
		for (int i = 0; i < cast->getOperators().size(); i++) {
			if (typeid(cast->getNumbers().at(i + 1)) == typeid(Integer)) {
				Integer * intCast = dynamic_cast<Integer*>(cast->getNumbers().at(i + 1));

				if (typeid(*this->base) == typeid(Placeholder)) {
					Placeholder * baseCast = dynamic_cast<Placeholder*>(this->base);
					for (int x = 0; x < baseCast->getNumbers().size(); x++) {
						if (typeid(*baseCast->getNumbers().at(x)) == typeid(Integer)) {
							Integer * baseInt = dynamic_cast<Integer*>(baseCast->getNumbers().at(x));
							if (intCast->getInt() <= 0) {
								throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
							}
							else if (baseInt->getInt() <= 0) {
								throw std::out_of_range("The log base must be greater than 0. SOURCE: " + this->toString());
							}
						}
					}
				}
				else if (typeid(*this->base) == typeid(Integer)) {
					Integer * baseInt = dynamic_cast<Integer*>(this->base);
					if (intCast->getInt() <= 0) {
						throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
					}
					else if (baseInt->getInt() <= 0) {
						throw std::out_of_range("The log base must be greater than 0. SOURCE: " + this->toString());
					}
				}
			}
		}
	}
	else if (typeid(*this->argument) == typeid(Integer) && (typeid(*this->base) == typeid(Placeholder) || typeid(*this->base) == typeid(Integer))) {
		Integer * cast = dynamic_cast<Integer*>(this->argument);
		if (typeid(*this->base) == typeid(Placeholder)) {
			Placeholder * baseCast = dynamic_cast<Placeholder*>(this->base);
			for (int x = 0; x < baseCast->getNumbers().size(); x++) {
				if (typeid(*baseCast->getNumbers().at(x)) == typeid(Integer)) {
					Integer * baseInt = dynamic_cast<Integer*>(baseCast->getNumbers().at(x));
					if (cast->getInt() <= 0) {
						throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
					}
					else if (baseInt->getInt() <= 0) {
						throw std::out_of_range("The log base must be greater than 0. SOURCE: " + this->toString());
					}
				}
			}
		}
		else if (typeid(*this->base) == typeid(Integer)) {
			Integer * baseInt = dynamic_cast<Integer*>(this->base);
			if (cast->getInt() <= 0) {
				throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
			}
			else if (baseInt->getInt() <= 0) {
				throw std::out_of_range("The log base must be greater than 0. SOURCE: " + this->toString());
			}
		}
		else {
			if (cast->getInt() <= 0) {
				throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
			}
		}
	}
}

Log::~Log() {
	delete base;
	delete argument;
}

Number& Log::operator+(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
			Number * answer = &(rhsCast->getArgument().clone() * this->argument->clone());
            Log * log = new Log(base->clone(), *answer);
			Number * simplified = &log->simplify();
			delete log;
			return *simplified;
        }
	}

	if (typeid(rhs) == typeid(Placeholder)) {
		return rhs.clone() + this->clone();
	}

	else {
		std::vector<Number*> * numbers = new std::vector<Number*>();
		std::vector<char> * operators = new std::vector<char>();
		numbers->push_back(&this->clone());
		numbers->push_back(&rhs.clone());
		operators->push_back('+');
		Placeholder * placeholder = new Placeholder(*numbers, *operators);
		return *placeholder;
	}
}

Number& Log::operator-(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
            Number * answer = &(rhsCast->getArgument().clone() / this->argument->clone());
            Log * log = new Log(base->clone(), *answer);
			Number * simplified = &log->simplify();
			delete log;
            return *simplified;
        }

	}
	if (typeid(rhs) == typeid(Placeholder)) {
		Integer * negativeOne = new Integer(-1);
		Number * negativePlaceholder = &(rhs.clone() * *negativeOne);
		Number * result = &(*negativePlaceholder + this->clone());
		delete negativeOne;
		delete negativePlaceholder;
		return *result;
	}

	else {
		std::vector<Number*> * numbers = new std::vector<Number*>();
		std::vector<char> * operators = new std::vector<char>();
		numbers->push_back(&this->clone());
		numbers->push_back(&rhs.clone());
		operators->push_back('-');
		Placeholder * placeholder = new Placeholder(*numbers, *operators);
		return *placeholder;
	}
}

Number& Log::operator*(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if((rhsCast->getBase() == *this->base) && (rhsCast->getArgument() == *this->argument)) {
    		Integer * raisedTo = new Integer(2);
			Exponent * exp = new Exponent(this->clone(), *raisedTo);
			Number * simplified = &exp->simplify();
			delete exp;
			return *simplified;
        }
    }

    else if(typeid(rhs) == typeid(Exponent)) {
        Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);

        if(rhsCast->getBase() == *this) {
        		Integer * toAdd = new Integer(1);
				Number * raisedTo = &(rhsCast->getExponent().clone() + *toAdd);
                Exponent * exp = new Exponent(this->clone(), raisedTo->clone());
				Number * simplified = &exp->simplify();
				delete raisedTo;
				delete exp;
                return *simplified;
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
			return rhs.clone() * this->clone();
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('*');
			Placeholder * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Log::operator/(Number& rhs) {

    if (Log * rhsCast = dynamic_cast<Log*>(&rhs)) {
		if(rhsCast->getBase() == *this->base) {
			Log * log = new Log(this->getArgument().clone(), rhsCast->getArgument().clone());
			Number * simplified = &log->simplify();
			delete log;
			return *simplified;
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
	else {

		if (typeid(rhs) == typeid(Placeholder)) {
			Integer * reciprical = new Integer(1);
			return (*reciprical / (rhs.clone() / this->clone()));
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('/');
			Placeholder * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

std::string Log::toString() {
    return "log_" + this->base->toString() + ":(" + this->argument->toString() + ")";
}

Number& Log::simplify() {
	if (typeid(*argument) == typeid(Integer) && typeid(*base) == typeid(Integer)) {
		Integer * baseInt = dynamic_cast<Integer*>(base);
		Integer * argumentInt = dynamic_cast<Integer*>(argument);
		Integer * tempHolder = NULL;

		bool isReversed = false;

		if (argumentInt->getInt() <= 0) {
			throw std::out_of_range("The log argument must be greater than 0. SOURCE: " + this->toString());
		}
		else if (baseInt->getInt() <= 0) {
			throw std::out_of_range("The log base must be greater than 0. SOURCE: " + this->toString());
		}

		if (baseInt->getInt() > argumentInt->getInt()) {
			isReversed = true;
			tempHolder = new Integer(baseInt->getInt());
			baseInt = argumentInt;
			argumentInt = tempHolder;
		}

		if((int)(log10(argumentInt->getInt())/log10(baseInt->getInt())) == (log10(argumentInt->getInt())/log10(baseInt->getInt()))) {   //checks to see if the log operator reduces to an integer
			int ans = log10(argumentInt->getInt())/log10(baseInt->getInt());
    		Integer *answer = new Integer(ans);

			if (isReversed) {
				Integer * one = new Integer(1);
				Placeholder * ph = new Placeholder();
				ph->getNumbers().push_back(one);
				ph->getNumbers().push_back(answer);
				ph->getOperators().push_back('/');
				return *ph;
			}

    		return *answer;
		} else if (!isReversed) {
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

			int aRHS = argumentInt->getInt();
			int ans1 = NULL;

			if (tempMax != 0) {
				aRHS = argumentInt->getInt() / tempMax;
				ans1 = log10(tempMax) / log10(baseInt->getInt());
			}

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
			Number* ans1Int;

			if (powMax == -1 && logBase == -1) {
				ebase = new Integer(aRHS);
				ans1Int = new Integer(ans1);

				Log* secondHalf = new Log(base->clone(), *ebase);

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

				if (ans1 != NULL) {
					ans1Int = new Integer(ans1);
				}
				else {
					ans1Int = new Log(base->clone(), *(new Integer(argumentInt->getInt() / pow(logBase, powMax))));
				}

				Log* secondHalf = new Log(base->clone(), *ebase);

				std::vector<Number*> * numbs = new std::vector<Number*>();
				std::vector<char> * ops = new std::vector<char>();
				numbs->push_back(ans1Int);
				ops->push_back('+');
				numbs->push_back(powMaxNum);
				numbs->push_back(secondHalf);
				ops->push_back('*');
				Placeholder * placeholder2 = new Placeholder(*numbs, *ops);
				return *placeholder2;
			}
		}
		else {
			return this->clone();
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
				return argumentExp->getExponent().clone();
			}
		}
	}
	else if (typeid(*argument) == typeid(Root) && typeid(*base) == typeid(Root)) {
		Root * argumentCast = dynamic_cast<Root*>(argument);
		Root * baseCast = dynamic_cast<Root*>(base);

		if (typeid(argumentCast->getBase()) == typeid(Integer) && typeid(argumentCast->getRoot()) == typeid(Integer) && typeid(baseCast->getRoot()) == typeid(Integer) && typeid(baseCast->getBase()) == typeid(Integer)) {
			Integer * baseBaseInt = dynamic_cast<Integer*>(&baseCast->getBase());
			Integer * baseRootInt = dynamic_cast<Integer*>(&baseCast->getRoot());
			Integer * argBaseInt = dynamic_cast<Integer*>(&argumentCast->getBase());
			Integer * argRootInt = dynamic_cast<Integer*>(&argumentCast->getRoot());

			double argumentVal = pow(argBaseInt->getInt(), 1 / (double)argRootInt->getInt());
			double baseVal = pow(baseBaseInt->getInt(), 1 / (double)baseRootInt->getInt());

			bool foundAns = false;
			double result = 0;
			int i = 1;
			while (result < argumentVal) {
				result = pow(baseVal, i);
				if (result != argumentVal) {
					i++;
				}
				else {
					foundAns = true;
				}
			}

			if (foundAns) {
				return *(new Integer(i));
			}
		}
	}
	else if (typeid(*argument) == typeid(Integer) && typeid(*base) == typeid(Root)) {
		Integer * argumentCast = dynamic_cast<Integer*>(argument);
		Root * baseCast = dynamic_cast<Root*>(base);

		if (typeid(baseCast->getRoot()) == typeid(Integer) && typeid(baseCast->getBase()) == typeid(Integer)) {
			Integer * baseInt = dynamic_cast<Integer*>(&baseCast->getBase());
			Integer * rootInt = dynamic_cast<Integer*>(&baseCast->getRoot());

			double baseVal = pow(baseInt->getInt(), 1 / (double)rootInt->getInt());

			bool foundAns = false;
			int result = 0;
			int i = 1;
			while (result < argumentCast->getInt()) {
				result = pow(baseVal, i);
				if (result != argumentCast->getInt()) {
					i++;
				}
				else {
					foundAns = true;
				}
			}

			if (foundAns) {
				return *(new Integer(i));
			}
		}
	}
	else if (typeid(*argument) == typeid(Integer)) {
		Integer * argumentInt = dynamic_cast<Integer*>(argument);
		if (argumentInt->getInt() == 1) {
			return *(new Integer(0));
		}
	}
	return this->clone();
}

Number& Log::getBase() {
    return *this->base;
}

Number& Log::getArgument() {
    return *this->argument;
}

Number& Log::clone() {
	return *(new Log(this->base->clone(), this->argument->clone()));
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
