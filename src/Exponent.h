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

class Exponent: public Number {
public:
	Exponent(Number& base, Number& exponent);
	virtual ~Exponent();

	std::string operator+(Number& rhs);
	std::string operator-(Number& rhs);
	std::string operator*(Number& rhs);
	std::string operator/(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	Number& getBase();
	Number& getExponent();

private:
	Number * base;
	Number * exponent;
};

#endif /* EXPONENT_H_ */
