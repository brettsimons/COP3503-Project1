/*
 * Placeholder.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: brettsimons
 */

#include "Placeholder.h"

Placeholder::Placeholder() {
	this->numbers = new std::vector<Number*>();
	this->operators = new std::vector<char>();
}

Placeholder::Placeholder(std::vector<Number*>& numbers, std::vector<char>& operators) {
	this->numbers = &numbers;
	this->operators = &operators;
}

Placeholder::~Placeholder() {
	// TODO Auto-generated destructor stub
}

Number& Placeholder::operator+(Number& rhs) {
	if (typeid(rhs) == typeid(Placeholder)) {
		Placeholder * rhsCast = dynamic_cast<Placeholder*>(&rhs);
		std::vector<Number*> * rhsNumberList = &rhsCast->getNumbers();
		std::vector<char> * rhsOperatorList = &rhsCast->getOperators();

		for (int i = 0; i < rhsNumberList->size(); i++) {
			Number * addition = &(*this + *rhsNumberList->at(i));

			if (typeid(*addition) == typeid(Placeholder)) {
				Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(addition);
				this->numbers = &tempPlaceholder->getNumbers();
				this->operators = &tempPlaceholder->getOperators();
			} else {
				this->numbers->clear();
				this->operators->clear();
				this->numbers->push_back(addition);
			}
		}

		if (this->numbers->size() == 1) {
			return *this->numbers->at(0);
		} else {
			return *this;
		}

	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Exponent)) {
				Exponent * lhsCast = dynamic_cast<Exponent*>(this->numbers->at(i));

				if (this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('+');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getExponent() == rhsCast->getExponent()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(2));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('+');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Integer)) {
		Integer * rhsCast = dynamic_cast<Integer*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (i == 0 && this->operators->size() > 0 && this->operators->at(0) == '/') {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (i != 0 && this->operators->size() > 0 && this->operators->at(i - 1) == '/') {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Exponent)) {
				Integer * lhsCast = dynamic_cast<Integer*>(this->numbers->at(i));

				this->numbers->assign(i, new Integer(lhsCast->getInt() + rhsCast->getInt()));
				return *this;
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('+');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Log)) {
		Log * rhsCast = dynamic_cast<Log*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if ((i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Log)) {
				Log * lhsCast = dynamic_cast<Log*>(this->numbers->at(i));

				if (this->operators->size() > 0 && this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('+');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getArgument() == rhsCast->getArgument()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(2));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('+');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Root)) {
				Root * lhsCast = dynamic_cast<Root*>(this->numbers->at(i));

				if (this->operators->size() > 0 && this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('+');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getRoot() == rhsCast->getRoot()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(2));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('+');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCast = dynamic_cast<Variable*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if ((i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) + *toAdd);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Variable)) {
				Variable * lhsCast = dynamic_cast<Variable*>(this->numbers->at(i));

				if (this->operators->size() > 0 && this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('+');
					return *this;
				} else if (lhsCast->getVariable() == rhsCast->getVariable()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(2));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('+');
				return *this;
			}
		}

	}
}

Number& Placeholder::operator-(Number& rhs) {
	if (typeid(rhs) == typeid(Placeholder)) {
		Placeholder * rhsCast = dynamic_cast<Placeholder*>(&rhs);
		std::vector<Number*> * rhsNumberList = &rhsCast->getNumbers();
		std::vector<char> * rhsOperatorList = &rhsCast->getOperators();

		for (int i = 0; i < rhsNumberList->size(); i++) {
			Number * subtraction = &(*this - *rhsNumberList->at(i));

			if (typeid(*subtraction) == typeid(Placeholder)) {
				Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(subtraction);
				this->numbers = &tempPlaceholder->getNumbers();
				this->operators = &tempPlaceholder->getOperators();
			} else {
				this->numbers->clear();
				this->operators->clear();
				this->numbers->push_back(subtraction);
			}
		}

		if (this->numbers->size() == 1) {
			return *this->numbers->at(0);
		} else {
			return *this;
		}

	} else if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) - *toSubtract);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Exponent)) {
				Exponent * lhsCast = dynamic_cast<Exponent*>(this->numbers->at(i));

				if (this->operators->size() > 0 && this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('-');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getExponent() == rhsCast->getExponent()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(0));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('-');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Integer)) {
		Integer * rhsCast = dynamic_cast<Integer*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) - *toSubtract);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
				Integer * lhsCast = dynamic_cast<Integer*>(this->numbers->at(i));

				this->numbers->assign(i, new Integer(lhsCast->getInt() - rhsCast->getInt()));
				return *this;
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('-');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Log)) {
		Log * rhsCast = dynamic_cast<Log*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) - *toSubtract);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Log)) {
				Log * lhsCast = dynamic_cast<Log*>(this->numbers->at(i));

				if (this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('-');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getArgument() == rhsCast->getArgument()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(0));
					this->operators->insert(this->operators->begin() + i - 2, '*');

					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('-');
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (this->operators->size() > 0 && i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) - *toSubtract);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Root)) {
				Root * lhsCast = dynamic_cast<Root*>(this->numbers->at(i));

				if (this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('-');
					return *this;
				} else if (lhsCast->getBase() == rhsCast->getBase() && lhsCast->getRoot() == rhsCast->getRoot()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(0));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('-');
				delete rhsCast;
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCast = dynamic_cast<Variable*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * result = &(*this->numbers->at(i) - *toSubtract);
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Variable)) {
				Variable * lhsCast = dynamic_cast<Variable*>(this->numbers->at(i));

				if (this->operators->size() > 0 && this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '*')) {
					this->numbers->push_back(&rhs);
					this->operators->push_back('-');
					return *this;
				} else if (lhsCast->getVariable() == rhsCast->getVariable()){
					this->numbers->insert(this->numbers->begin() + i - 1, new Integer(0));
					this->operators->insert(this->operators->begin() + i - 2, '*');
					return *this;
				}
			} else if (i == this->numbers->size() - 1) {
				this->numbers->push_back(&rhs);
				this->operators->push_back('-');
				return *this;
			}
		}
	}
}

Number& Placeholder::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Placeholder)) {
		Placeholder * rhsCast = dynamic_cast<Placeholder*>(&rhs);
		std::vector<Number*> * rhsNumberList = &rhsCast->getNumbers();
		std::vector<char> * rhsOperatorList = &rhsCast->getOperators();

		for (int i = 0; i < rhsNumberList->size(); i++) {
			Number * multiplication = &(*this * *rhsNumberList->at(i));

			if (typeid(*multiplication) == typeid(Placeholder)) {
				Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(multiplication);
				this->numbers = &tempPlaceholder->getNumbers();
				this->operators = &tempPlaceholder->getOperators();
			} else {
				this->numbers->clear();
				this->operators->clear();
				this->numbers->push_back(multiplication);
			}
		}

		if (this->numbers->size() == 1) {
			return *this->numbers->at(0);
		} else {
			return *this;
		}
	} else {
		for (int i = 0; i < this->operators->size(); i++) {
			if (this->operators->at(i) == '/') {
				i++;
				this->numbers->at(i) = &(*this->numbers->at(i - 1) * rhs);
			} else if (this->operators->at(i) == '+' || this->operators->at(i) == '-') {
				this->numbers->at(i) = &(*this->numbers->at(i) * rhs);
			} else if (this->operators->at(i) == '*') {
				int numberOfMoreMult = 0;

				for (int x = i; x < this->operators->size(); x++) {
					if (this->operators->at(x) == '+' || this->operators->at(x) == '-' || this->operators->at(x) == '/') {
						x = this->operators->size();
					} else if (this->operators->at(x) == '*') {
						numberOfMoreMult++;
					}
				}

				if (numberOfMoreMult > 0) {
					bool typeMatchFound = false;

					for (int x = 0; x < numberOfMoreMult; x++) {
						if (typeid(*this->numbers->at(i)) == typeid(rhs)) {
							typeMatchFound = true;
							this->numbers->at(i) = &(*this->numbers->at(i) * rhs);
						}
					}

					if (!typeMatchFound) {
						for (int x = 0; x < numberOfMoreMult; x++) {
							if ((typeid(*this->numbers->at(i)) == typeid(Root) && typeid(rhs) == typeid(Exponent)) || (typeid(*this->numbers->at(i)) == typeid(Exponent) && typeid(rhs) == typeid(Root))) {
								this->numbers->at(i) == &(*this->numbers->at(i) * rhs);
								typeMatchFound = true;
							}
						}

						if (!typeMatchFound) {
							this->numbers->insert(this->numbers->begin() + i, 1, &rhs);
							this->operators->insert(this->operators->begin() + i, 1, '*');
						}
					}
				} else {
					this->numbers->at(i) = &(*this->numbers->at(i) * rhs);
				}
			}
		}

		return *this;
	}
}

Number& Placeholder::operator/(Number& rhs) {
	// TODO: sepcifically account for 1 / some fraction.
	if (typeid(rhs) == typeid(Integer)) {
		Integer * integer = dynamic_cast<Integer*>(&rhs);

		if (integer->getInt() == 1) {
			return *this;
		} else if (integer->getInt() == 0) {
			// TODO: throw error.
		}

		std::vector<int> intArray;
		bool hasInt = false;

		for (int i = 0; i < this->operators->size(); i++) {
			if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
				Integer * tempInt = dynamic_cast<Integer*>(this->numbers->at(i));
				intArray.push_back(tempInt->getInt());
			}

			if (i == this->operators->size() - 1) {
				intArray.push_back(integer->getInt());
			}

			if (this->operators->at(i) == '/') {
				this->numbers->at(i + 1) = &(*this->numbers->at(i + 1) * rhs);

				for (int x = 0; x < this->numbers->size(); x++) {
					if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
						Integer * tempInt = dynamic_cast<Integer*>(this->numbers->at(i));
						intArray.push_back(tempInt->getInt());
					}
				}
			}
		}

		int gcdResult = gcd(intArray);
		Integer * gcdInteger = new Integer(gcdResult);

		return (*this / *gcdInteger);
	} else if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		Exponent * denominator = new Exponent(*rhsCast, rhsCast->getRoot());
		Number * numerator = &(rhs * *this);

		return (*numerator / *denominator);
	} else if (typeid(rhs) == typeid(Placeholder)) {
		Integer * one = new Integer(1);
		Number * result = &(*one / rhs);
		return *result;
	}
}

std::string Placeholder::toString() {
	std::string toReturn = "";
	for (int i = 0; i < this->numbers->size(); i++) {
		if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
			Integer * integer = dynamic_cast<Integer*>(this->numbers->at(i));
			if (integer->getInt() == 1 && this->operators->size() > 0 && (this->operators->at(i) == '*' || (i != 0 && this->operators->at(i-1) == '/'))) {
				i++;
			} else if (integer->getInt() == 0) {
				int lowExtrenum = -1;
				int highExtrenum = this->operators->size() - 1;
				for(int x = i; x < this->operators->size(); x++) {
					if (this->operators->at(x) == '*' || this->operators->at(x) == '/') {
						highExtrenum = x+1;
					}
				}

				i = highExtrenum;
			}
		}
		if (i < this->operators->size()) {
			toReturn += this->numbers->at(i)->toString() + this->operators->at(i);
		} else {
			toReturn += this->numbers->at(i)->toString();
		}
	}

	return toReturn;
}

Number& Placeholder::simplify() {

}

std::vector<Number*>& Placeholder::getNumbers() {
	return *this->numbers;
}

std::vector<char>& Placeholder:: getOperators() {
	return *this->operators;
}

// **NOTICE**: In order to avoid circular referencing, this method must be copy and pasted as opposed to
// being inherited from the Number class.
bool Placeholder::operator==(Number& rhs) {
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
