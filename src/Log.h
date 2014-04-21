/*
 * Log.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef LOG_H_
#define LOG_H_

#include "Number.h"
#include <string>
#include <cmath>
#include "Exponent.h"
#include "Integer.h"
#include "Placeholder.h"
#include "Root.h"
#include "Variable.h"

class Log: public Number {
public:
	Log(Number& base, Number& argument);
	virtual ~Log();

	Number& operator+(Number& rhs);
	Number& operator-(Number& rhs);
	Number& operator*(Number& rhs);
	Number& operator/(Number& rhs);
	bool operator==(Number& rhs);

	std::string toString();
	Number& simplify();
	Number& clone();

	Number& getBase();
	Number& getArgument();

private:
	Number * base;
	Number * argument;
};

#endif /* LOG_H_ */
