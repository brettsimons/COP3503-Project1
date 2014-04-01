/*
 * Log.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef LOG_H_
#define LOG_H_

#include "Number.h"
#include<string>

class Log: public Number {
public:
	Log(Number& base, Number& argument);
	virtual ~Log();

	std::string operator+(Number& rhs);
	std::string operator-(Number& rhs);
	std::string operator*(Number& rhs);
	std::string operator/(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	Number& getBase();
	Number& getArgument();

private:
	Number * base;
	Number * argument;
};

#endif /* LOG_H_ */
