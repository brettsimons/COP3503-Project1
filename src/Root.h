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
#include "Exponent.h"
#include "Integer.h"
#include "Log.h"
#include "Placeholder.h"
#include "Variable.h"

class Root: public Number {
public:
	Root(Number& base, Number& root);
	virtual ~Root();

	Number& operator+(Number& rhs);
	Number& operator-(Number& rhs);
	Number& operator*(Number& rhs);
	Number& operator/(Number& rhs);
	bool operator==(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	Number& getBase();
	Number& getRoot();

private:
	Number * base;
	Number * root;
};

#endif /* ROOT_H_ */
