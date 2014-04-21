/*
 * Placeholder.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: brettsimons
 */

#include "Placeholder.h"
#include <algorithm>

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
		std::vector<Placeholder*> * lhsPlaceholders = new std::vector<Placeholder*>();
		std::vector<int> * lhsPlaceholdersOrigIndex = new std::vector<int>();
		std::vector<Number*> * rhsNumberList = &rhsCast->getNumbers();
		std::vector<char> * rhsOperatorList = &rhsCast->getOperators();

		if (rhsOperatorList->size() == 1 && rhsOperatorList->at(0) == '/' && this->operators->size() == 1 && this->operators->at(0) == '/') {
			Number * toAdd1 = &(*this->numbers->at(0) * *rhsNumberList->at(1));
			Number * toAdd2 = &(*rhsNumberList->at(0) * *this->numbers->at(1));
			Number * numerator = &(*toAdd1 + *toAdd2);
			Number * denominator = &(*rhsNumberList->at(1) * *this->numbers->at(1));
			Number * addition = &(*numerator / *denominator);
			return *addition;
		}

		bool added = false;

		for (int i = 0; i <= this->operators->size(); i++) {
			if ((i == 0 && (this->operators->at(0) == '+' || this->operators->at(0) == '-')) || (i > 0 && i < this->operators->size() && (this->operators->at(i) == '+' || this->operators->at(i) == '-') && (this->operators->at(i - 1) == '+' || this->operators->at(i - 1) == '-'))) {
				if (typeid(*this->numbers->at(i)) == typeid(Placeholder)) {
					lhsPlaceholders->push_back(dynamic_cast<Placeholder*>(this->numbers->at(i)));
					lhsPlaceholdersOrigIndex->push_back(i);
				}
			}

			if (i + 1 == this->operators->size() && (this->operators->at(i) == '+' || this->operators->at(i) == '-')) {
				if (typeid(*this->numbers->at(i + 1)) == typeid(Placeholder)) {
					lhsPlaceholders->push_back(dynamic_cast<Placeholder*>(this->numbers->at(i + 1)));
					lhsPlaceholdersOrigIndex->push_back(i + 1);
					break;
				}
			}
		}

		for (int i = 0; i < rhsNumberList->size(); i++) {
			if ((i == 0 && (rhsOperatorList->at(0) == '*' || rhsOperatorList->at(0) == '/')) || (i != 0 && (rhsOperatorList->at(i - 1) == '*' || rhsOperatorList->at(i - 1) == '/')) || (i < rhsOperatorList->size() && (rhsOperatorList->at(i) == '*' || rhsOperatorList->at(i) == '/'))) {
				Placeholder * innerPH = new Placeholder();
				
				int tempX = -1;
				for (int x = i; x < rhsNumberList->size(); x++) {
					if ((x == 0 && (rhsOperatorList->at(0) == '*' || rhsOperatorList->at(0) == '/')) || (x < rhsOperatorList->size() && (rhsOperatorList->at(x) == '*' || rhsOperatorList->at(x) == '/'))) {
						innerPH->getNumbers().push_back(rhsNumberList->at(x));
						innerPH->getOperators().push_back(rhsOperatorList->at(x));

						if (x + 1 == rhsOperatorList->size()) {
							innerPH->getNumbers().push_back(rhsNumberList->at(x + 1));
						}
					}
					else if ((x == 0 && (rhsOperatorList->at(0) == '+' || rhsOperatorList->at(0) == '-')) || (x != 0 && (rhsOperatorList->at(x - 1) == '+' || rhsOperatorList->at(x - 1) == '-')) || (x < rhsOperatorList->size() && (rhsOperatorList->at(x) == '+' || rhsOperatorList->at(x) == '-'))) {
						tempX = x;
						x = rhsNumberList->size();
					}
				}

				if (lhsPlaceholders->size() > 0) {
					Number * toReturn = NULL;
					for (int y = 0; y < lhsPlaceholders->size(); y++) {
						if (canAddOrSubtract(lhsPlaceholders->at(y), innerPH)) {
							for (int x = 0; x < lhsPlaceholders->at(y)->getNumbers().size(); x++) {
								if (typeid(*lhsPlaceholders->at(y)->getNumbers().at(x)) == typeid(Integer)) {
									if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
										lhsPlaceholders->at(y)->getNumbers().at(x) = &(*lhsPlaceholders->at(y)->getNumbers().at(x) + *innerPH->getNumbers().at(0));
										this->numbers->at(lhsPlaceholdersOrigIndex->at(y)) = lhsPlaceholders->at(y);
										return *this;
									}
									else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
										lhsPlaceholders->at(y)->getNumbers().at(x) = &(*lhsPlaceholders->at(y)->getNumbers().at(x) + *innerPH->getNumbers().at(1));
										this->numbers->at(lhsPlaceholdersOrigIndex->at(y)) = lhsPlaceholders->at(y);
										return *this;
									}
								}
							}
						}

						if (tempX != -1) {
							i = tempX;
						}
					}

					delete innerPH;
				}
				else {
					if (canAddOrSubtract(innerPH)) {
						for (int x = 0; x < numbers->size(); x++) {
							if (typeid(*numbers->at(x)) == typeid(Integer)) {
								if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
									numbers->at(x) = &(*numbers->at(x) + *innerPH->getNumbers().at(0));
									added = true;
									break;
								}
								else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
									numbers->at(x) = &(*numbers->at(x) + *innerPH->getNumbers().at(1));
									added = true;
									break;
								}
							}
							else if (typeid(*numbers->at(x + 1)) == typeid(Integer)) {
								if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
									numbers->at(x + 1) = &(*numbers->at(x) + *innerPH->getNumbers().at(0));
									added = true;
									break;
								}
								else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
									numbers->at(x + 1) = &(*numbers->at(x) + *innerPH->getNumbers().at(1));
									added = true;
									break;
								}
							}
							else if (typeid(*numbers->at(x)) == typeid(*innerPH->getNumbers().at(0)) || typeid(*numbers->at(x)) == typeid(*innerPH->getNumbers().at(1))) {
								Integer * tempInt = new Integer(2);
								numbers->at(x) = &(*tempInt * *numbers->at(x));
								added = true;
								break;
							}
							else if (typeid(*numbers->at(x + 1)) == typeid(*innerPH->getNumbers().at(0)) || typeid(*numbers->at(x + 1)) == typeid(*innerPH->getNumbers().at(1))) {
								Integer * tempInt = new Integer(2);
								numbers->at(x + 1) = &(*tempInt * *numbers->at(x + 1));
								added = true;
								break;
							}
						}
					}
					else {
						delete innerPH;
					}

					if (tempX != -1) {
						i = tempX;
					}
				}
			}
			else {
				Number * addition = &(*this + *rhsNumberList->at(i));

				if (typeid(*addition) == typeid(Placeholder)) {
					Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(addition);
					this->numbers = &tempPlaceholder->getNumbers();
					this->operators = &tempPlaceholder->getOperators();
				}
				else {
					this->numbers->clear();
					this->operators->clear();
					this->numbers->push_back(addition);
				}

				added = true;
			}
		}

		if (!added) {
			this->numbers->push_back(&rhs);
			this->operators->push_back('+');
			return *this;
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
	} else if (typeid(rhs) == typeid(Integer)) {
		Integer * rhsCast = dynamic_cast<Integer*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (i == 0 && this->operators->size() > 0 && this->operators->at(0) == '/') {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * numerator = &(*this->numbers->at(i) + *toAdd);
				Number * result = &(*numerator / *this->numbers->at(i + 1));
				return *result;
			} else if (i != 0 && this->operators->size() > 0 && this->operators->at(i - 1) == '/') {
				Number * toAdd = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * numerator = &(*this->numbers->at(i) + *toAdd);
				Number * result = &(*numerator / *this->numbers->at(i + 1));
				return *result;
			} else if (typeid(*this->numbers->at(i)) == typeid(Exponent)) {
				Integer * lhsCast = dynamic_cast<Integer*>(this->numbers->at(i));
				delete lhsCast;

				this->numbers->assign(i, new Integer(lhsCast->getInt() + rhsCast->getInt()));
				return *this;
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
		std::vector<Placeholder*> * lhsPlaceholders = new std::vector<Placeholder*>();
		std::vector<int> * lhsPlaceholdersOrigIndex = new std::vector<int>();
		std::vector<Number*> * rhsNumberList = &rhsCast->getNumbers();
		std::vector<char> * rhsOperatorList = &rhsCast->getOperators();

		if (rhsOperatorList->size() == 1 && rhsOperatorList->at(0) == '/' && this->operators->size() == 1 && this->operators->at(0) == '/') {
			Number * toAdd1 = &(*this->numbers->at(0) * *rhsNumberList->at(1));
			Number * toAdd2 = &(*rhsNumberList->at(0) * *this->numbers->at(1));
			Number * numerator = &(*toAdd1 - *toAdd2);
			Number * denominator = &(*rhsNumberList->at(1) * *this->numbers->at(1));
			Number * subtraction = &(*numerator / *denominator);
			return *subtraction;
		}

		bool subtracted = false;

		for (int i = 0; i <= this->operators->size(); i++) {
			if ((i == 0 && (this->operators->at(0) == '+' || this->operators->at(0) == '-')) || (i > 0 && i < this->operators->size() && (this->operators->at(i) == '+' || this->operators->at(i) == '-') && (this->operators->at(i - 1) == '+' || this->operators->at(i - 1) == '-'))) {
				if (typeid(*this->numbers->at(i)) == typeid(Placeholder)) {
					lhsPlaceholders->push_back(dynamic_cast<Placeholder*>(this->numbers->at(i)));
					lhsPlaceholdersOrigIndex->push_back(i);
				}
			}

			if (i + 1 == this->operators->size() && (this->operators->at(i) == '+' || this->operators->at(i) == '-')) {
				if (typeid(*this->numbers->at(i + 1)) == typeid(Placeholder)) {
					lhsPlaceholders->push_back(dynamic_cast<Placeholder*>(this->numbers->at(i + 1)));
					lhsPlaceholdersOrigIndex->push_back(i + 1);
					break;
				}
			}
		}

		for (int i = 0; i < rhsNumberList->size(); i++) {
			if ((i == 0 && (rhsOperatorList->at(0) == '*' || rhsOperatorList->at(0) == '/')) || (i != 0 && (rhsOperatorList->at(i - 1) == '*' || rhsOperatorList->at(i - 1) == '/')) || (i < rhsOperatorList->size() && (rhsOperatorList->at(i) == '*' || rhsOperatorList->at(i) == '/'))) {
				Placeholder * innerPH = new Placeholder();

				int tempX = -1;
				for (int x = i; x < rhsNumberList->size(); x++) {
					if ((x == 0 && (rhsOperatorList->at(0) == '*' || rhsOperatorList->at(0) == '/')) || (x < rhsOperatorList->size() && (rhsOperatorList->at(x) == '*' || rhsOperatorList->at(x) == '/'))) {
						innerPH->getNumbers().push_back(rhsNumberList->at(x));
						innerPH->getOperators().push_back(rhsOperatorList->at(x));

						if (x + 1 == rhsOperatorList->size()) {
							innerPH->getNumbers().push_back(rhsNumberList->at(x + 1));
						}
					}
					else if ((x == 0 && (rhsOperatorList->at(0) == '+' || rhsOperatorList->at(0) == '-')) || (x != 0 && (rhsOperatorList->at(x - 1) == '+' || rhsOperatorList->at(x - 1) == '-')) || (x < rhsOperatorList->size() && (rhsOperatorList->at(x) == '+' || rhsOperatorList->at(x) == '-'))) {
						tempX = x;
						x = rhsNumberList->size();
					}
				}

				if (lhsPlaceholders->size() > 0) {
					Number * toReturn = NULL;
					for (int y = 0; y < lhsPlaceholders->size(); y++) {
						if (canAddOrSubtract(lhsPlaceholders->at(y), innerPH)) {
							for (int x = 0; x < lhsPlaceholders->at(y)->getNumbers().size(); x++) {
								if (typeid(*lhsPlaceholders->at(y)->getNumbers().at(x)) == typeid(Integer)) {
									if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
										lhsPlaceholders->at(y)->getNumbers().at(x) = &(*lhsPlaceholders->at(y)->getNumbers().at(x) - *innerPH->getNumbers().at(0));
										this->numbers->at(lhsPlaceholdersOrigIndex->at(y)) = lhsPlaceholders->at(y);
										return *this;
									}
									else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
										lhsPlaceholders->at(y)->getNumbers().at(x) = &(*lhsPlaceholders->at(y)->getNumbers().at(x) - *innerPH->getNumbers().at(1));
										this->numbers->at(lhsPlaceholdersOrigIndex->at(y)) = lhsPlaceholders->at(y);
										return *this;
									}
								}
							}
						}

						if (tempX != -1) {
							i = tempX;
						}
					}

					delete innerPH;
				}
				else {
					if (canAddOrSubtract(innerPH)) {
						for (int x = 0; x < numbers->size(); x++) {
							if (typeid(*numbers->at(x)) == typeid(Integer)) {
								if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
									numbers->at(x) = &(*numbers->at(x) - *innerPH->getNumbers().at(0));
									subtracted = true;
									break;
								}
								else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
									numbers->at(x) = &(*numbers->at(x) - *innerPH->getNumbers().at(1));
									subtracted = true;
									break;
								}
							}
							else if (typeid(*numbers->at(x + 1)) == typeid(Integer)) {
								if (typeid(*innerPH->getNumbers().at(0)) == typeid(Integer)) {
									numbers->at(x + 1) = &(*numbers->at(x) - *innerPH->getNumbers().at(0));
									subtracted = true;
									break;
								}
								else if (typeid(*innerPH->getNumbers().at(1)) == typeid(Integer)) {
									numbers->at(x + 1) = &(*numbers->at(x) - *innerPH->getNumbers().at(1));
									subtracted = true;
									break;
								}
							}
							else if (typeid(*numbers->at(x)) == typeid(*innerPH->getNumbers().at(0)) || typeid(*numbers->at(x)) == typeid(*innerPH->getNumbers().at(1))) {
								Integer * tempInt = new Integer(0);
								numbers->at(x) = tempInt;
								subtracted = true;
								break;
							}
							else if (typeid(*numbers->at(x + 1)) == typeid(*innerPH->getNumbers().at(0)) || typeid(*numbers->at(x + 1)) == typeid(*innerPH->getNumbers().at(1))) {
								Integer * tempInt = new Integer(0);
								numbers->at(x + 1) = tempInt;
								subtracted = true;
								break;
							}
						}
					}
					else {
						delete innerPH;
					}

					if (tempX != -1) {
						i = tempX;
					}
				}
			}
			else {
				Number * subtraction = &(*this - *rhsNumberList->at(i));

				if (typeid(*subtraction) == typeid(Placeholder)) {
					Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(subtraction);
					this->numbers = &tempPlaceholder->getNumbers();
					this->operators = &tempPlaceholder->getOperators();
				}
				else {
					this->numbers->clear();
					this->operators->clear();
					this->numbers->push_back(subtraction);
				}

				subtracted = true;
			}
		}

		if (!subtracted) {
			this->numbers->push_back(&rhs);
			this->operators->push_back('-');
			return *this;
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
			if (i != 0 && (this->operators->at(i-1) == '/' || this->operators->at(i-1) == '*')) {

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
				return *this;
			}
		}

	} else if (typeid(rhs) == typeid(Variable)) {
		Variable * rhsCast = dynamic_cast<Variable*>(&rhs);

		for (int i = 0; i < this->numbers->size(); i++) {
			if (i != 0 && this->operators->at(i-1) == '/') {

			} else if (this->operators->size() > 0 && (i == 0 && this->operators->at(0) == '/') || (i != 0 && this->operators->at(i - 1) == '/')) {
				Number * toSubtract = &(*this->numbers->at(i + 1) * *rhsCast);
				Number * numerator = &(*this->numbers->at(i) + *toSubtract);
				Number * result = &(*numerator / *this->numbers->at(i + 1));
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

		bool isFOIL = false;
		
		for (int i = 0; i < rhsOperatorList->size(); i++) {
			if (rhsOperatorList->at(i) == '+' || rhsOperatorList->at(i) == '-') {
				isFOIL = true;
			}

			if (rhsOperatorList->at(i) == '-') {
				Integer * neg = new Integer(-1);
				rhsNumberList->at(i + 1) = &(*rhsNumberList->at(i + 1) * *neg);
				rhsOperatorList->at(i) = '+';
			}
		}

		Placeholder * foilResult = NULL;
		Placeholder * lhsReplacement = NULL;

		if (isFOIL) {
			foilResult = new Placeholder();

			for (int i = 0; i < operators->size(); i++) {
				if (operators->at(i) == '-') {
					Integer * neg = new Integer(-1);
					numbers->at(i + 1) = &(*numbers->at(i + 1) * *neg);
					operators->at(i) = '+';
				}
			}
		}

		for (int i = 0; i < rhsNumberList->size(); i++) {
			Number * multiplication = NULL;

			if (isFOIL) {
				lhsReplacement = dynamic_cast<Placeholder*>(&this->clone());
				multiplication = &(*lhsReplacement * *rhsNumberList->at(i));
			}
			else {
				multiplication = &(*this * *rhsNumberList->at(i));
			}

			if (typeid(*multiplication) == typeid(Placeholder)) {
				if (isFOIL && i > 0) {
					foilResult->getNumbers().push_back(multiplication);
					foilResult->getOperators().push_back(rhsOperatorList->at(i - 1));
				}
				else if (isFOIL && i == 0) {
					foilResult->getNumbers().push_back(multiplication);
				}
				else {
					Placeholder * tempPlaceholder = dynamic_cast<Placeholder*>(multiplication);
					this->numbers = &tempPlaceholder->getNumbers();
					this->operators = &tempPlaceholder->getOperators();
				}
			} else {
				this->numbers->clear();
				this->operators->clear();
				this->numbers->push_back(multiplication);
			}
		}

		if (this->numbers->size() == 1) {
			return *this->numbers->at(0);
		}
		else if (foilResult != NULL) {
			return *foilResult;
		}
		else {
			return *this;
		}
	} else {
		for (int i = 0; i < this->operators->size(); i++) {
			if (this->operators->at(i) == '/') {
				this->numbers->at(i) = &(*this->numbers->at(i) * rhs);
			} else if (this->operators->at(i) == '+' || this->operators->at(i) == '-') {
				this->numbers->at(i) = &(*this->numbers->at(i) * rhs);
				this->numbers->at(i + 1) = &(*this->numbers->at(i + 1) * rhs);
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

					for (int x = 0; x <= numberOfMoreMult; x++) {
						if (typeid(*this->numbers->at(x)) == typeid(rhs)) {
							typeMatchFound = true;
							this->numbers->at(x) = &(*this->numbers->at(x) * rhs);
						}
					}

					if (!typeMatchFound) {
						for (int x = 0; x <= numberOfMoreMult; x++) {
							if ((typeid(*this->numbers->at(x)) == typeid(Root) && typeid(rhs) == typeid(Exponent)) || (typeid(*this->numbers->at(x)) == typeid(Exponent) && typeid(rhs) == typeid(Root))) {
								this->numbers->at(x) == &(*this->numbers->at(x) * rhs);
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
	if (typeid(rhs) == typeid(Integer)) {
		Integer * integer = dynamic_cast<Integer*>(&rhs);

		if (integer->getInt() == 1) {
			return *this;
		} else if (integer->getInt() == 0) {
			// TODO: throw error.
		}

		if (this->numbers->size() == 2 && this->operators->at(0) == '/' && typeid(*this->numbers->at(0)) == typeid(Integer) && typeid(*this->numbers->at(1)) == typeid(Integer)) {
			Number * result = &(*this->numbers->at(0) / *integer);
			if (typeid(*result) == typeid(Integer)) {
				return (*result / *this->numbers->at(1));
			} else {
				Number * denominator = &(*this->numbers->at(1) * *integer);
				return (*this->numbers->at(0) / *denominator);
			}
		}

		std::vector<int> * intArray = new std::vector<int>();
		int indexOfDivision = -1;

		for (int i = 0; i < this->operators->size(); i++) {
			if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
				Integer * tempInt = dynamic_cast<Integer*>(this->numbers->at(i));
				intArray->push_back(tempInt->getInt());
			}

			if (i == this->operators->size() - 1) {
				intArray->push_back(integer->getInt());
			}

			if (this->operators->at(i) == '/') {
				//this->numbers->at(i + 1) = &(*this->numbers->at(i + 1) * rhs);
				indexOfDivision = i;

				for (int x = 0; x < i; x++) {
					if (typeid(*this->numbers->at(x)) == typeid(Integer)) {
						Integer * tempInt = dynamic_cast<Integer*>(this->numbers->at(x));
						intArray->push_back(tempInt->getInt());
					}
				}
			}
		}

		if (indexOfDivision >= 0) {
			Placeholder * numerator = new Placeholder();
			Placeholder * denominator = new Placeholder();

			for (int x = 0; x < indexOfDivision; x++) {
				if (x - 1 == indexOfDivision) {
					numerator->getNumbers().push_back(this->numbers->at(x + 1));
				} else {
					numerator->getNumbers().push_back(this->numbers->at(x));
					numerator->getOperators().push_back(this->operators->at(x));
				}
			}

			for (int x = indexOfDivision; x < this->operators->size(); x++) {
				if (x - 1 == this->operators->size()) {
					numerator->getNumbers().push_back(this->numbers->at(x + 1));
				} else {
					numerator->getNumbers().push_back(this->numbers->at(x));
					numerator->getOperators().push_back(this->operators->at(x));
				}
			}

			int gcdResult = gcd(*intArray);
			Integer * gcdInteger = new Integer(gcdResult);

			if (gcdResult > 1) {
				if (numerator->numbers->size() == 1 && typeid(numerator->numbers->at(0)) == typeid(Integer)) {
					numerator->numbers->at(0) = &(*numerator->numbers->at(0) / *gcdInteger);
				}

				Placeholder * toReturn = new Placeholder();
				toReturn->getNumbers().push_back(numerator);
				toReturn->getNumbers().push_back(denominator);
				toReturn->getOperators().push_back('/');
				return *toReturn;
			} else {
				return *this;
			}
		} else {
			Placeholder * result = new Placeholder();
			result->getNumbers().push_back(this);
			result->getNumbers().push_back(&rhs);
			result->getOperators().push_back('/');

			return *result;
		}
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

	return *this;
}

std::string Placeholder::toString() {
	std::string toReturn = "";
	bool openedParanthases = false;
	for (int i = 0; i < this->numbers->size(); i++) {
		if (typeid(*this->numbers->at(i)) == typeid(Integer)) {
			Integer * integer = dynamic_cast<Integer*>(this->numbers->at(i));
			if (integer->getInt() == 1 && this->operators->size() > 0) {
				if (i != 0 && this->operators->at(i - 1) == '*') {
					toReturn = toReturn.substr(0, toReturn.length() - 1);
					i++;
				}
				else if (i != 0 && this->operators->at(i - 1) == '/' && this->operators->at(i) != '*') {
					toReturn = toReturn.substr(0, toReturn.length() - 1);
					i++;
				}
				else if (i != 0 && this->operators->at(i - 1) == '/' && this->operators->at(i) == '*') {
					i++;
				}
			} else if (integer->getInt() == 0) {
				int lowExtrenum = -1;
				int highExtrenum = this->operators->size() - 1;
				for(int x = i; x < this->operators->size(); x++) {
					if (this->operators->at(x) == '*' || this->operators->at(x) == '/') {
						highExtrenum = x+2;
					}
				}

				i = highExtrenum;
			}
		}
		if (i == 0 && this->operators->size() > 0 && this->operators->at(0) != '\0') {
			toReturn += "(" + this->numbers->at(i)->toString() + this->operators->at(i);
			openedParanthases = true;
		}
		else if (i < this->operators->size()) {
			toReturn += this->numbers->at(i)->toString() + this->operators->at(i);
		}
		else if (openedParanthases && i < this->numbers->size()) {
			toReturn += this->numbers->at(i)->toString() + ")";
		}
		else if (!openedParanthases && i < this->numbers->size()) {
			toReturn += this->numbers->at(i)->toString();
		}
		else if (openedParanthases) {
			toReturn += ")";
		}
	}

	return toReturn;
}

Number& Placeholder::clone() {
	Placeholder * clone = new Placeholder();
	for (int i = 0; i < this->operators->size(); i++) {
		clone->getNumbers().push_back(&this->numbers->at(i)->clone());
		clone->getOperators().push_back(this->operators->at(i));

		if (i + 1 == this->operators->size()) {
			clone->getNumbers().push_back(&this->numbers->at(i + 1)->clone());
		}
	}

	return *clone;
}

Number& Placeholder::simplify() {
	if (this->numbers->size() == 1) {
		return this->numbers->at(0)->simplify();
	} else {
		for (int i = 0; i < operators->size(); i++) {
			if (operators->at(i) == '*') {
				Number * result = &(*numbers->at(i) * *numbers->at(i + 1));
				numbers->at(i) = result;
				numbers->erase(numbers->begin() + i + 1);
				operators->erase(operators->begin() + i);
				i--;
			} else if (operators->at(i) == '/') {
				Number * result = &(*numbers->at(i) / *numbers->at(i + 1));
				numbers->at(i) = result;
				numbers->erase(numbers->begin() + i + 1);
				operators->erase(operators->begin() + i);
				i--;
			}
		}

		for (int i = 0; i < operators->size(); i++) {
			if (operators->at(i) == '+') {
				if (typeid(*numbers->at(i)) != typeid(*numbers->at(i + 1))) {
					Number * result = NULL;
					int x = 0;
					for (x = i+1; x < numbers->size(); x++) {
						if (typeid(*numbers->at(i)) == typeid(*numbers->at(x)) && operators->at(x - 1) == '+') {
							result = &(*numbers->at(i) + *numbers->at(x));
						}
						else if (typeid(*numbers->at(i)) == typeid(*numbers->at(x)) && operators->at(x - 1) == '-') {
							result = &(*numbers->at(i) - *numbers->at(x));
						}
					}
					x--;
					if (result != NULL) {
						numbers->at(i) = result;
						numbers->erase(numbers->begin() + x);
						operators->erase(operators->begin() + x - 1);
					}
				} else {
					Number * result = &(*numbers->at(i) + *numbers->at(i + 1));
					numbers->at(i) = result;
					numbers->erase(numbers->begin() + i + 1);
					operators->erase(operators->begin() + i);
					i--;
				}
			} else if (operators->at(i) == '-') {
				if (typeid(*numbers->at(i)) != typeid(*numbers->at(i + 1))) {
					Number * result = NULL;
					int x = 0;
					for (x = i+1; x < numbers->size(); x++) {
						if (typeid(*numbers->at(i)) == typeid(*numbers->at(x))) {
							result = &(*numbers->at(i) - *numbers->at(x));
						}
					}
					x--;
					if (result != NULL) {
						numbers->at(i) = result;
						numbers->erase(numbers->begin() + x);
						operators->erase(operators->begin() + x - 1);
					}
				} else {
					Number * result = &(*numbers->at(i) - *numbers->at(i + 1));
					numbers->at(i) = result;
					numbers->erase(numbers->begin() + i + 1);
					operators->erase(operators->begin() + i);
					i--;
				}
			}
		}

		if (!operators->empty()) {
			Number * placeholder = new Placeholder(*this->numbers, *this->operators);
			return *placeholder;
		} else {
			return *numbers->at(0);
		}
	}
	return *this;
}

std::vector<Number*>& Placeholder::getNumbers() {
	return *this->numbers;
}

std::vector<char>& Placeholder:: getOperators() {
	return *this->operators;
}

bool Placeholder::canAddOrSubtract(Placeholder * rhs) {
	if (rhs->getNumbers().size() == numbers->size()) {
		for (int i = 0; i < numbers->size(); i++) {
			bool matched = false;

			for (int y = 0; y < rhs->getNumbers().size(); y++) {
				if (typeid(*numbers->at(i)) == typeid(Integer) && typeid(*rhs->getNumbers()[y]) == typeid(Integer)) {
					matched = true;
				} else if (*numbers->at(i) == *rhs->getNumbers()[y]) {
					matched = true;
				}
				else if (typeid(*numbers->at(i)) == typeid(Placeholder)) {
					Placeholder * temp = dynamic_cast<Placeholder*>(numbers->at(i));
					return canAddOrSubtract(temp, rhs);
				}
			}

			if (!matched) {
				return false;
			}
		}

		for (int i = 0; i < operators->size(); i++) {
			bool matched = false;

			for (int y = 0; y < rhs->getOperators().size(); y++) {
				if (operators->at(i) == rhs->getOperators()[y]) {
					if (this->getOperators()[i] == '-' || this->getOperators()[i] == '/') {
						if (this->getNumbers()[i] == rhs->getNumbers()[y] && this->getNumbers()[i + 1] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else {
							matched = false;
						}
					}
					else {
						if (this->getNumbers()[i] == rhs->getNumbers()[y] && this->getNumbers()[i + 1] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else if (this->getNumbers()[i + 1] == rhs->getNumbers()[y] && this->getNumbers()[i] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else if (typeid(*numbers->at(i)) == typeid(Integer) || typeid(*numbers->at(i + 1)) == typeid(Integer) || typeid(*rhs->getNumbers()[y]) == typeid(Integer) || typeid(*rhs->getNumbers()[y + 1]) == typeid(Integer)) {
							matched = true;
						}
						else {
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
	}
	else {
		return false;
	}
}

bool Placeholder::canAddOrSubtract(Placeholder * lhs, Placeholder * rhs) {
	if (rhs->getNumbers().size() == lhs->getNumbers().size()) {
		for (int i = 0; i < lhs->getNumbers().size(); i++) {
			bool matched = false;

			for (int y = 0; y < rhs->getNumbers().size(); y++) {
				if (typeid(lhs->getNumbers().at(i)) == typeid(Integer) && typeid(*rhs->getNumbers()[y]) == typeid(Integer)) {
					matched = true;
				} else if (*lhs->getNumbers().at(i) == *rhs->getNumbers()[y]) {
					matched = true;
				}
			}

			if (!matched) {
				return false;
			}
		}

		for (int i = 0; i < lhs->getOperators().size(); i++) {
			bool matched = false;

			for (int y = 0; y < rhs->getOperators().size(); y++) {
				if (lhs->getOperators().at(i) == rhs->getOperators()[y]) {
					if (lhs->getOperators()[i] == '-' || lhs->getOperators()[i] == '/') {
						if (lhs->getNumbers()[i] == rhs->getNumbers()[y] && lhs->getNumbers()[i + 1] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else {
							matched = false;
						}
					}
					else {
						if (lhs->getNumbers()[i] == rhs->getNumbers()[y] && lhs->getNumbers()[i + 1] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else if (lhs->getNumbers()[i + 1] == rhs->getNumbers()[y] && lhs->getNumbers()[i] == rhs->getNumbers()[y + 1]) {
							matched = true;
						}
						else if (typeid(lhs->getNumbers().at(i)) == typeid(Integer) || typeid(lhs->getNumbers().at(i + 1)) == typeid(Integer) || typeid(*rhs->getNumbers()[y]) == typeid(Integer) || typeid(*rhs->getNumbers()[y + 1]) == typeid(Integer)) {
							matched = true;
						}
						else {
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
	}
	else {
		return false;
	}
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
						if (lhsCast->getNumbers()[i] == rhsCast->getNumbers()[y]) {
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
