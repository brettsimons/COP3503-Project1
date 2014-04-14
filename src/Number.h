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

		virtual Number& operator+(Number& rhs) = 0;
		virtual Number& operator-(Number& rhs) = 0;
		virtual Number& operator*(Number& rhs) = 0;
		virtual Number& operator/(Number& rhs) = 0;
		virtual bool operator==(Number& rhs) = 0;

		virtual Number& simplify() = 0;
		virtual std::string toString() = 0;

		template<class T1, class T2>
		std::string lcm(T1 denom1, T2 denom2);
		int gcd(int num1, int num2);
		int gcd(std::vector<int>& numList);
};


#endif /* NUMBER_H_ */
