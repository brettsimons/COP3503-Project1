/*
 * Root.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Root.h"

Root::Root(Number& base, Number& root) {
	this->base = &base;
	this->root = &root;
	
	if (typeid(*this->root) == typeid(Placeholder) && (typeid(*this->base) == typeid(Placeholder) || typeid(*this->base) == typeid(Integer))) {
		Placeholder * cast = dynamic_cast<Placeholder*>(this->root);
		for (int i = 0; i < cast->getOperators().size(); i++) {
			if (cast->getOperators().at(i) == '/') {
				if (typeid(cast->getNumbers().at(i + 1)) == typeid(Integer)) {
					Integer * intCast = dynamic_cast<Integer*>(cast->getNumbers().at(i + 1));

					if (typeid(*this->base) == typeid(Placeholder)) {
						Placeholder * baseCast = dynamic_cast<Placeholder*>(this->base);
						for (int x = 0; x < baseCast->getNumbers().size(); x++) {
							if (typeid(*baseCast->getNumbers().at(x)) == typeid(Integer)) {
								Integer * baseInt = dynamic_cast<Integer*>(baseCast->getNumbers().at(x));
								if (intCast->getInt() % 2 == 0 && baseInt < 0) {
									throw std::out_of_range("A negative number cannot be applied to an even root. SOURCE: " + this->toString());
								}
							}
						}
					}
					else if (typeid(*this->base) == typeid(Integer)) {
						Integer * baseInt = dynamic_cast<Integer*>(this->base);
						if (intCast->getInt() % 2 == 0 && baseInt < 0) {
							throw std::out_of_range("A negative number cannot be applied to an even root. SOURCE: " + this->toString());
						}
					}
				}
			}
		}
	}
}

Root::~Root() {
	delete base;
	delete root;
}

Number& Root::getBase() {
	return *this->base;
}

Number& Root::getRoot() {
	return *this->root;
}

Number& Root::simplify(){
	if (typeid(*base) == typeid(Integer) && typeid(*root) == typeid(Integer)) {
		Integer * baseInt = dynamic_cast<Integer*>(base);
		Integer * rootInt = dynamic_cast<Integer*>(root);

		bool isRootNeg = false;
		bool isBaseNeg = false;

		if (rootInt->getInt() < 0) {
			isRootNeg = true;
			int newRoot = rootInt->getInt() * -1;
			delete rootInt;
			root = new Integer(newRoot);
			rootInt = new Integer(newRoot);
		}
		else if (rootInt->getInt() == 0) {
			throw std::out_of_range("The value of n for an n'th root object cannot be 0. SOURCE: " + this->toString());
		}

		if (baseInt->getInt() < 0 && rootInt->getInt() % 2 != 0) {
			isBaseNeg = true;
			int newBase = baseInt->getInt() * -1;
			delete baseInt;
			base = new Integer(newBase);
			baseInt = new Integer(newBase);
		}
		else if (baseInt->getInt() == 0) {
			return *(new Integer(0));
		}
		else if (baseInt->getInt() < 0 && rootInt->getInt() % 2 == 0) {
			throw std::out_of_range("A negative number cannot be applied to an even root. SOURCE: " + this->toString());
		}
		
		std::vector<int*> vec = primeFactors(baseInt->getInt());

		double invRoot = 1 / ((double)rootInt->getInt());

		if ((int)pow(baseInt->getInt(), invRoot) == pow(baseInt->getInt(), invRoot)) {
			if (isRootNeg) {
				Integer * one = new Integer(1);
				Placeholder * ph = new Placeholder();
				ph->getNumbers().push_back(one);
				if (isBaseNeg) {
					ph->getNumbers().push_back(new Integer(pow(baseInt->getInt(), invRoot) * -1));
				}
				else {
					ph->getNumbers().push_back(new Integer(pow(baseInt->getInt(), invRoot)));
				}
				ph->getOperators().push_back('/');
				return *ph;
			}
			if (isBaseNeg) {
				Number * toReturn = (new Integer(pow(baseInt->getInt(), invRoot) * -1));
				return *toReturn;
			}
			else {
				Number * toReturn = (new Integer(pow(baseInt->getInt(), invRoot)));
				return *toReturn;
			}
		}

		int multiplier = 1;
		int innerValue = 1;

		for (int i = 1; i < vec.size(); i++) {
			int exp = vec.at(i)[1];
			int multiplierExp = 0;
			for (int x = 0; x < vec.at(i)[1]; x++) {
				if (exp - rootInt->getInt() >= 0) {
					exp = exp - rootInt->getInt();
					multiplierExp++;
				} else {
					x = vec.at(i)[1];
				}
			}

			vec.at(i)[1] = exp;
			multiplier *= pow(vec.at(i)[0], multiplierExp);
			innerValue *= pow(vec.at(i)[0], vec.at(i)[1]);
		}

		if (multiplier > 1) {
			if (innerValue == 1) {
				Number * toReturn = new Integer(multiplier);
				return *toReturn;
			}

			if (isRootNeg) {
				Integer * one = NULL;

				if (isBaseNeg) {
					one = new Integer(1);
				}
				else {
					one = new Integer(-1);
				}

				Placeholder * simplified = new Placeholder();
				simplified->getNumbers().push_back(new Integer(multiplier));
				delete base;
				this->base = new Integer(innerValue);
				simplified->getNumbers().push_back(&this->clone());
				simplified->getOperators().push_back('*');

				Placeholder * ph = new Placeholder();
				ph->getNumbers().push_back(one);
				ph->getNumbers().push_back(simplified);
				ph->getOperators().push_back('/');
				return *ph;
			}

			Placeholder * simplified = new Placeholder();
			if (isBaseNeg) {
				simplified->getNumbers().push_back(new Integer(multiplier * -1));
			}
			else {
				simplified->getNumbers().push_back(new Integer(multiplier));
			}
			delete base;
			this->base = new Integer(innerValue);
			simplified->getNumbers().push_back(&this->clone());
			simplified->getOperators().push_back('*');
			return *simplified;
		}

		return this->clone();
	} else if (typeid(*root) == typeid(Placeholder)) {
		Placeholder * rootPlaceholder = dynamic_cast<Placeholder*>(root);
		if (typeid(rootPlaceholder->getNumbers().at(0)) == typeid(Integer)) {
			Integer * negRootInt = dynamic_cast<Integer*>(rootPlaceholder->getNumbers().at(0));
			Integer * rootInt = dynamic_cast<Integer*>(rootPlaceholder->getNumbers().at(1));
			if (negRootInt->getInt() == -1) {
				Placeholder * ph = new Placeholder();
				Integer * one = new Integer(1);

				ph->getNumbers().push_back(one);
				ph->getNumbers().push_back(&rootInt->clone());
				ph->getOperators().push_back('/');
				Exponent * exp = new Exponent(base->clone(), *ph);
				return *exp;
			} else {
				return this->clone();
			}
		} else {
			return this->clone();
		}
	} else if (typeid(*base) == typeid(Placeholder)) {
		Placeholder * basePlaceholder = dynamic_cast<Placeholder*>(base);
		if (typeid(basePlaceholder->getNumbers().at(0)) == typeid(Integer)) {
			Integer * negBaseInt = dynamic_cast<Integer*>(basePlaceholder->getNumbers().at(0));
			Integer * baseInt = dynamic_cast<Integer*>(basePlaceholder->getNumbers().at(1));
			if (typeid(*root) == typeid(Integer)) {
				Integer * rootInt = dynamic_cast<Integer*>(root);
				if (negBaseInt->getInt() < 0 && rootInt->getInt() % 2 == 0) {
					throw std::out_of_range("A negative number cannot be applied to an even root. SOURCE: " + this->toString());
				}
			}
		}
	} else {
		return this->clone();
	}
}

std::vector<int*> Root::primeFactors(int number) {
	std::vector<int*> * toReturn = new std::vector<int*>();
	int * intHolder = new int[2];
	intHolder[0] = 1;
	intHolder[1] = number;
	toReturn->push_back(intHolder);
	bool isPrime = true;
	for (int i = 2; i < number; i++) {
		if (number % i == 0) {
			for (int x = 2; x < i; x++) {
				if (i % x == 0) {
					isPrime = false;
				} else if (!isPrime) {
					x = i;
				}
			}

			if (isPrime) {
				int tempNumber = number;
				int multiplier = 0;
				for (int x = 0; x < number; x++) {
					if (tempNumber % i == 0) {
						tempNumber = tempNumber / i;
						multiplier++;
					} else {
						x = number;
					}
				}

				int * holder = new int[2];
				holder[0] = i;
				holder[1] = multiplier;

				toReturn->push_back(holder);
			}

			isPrime = true;
		}
	}

	return *toReturn;
}

std::string Root::toString() {
	if (typeid(*this->root) == typeid(Integer)) {
		Integer * intRoot = dynamic_cast<Integer*>(this->root);
		if (intRoot->getInt() == 2) {
			return "sqrt:(" + this->base->toString() + ")";
		}
	}

	return this->root->toString() + "rt:(" + this->base->toString() + ")";
}

Number& Root::operator+(Number& rhs) {
	if (Root * rhsCast = dynamic_cast<Root*>(&rhs)) {
		if ((rhsCast->getBase() == *this->base) && (rhsCast->getRoot() == *this->root)) {
			Number * integer = new Integer(2);
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('*');
			Number * result = new Placeholder(*numbers, *operators);
			return *result;
		}
	}
	else {
		if (typeid(rhs) == typeid(Placeholder)) {
			return rhs.clone() + this->clone();
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('+');
			Number * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Root::operator-(Number& rhs) {
	if (Root * root = dynamic_cast<Root*>(&rhs)) {
		if ((root->getBase() == *this->base) && (root->getRoot() == *this->root)) {
			Number * integer = new Integer(0);
			return *integer;
		}
	}
	else {
		if (typeid(rhs) == typeid(Placeholder)) {
			Integer * neg = new Integer(-1);
			return (*neg * rhs.clone()) + this->clone();
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('-');
			Number * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Root::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		if (rhsCast->getRoot() == *this->root) {
			if (rhsCast->getBase() == *this->base) {
				return this->base->clone();
			}

			Number * innards = &(rhsCast->getBase().clone() * this->base->clone());
			Root * answer = new Root(*innards, this->root->clone());
			Number * simplified = &answer->simplify();
			delete answer;
			return *simplified;
		}
		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('*');
			Number * placeholder = new Placeholder(*numbers, *operators);
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
			Number * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Root::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		if (rhsCast->getRoot() == *this->root) {
			Number * innards = &(this->base->clone() / rhsCast->getBase().clone());
			Root * answer = new Root(*innards, this->root->clone());
			Number * simplified = &answer->simplify();
			delete answer;
			return *simplified;
		}
		else {
			Exponent * denominator = new Exponent(rhsCast->clone(), rhsCast->getRoot().clone());
			Number * numerator = &(rhs.clone() * this->clone());
			Number * result = &(numerator->clone() / denominator->simplify());
			delete denominator;
			delete numerator;
			return *result;
		}
	}
	else {
		if (typeid(rhs) == typeid(Placeholder)) {
			Integer * one = new Integer(1);
			return *one / (rhs.clone() / this->clone());
		}

		else {
			std::vector<Number*> * numbers = new std::vector<Number*>();
			std::vector<char> * operators = new std::vector<char>();
			numbers->push_back(&this->clone());
			numbers->push_back(&rhs.clone());
			operators->push_back('/');
			Number * placeholder = new Placeholder(*numbers, *operators);
			return *placeholder;
		}
	}
}

Number& Root::clone() {
	return *(new Root(this->base->clone(), this->root->clone()));
}

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Root::operator==(Number& rhs) {
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

