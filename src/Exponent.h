/*
 * Exponent.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef EXPONENT_H_
#define EXPONENT_H_

#include "Number.h"

using namespace std;

class Exponent: public Number {
public:
	Exponent();
	virtual ~Exponent();

	template<class T1, class T2>
	string operator+(T1 lhs, T2 rhs);
	template<class T1, class T2>
	string operator-(T1 lhs, T2 rhs);
	template<class T1, class T2>
	string operator*(T1 lhs, T2 rhs);
	template<class T1, class T2>
	string operator/(T1 lhs, T2 rhs);

	string toString();

private:
	int lcm(int denom1, int denom2);
};

#endif /* EXPONENT_H_ */
