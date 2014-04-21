/*
 * Variable.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Brett
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include<string>
#include "Number.h"
#include "Exponent.h"
#include "Integer.h"
#include "Log.h"
#include "Root.h"
#include "Placeholder.h"

class Variable: public Number {
public:
	Variable(std::string variable);
	virtual ~Variable();

	Number& operator+(Number& rhs);
	Number& operator-(Number& rhs);
	Number& operator*(Number& rhs);
	Number& operator/(Number& rhs);
	bool operator==(Number& rhs);

	std::string toString();
	Number& simplify();
	Number& clone();

	std::string getVariable();

private:
	std::string var;
};

#endif /* VARIABLE_H_ */
