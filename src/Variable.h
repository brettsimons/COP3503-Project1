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

class Variable: public Number {
public:
	Variable(std::string variable);
	virtual ~Variable();

	std::string operator+(Number& rhs);
	std::string operator-(Number& rhs);
	std::string operator*(Number& rhs);
	std::string operator/(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	std::string getVariable();

private:
	std::string var;
};

#endif /* VARIABLE_H_ */
