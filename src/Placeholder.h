/*
 * Placeholder.h
 *
 *  Created on: Apr 5, 2014
 *      Author: brettsimons
 */

#ifndef PLACEHOLDER_H_
#define PLACEHOLDER_H_

#include <vector>
#include <typeinfo>
#include "Number.h"
#include "Exponent.h"
#include "Integer.h"
#include "Log.h"
#include "Root.h"
#include "Variable.h"

class Placeholder: public Number {
public:
	Placeholder();
	Placeholder(std::vector<Number*>& numbers, std::vector<char>& operators);
	virtual ~Placeholder();

	Number& operator+(Number& rhs);
	Number& operator-(Number& rhs);
	Number& operator*(Number& rhs);
	Number& operator/(Number& rhs);
	bool operator==(Number& rhs);
	std::string toString();
	Number& simplify();

	template <class T1>
	std::vector<T1> getInnerGCD(Placeholder& innerEquation);
	std::vector<Number*>& getNumbers();
	std::vector<char>& getOperators();
private:
	std::vector<Number*> * numbers;
	std::vector<char> * operators;

	bool canAddOrSubtract(Placeholder * rhs);
	bool canAddOrSubtract(Placeholder * lhs, Placeholder * rhs);
};

#endif /* PLACEHOLDER_H_ */
