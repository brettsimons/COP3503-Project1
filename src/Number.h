/*
 * Number.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef NUMBER_H_
#define NUMBER_H_

#include<vector>
#include<string>

class Number {
	public:
		Number();
		virtual ~Number();

		virtual std::string operator+(Number& rhs) = 0;
		virtual std::string operator-(Number& rhs) = 0;
		virtual std::string operator*(Number& rhs) = 0;
		virtual std::string operator/(Number& rhs) = 0;

		virtual std::vector<std::pair<Number*, char> > simplify() = 0;
		virtual std::string toString() = 0;

		template<class T1, class T2>
		std::string lcm(T1 denom1, T2 denom2);
		int gcd(int num1, int num2);
};


#endif /* NUMBER_H_ */
