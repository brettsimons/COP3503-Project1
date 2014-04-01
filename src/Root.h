/*
 * Root.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef ROOT_H_
#define ROOT_H_

#include<string>

#include "Number.h"

class Root: public Number {
public:
	Root(Number& base, Number& root);
	virtual ~Root();

	std::string operator+(Number& rhs);
	std::string operator-(Number& rhs);
	std::string operator*(Number& rhs);
	std::string operator/(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	Number& getBase();
	Number& getRoot();

private:
	Number * base;
	Number * root;
};

#endif /* ROOT_H_ */
