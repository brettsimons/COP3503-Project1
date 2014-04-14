/*
 * Exponent.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef EXPONENT_H_
#define EXPONENT_H_

#include "Number.h"
#include <string>
#include <cmath>
#include "Placeholder.h"
#include "Integer.h"
#include "Log.h"
#include "Root.h"
#include "Variable.h"

class Exponent: public Number {
public:
	Exponent(Number& base, Number& exponent);
	virtual ~Exponent();

	Number& operator+(Number& rhs);
	Number& operator-(Number& rhs);
	Number& operator*(Number& rhs);
	Number& operator/(Number& rhs);
	bool operator==(Number& rhs);

	std::string toString();
	Number& simplify();

	Number& getBase();
	Number& getExponent();

private:
	Number * base;
	Number * exponent;
};

#endif /* EXPONENT_H_ */
