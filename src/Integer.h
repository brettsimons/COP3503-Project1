/*
 * Integer.h
 *
 *  Created on: Mar 21, 2014
 *      Author: Brett
 */

#ifndef INTEGER_H_
#define INTEGER_H_

#include<string>
#include<vector>
#include "Number.h"
#include "Exponent.h"

class Integer: public Number {
public:
	Integer(int toContain);
	virtual ~Integer();

	std::string operator+(Number& rhs);
	std::string operator-(Number& rhs);
	std::string operator*(Number& rhs);
	std::string operator/(Number& rhs);

	std::string toString();
	std::vector<std::pair<Number*, char> > simplify();

	int getInt();

private:
	int intContainer;
};

#endif /* INTEGER_H_ */
