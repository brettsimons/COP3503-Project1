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

	if (typeid(*this->exponent) == typeid(Placeholder) && (typeid(*this->base) == typeid(Placeholder) || typeid(*this->base) == typeid(Integer))) {
		Placeholder * cast = dynamic_cast<Placeholder*>(this->exponent);
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

Exponent::~Exponent() {
	delete base;
	delete exponent;
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
            numbers->push_back(&this->clone());
            numbers->push_back(integer);
            operators->push_back('+');
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

Number& Exponent::operator-(Number& rhs) {
	if (Exponent * rhsCast = dynamic_cast<Exponent*>(&rhs)) {
		if ((*rhsCast->base == *this->base) && (*rhsCast->exponent == *this->exponent)) {
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

Number& Exponent::operator*(Number& rhs) {
	if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);

		if (rhsCastExp->getBase() == *this->base){
			Number * newExp = &(this->exponent->clone() + rhsCastExp->getExponent().clone());
			Number * expon = new Exponent(base->clone(), *newExp);
			return expon->simplify();
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
		else if (typeid(rhs) == typeid(Root)) {
			if (typeid(*this->exponent) == typeid(Placeholder)) {
				Root * rhsCast = dynamic_cast<Root*>(&rhs);
				Placeholder * expCast = dynamic_cast<Placeholder*>(this->exponent);

				if (expCast->getOperators().at(0) == '/' && typeid(expCast->getNumbers().at(1)) == typeid(rhsCast->getRoot())) {

				}
			}
		}
		else if (typeid(rhs) == typeid(this->base)) {
			Number * result = &(this->exponent->clone() + *(new Integer(1)));
			delete this->exponent;
			this->exponent = result;
			return this->clone();
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

Number& Exponent::operator/(Number& rhs) {
	if (typeid(rhs) == typeid(Exponent)) {
		Exponent * rhsCastExp = dynamic_cast<Exponent*>(&rhs);

		if (rhsCastExp->getBase() == *this->base){
			if (typeid(*this->exponent) != typeid(rhsCastExp->getExponent())){
				std::vector<Number*> * numbers = new std::vector<Number*>();
				std::vector<char> * operators = new std::vector<char>();
				numbers->push_back(&this->exponent->clone());
				numbers->push_back(&rhsCastExp->getExponent().clone());
				operators->push_back('-');
				Number * placeholder = new Placeholder(*numbers, *operators);
				Number * expon = new Exponent(base->clone(), *placeholder);
				return expon->simplify();
			}
			else {
				Number * times = &(this->exponent->clone() - rhsCastExp->getExponent().clone());
				Number * expon = new Exponent(base->clone(), *times);
				return expon->simplify();
			}
		}
		Number * times = &(this->exponent->clone() - rhsCastExp->getExponent().clone());
		Number * expon = new Exponent(base->clone(), *times);
		return expon->simplify();
	}
	else if (typeid(rhs) == typeid(Root)) {
		Root * rhsCast = dynamic_cast<Root*>(&rhs);
		Exponent * denominator = new Exponent(rhsCast->clone(), rhsCast->getRoot().clone());
		Number * numerator = &(rhs.clone() * this->clone());

		Number* result = &(*numerator / denominator->simplify());
		delete denominator;
		delete numerator;
		return *result;
	}
	else if (typeid(rhs) == typeid(this->base)) {
		Number * result = &(this->exponent->clone() - *(new Integer(1)));
		delete this->exponent;
		this->exponent = result;
		return this->clone();
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

std::string Exponent::toString() {
	if (typeid(*this->exponent) == typeid(Placeholder) && typeid(*this->base) != typeid(Placeholder)) {
		return this->base->toString() + "^(" + this->exponent->toString() + ")";
	}
	else if (typeid(*this->exponent) != typeid(Placeholder) && typeid(*this->base) == typeid(Placeholder)) {
		return "(" + this->base->toString() + ")^" + this->exponent->toString();
	}
	else if (typeid(*this->exponent) == typeid(Placeholder) && typeid(*this->base) == typeid(Placeholder)) {
		return "(" + this->base->toString() + ")^(" + this->exponent->toString() + ")";
	}
	else if (typeid(*this->base) == typeid(Integer)) {
		Integer * integer = dynamic_cast<Integer*>(this->base);
		if (integer->getInt() < 0) {
			return "(" + this->base->toString() + ")^" + this->exponent->toString();
		}
	}
	
	return this->base->toString() + "^" + this->exponent->toString();
}

Number& Exponent::simplify() {
	if (typeid(*exponent) == typeid(Placeholder)) {
		Placeholder * expCast = dynamic_cast<Placeholder*>(exponent);
		bool isNeg = false;
		for (int i = 0; i < expCast->getNumbers().size(); i++) {
			if (typeid(*expCast->getNumbers()[i]) == typeid(Integer)) {
				Integer * tempIntObj = dynamic_cast<Integer*>(expCast->getNumbers()[i]);
				if (tempIntObj->getInt() < 0) {
					isNeg = true;
					int tempInt = tempIntObj->getInt();
					delete expCast->getNumbers()[i];
					expCast->getNumbers()[i] = new Integer(tempInt * -1);
				}
			}
		}

		if (isNeg) {
			Integer * one = new Integer(1);

			Placeholder * toReturn = new Placeholder();
			toReturn->getNumbers().push_back(one);
			toReturn->getNumbers().push_back(&this->simplify());
			toReturn->getOperators().push_back('/');
			return *toReturn;
		}
		else {
			Placeholder * temp = new Placeholder();
			Placeholder * result = new Placeholder();

			for (int i = 0; i < expCast->getOperators().size(); i++) {
				if (expCast->getOperators().at(i) == '+') {
					if (temp->getNumbers().size() > 0) {
						if (typeid(*expCast->getNumbers().at(i)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								Exponent * exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								Exponent * exp1 = new Exponent(this->base->clone(), *temp);
								temp = new Placeholder();
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
						}
						else {
							Exponent * exp1 = new Exponent(this->base->clone(), *temp);
							temp = new Placeholder();
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('*');
						}
					}
					else {
						if (typeid(*expCast->getNumbers().at(i)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								Exponent * exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								Exponent * exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i)->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
						}
						else {
							Exponent * exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i)->clone());
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('*');
						}
					}
				}
				else if (expCast->getOperators().at(i) == '-') {
					if (temp->getNumbers().size() > 0) {
						if (typeid(*expCast->getNumbers().at(i)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								Exponent * exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								Exponent * exp1 = new Exponent(this->base->clone(), *temp);
								temp = new Placeholder();
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('/');
							}
						}
						else {
							Exponent * exp1 = new Exponent(this->base->clone(), *temp);
							temp = new Placeholder();
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('/');
						}
					}
					else {
						if (typeid(*expCast->getNumbers().at(i)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								Exponent * exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								Exponent * exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i)->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('/');
							}
						}
						else {
							Exponent * exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i)->clone());
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('/');
						}
					}
				}
				else if (i < expCast->getOperators().size()) {
					if (expCast->getOperators().size() > 0 && expCast->getOperators()[i] == '/') {
						Root * innerExponent = new Root(this->base->clone(), expCast->getNumbers()[i + 1]->clone());
						Exponent * exp1 = new Exponent(innerExponent->simplify(), expCast->getNumbers()[i]->clone());
						if (expCast->getOperators().at(i + 1) == '*' || expCast->getOperators().at(i + 1) == '/') {
							temp->getNumbers().push_back(&expCast->getNumbers().at(i)->clone());
							temp->getOperators().push_back(expCast->getOperators().at(i + 1));
						}
						else if (expCast->getOperators().at(i + 1) == '+') {
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('*');
						}
						else if (expCast->getOperators().at(i + 1) == '-') {
							result->getNumbers().push_back(&exp1->simplify());
							result->getOperators().push_back('/');
						}
					}
					else {
						temp->getNumbers().push_back(&expCast->getNumbers().at(i)->clone());
						temp->getOperators().push_back(expCast->getOperators().at(i));
					}
				}

				if (i + 1 == expCast->getOperators().size()) {
					Exponent * exp1 = NULL;

					if (temp->getNumbers().size() > 0) {
						if (typeid(*expCast->getNumbers().at(i + 1)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i + 1));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								temp->getNumbers().push_back(&expCast->getNumbers().at(i + 1)->clone());

								exp1 = new Exponent(this->base->clone(), *temp);

								result->getNumbers().push_back(&exp1->simplify());
							}
						}
						else {
							temp->getNumbers().push_back(&expCast->getNumbers().at(i + 1)->clone());

							exp1 = new Exponent(this->base->clone(), *temp);

							result->getNumbers().push_back(&exp1->simplify());
						}
					}
					else {
						if (typeid(*expCast->getNumbers().at(i + 1)) == typeid(Placeholder)) {
							Placeholder * holder = dynamic_cast<Placeholder*>(expCast->getNumbers().at(i + 1));
							if (holder->getOperators().size() > 0 && holder->getOperators()[holder->getOperators().size() - 1] == '/') {
								Root * innerExponent = new Root(this->base->clone(), holder->getNumbers()[holder->getNumbers().size() - 1]->clone());
								exp1 = new Exponent(innerExponent->simplify(), holder->getNumbers()[holder->getNumbers().size() - 2]->clone());
								result->getNumbers().push_back(&exp1->simplify());
								result->getOperators().push_back('*');
							}
							else {
								exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i + 1)->clone());

								result->getNumbers().push_back(&exp1->simplify());
							}
						}
						else {
							exp1 = new Exponent(this->base->clone(), expCast->getNumbers().at(i + 1)->clone());

							result->getNumbers().push_back(&exp1->simplify());
						}
					}
				}
			}

			if (result->getNumbers().size() > 0) {
				return *result;
			}
		}
	}
    else if ((typeid(*base) == typeid(Integer)) &&  (typeid(*exponent) == typeid(Integer))) {
    	Integer * baseCast = dynamic_cast<Integer*>(base);
    	Integer * exponentCast = dynamic_cast<Integer*>(exponent);

		if (exponentCast->getInt() >= 0) {
			int powr = pow(baseCast->getInt(), exponentCast->getInt());
			Number *num = new Integer(powr);
			return *num;
		}
		else {
			Number * newExp = new Integer(exponentCast->getInt() * -1);
			delete this->exponent;
			this->exponent = newExp;

			Integer * one = new Integer(1);

			Placeholder * toReturn = new Placeholder();
			toReturn->getNumbers().push_back(one);
			toReturn->getNumbers().push_back(&this->simplify());
			toReturn->getOperators().push_back('/');
			return *toReturn;
		}
	}
	else if ((typeid(*base) == typeid(Variable)) && (typeid(*exponent) == typeid(Integer))) {
		Integer * exponentCast = dynamic_cast<Integer*>(exponent);
		if (exponentCast->getInt() == 0) {
			return *(new Integer(1));
		}
		else if (exponentCast->getInt() == 1) {
			return base->clone();
		}
		else if (exponentCast->getInt() < 0) {
			this->exponent = new Integer(exponentCast->getInt() * -1);

			Integer * one = new Integer(1);

			Placeholder * toReturn = new Placeholder();
			toReturn->getNumbers().push_back(one);
			toReturn->getNumbers().push_back(&this->simplify());
			toReturn->getOperators().push_back('/');
			return *toReturn;
		}
	}
	else if (typeid(*base) == typeid(Root)) {
		Root * baseCast = dynamic_cast<Root*>(base);
		Integer * one = new Integer(1);
		Number * rootEquiv = &(*one / this->exponent->clone());

		if (baseCast->getRoot() == *rootEquiv) {
			delete one;
			delete rootEquiv;
			return baseCast->getBase().clone();
		}
		else if (typeid(*this->exponent) == typeid(Integer) && typeid(baseCast->getBase()) == typeid(Integer)) {
			Number * innerRoot = &((new Exponent(baseCast->getBase().clone(), this->exponent->clone()))->simplify());
			Integer * one = new Integer(1);
			delete exponent;
			this->exponent = one;
			Root * result = new Root(*innerRoot, baseCast->getRoot().clone());
			return result->simplify();
		}
	}
	else if (typeid(*base) == typeid(Placeholder) && typeid(*exponent) == typeid(Integer)) {
		Integer * expCast = dynamic_cast<Integer*>(exponent);
		Placeholder * result = new Placeholder();
		result->getNumbers().push_back(&base->clone());

		for (int i = 0; i < expCast->getInt() - 1; i++) {
			Number * replacement = &(result->getNumbers().at(0)->clone() * base->clone());
			delete result->getNumbers().at(0);
			result->getNumbers().at(0) = &replacement->simplify();
			delete replacement;
		}

		return *result;
	}
    return this->clone();
}

Number& Exponent::clone() {
	return *(new Exponent(this->base->clone(), this->exponent->clone()));
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
