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
}

Root::~Root() {
	// TODO Auto-generated destructor stub
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
		std::vector<int> vec = primeFactors(baseInt->getInt());

		double invRoot = 1/((double)baseInt->getInt());

		if ((int)pow(rootInt->getInt(), invRoot) == pow(rootInt->getInt(), invRoot)) {
			return *(new Integer(pow(rootInt->getInt(), invRoot)));
		}

		int c2 = 0;
		int c3 = 0;
		int c5 = 0;
		int c7 = 0;

		int fin2 = 0;
		int fin3 = 0;
		int fin5 = 0;
		int fin7 = 0;

		for(int i = 0; i < vec.size(); i++) {
			if(vec[i] == 2) {
				c2++;
			}
			if(vec[i] == 3) {
				c3++;
			}
			if(vec[i] == 5) {
				c5++;
			}
			if(vec[i] == 7) {
				c7++;
			}
		}

		if(c2 > 1) {
			int n = c2/rootInt->getInt();
			fin2 = n * 2;
			if(fin2 == 0){
				fin2 = 1;
			}
		}

		else {
			fin2 = 1;
		}

		if(c3 > 1) {
			int n = c3/rootInt->getInt();
			fin3 = n * 3;
			if(fin3 == 0){
				fin3 = 1;
			}

		}

		else {
			fin3 = 1;
		}

		if(c5 > 1) {
			int n = c5/rootInt->getInt();
			fin5 = n * 5;
			if(fin5 == 0){
				fin5 = 1;
			}

		}

		else {
			fin5 = 1;
		}

		if(c7 > 1) {
			int n = c7/rootInt->getInt();
			fin7 = n * 7;
			if(fin7 == 0){
				fin7 = 1;
			}
		}

		else {
			fin7 = 1;
		}

		int maxFactor = fin2 * fin3 * fin5 * fin7;
		int baseFin = baseInt->getInt()/pow(maxFactor,rootInt->getInt());
		return *this;
	} else if (typeid(*root) == typeid(Placeholder)) {
		Placeholder * rootPlaceholder = dynamic_cast<Placeholder*>(root);
		if (typeid(rootPlaceholder->getNumbers().at(0)) == typeid(Integer)) {
			Integer * negRootInt = dynamic_cast<Integer*>(rootPlaceholder->getNumbers().at(0));
			Integer * rootInt = dynamic_cast<Integer*>(rootPlaceholder->getNumbers().at(1));
			if (negRootInt->getInt() == -1) {
				Placeholder * ph = new Placeholder();
				Integer * one = new Integer(1);

				ph->getNumbers().push_back(one);
				ph->getNumbers().push_back(rootInt);
				ph->getOperators().push_back('/');
				Exponent * exp = new Exponent(*base, *ph);
				return *exp;
			} else {
				return *this;
			}
		} else {
			return *this;
		}
	} else if (typeid(*base) == typeid(Placeholder)) {
		Placeholder * basePlaceholder = dynamic_cast<Placeholder*>(base);
		if (typeid(basePlaceholder->getNumbers().at(0)) == typeid(Integer)) {
			Integer * negBaseInt = dynamic_cast<Integer*>(basePlaceholder->getNumbers().at(0));
			Integer * baseInt = dynamic_cast<Integer*>(basePlaceholder->getNumbers().at(1));
			if (negBaseInt->getInt() == -1) {
				//TODO: throw exception
			}
		}
	} else {
		return *this;
	}
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
			std::vector<Number*> numbers;
			std::vector<char> operators;
			numbers.push_back(this);
			numbers.push_back(integer);
			operators.push_back('*');
			Number * result = new Placeholder(numbers, operators);
			return *result;
		}
	}
	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('+');
	Number * result = new Placeholder(numbers, operators);
	return *result;
}

Number& Root::operator-(Number& rhs) {
	if (Root * root = dynamic_cast<Root*>(&rhs)) {
		if ((root->getBase() == *this->base) && (root->getRoot() == *this->root)) {
			Number * integer = new Integer(0);
			return *integer;
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

Number& Root::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		if (rhsCast->getRoot() == *this->root) {
				Number * innards = &(rhsCast->getBase() * *this->base);
				Root * answer = new Root(*this, *innards);
				return *answer;
		}
	}
	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('*');
	Number * result = new Placeholder(numbers, operators);
	return *result;
}

Number& Root::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		if (rhsCast->getRoot() == *this->root) {
				Number * innards = &(*this->base / rhsCast->getBase());
				Root * answer = new Root(*this, *innards);
				return *answer;
		}
	}

	std::vector<Number*> numbers;
	std::vector<char> operators;
	numbers.push_back(this);
	numbers.push_back(&rhs);
	operators.push_back('/');
	Number * result = new Placeholder(numbers, operators);
	return *result;
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

