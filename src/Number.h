/*
 * Number.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef NUMBER_H_
#define NUMBER_H_

#include <vector>

using namespace std;

class Number {
	public:
		template<class T1, class T2>
		virtual string operator+(T1 lhs, T2 rhs) = 0;
		template<class T1, class T2>
		virtual string operator-(T1 lhs, T2 rhs) = 0;
		template<class T1, class T2>
		virtual string operator*(T1 lhs, T2 rhs) = 0;
		template<class T1, class T2>
		virtual string operator/(T1 lhs, T2 rhs) = 0;

		virtual string toString() = 0;

	private:
		int lcm(int denom1, int denom2);
};


#endif /* NUMBER_H_ */
