/*
 * Calculator.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <string>
#include <list>
#include <algorithm>
#include "Number.h"

using namespace std;

class Calculator {
public:
	Calculator();
	virtual ~Calculator();

private:
	string Calculate(string equation);
	string UnpackExpression(string equationSegment);
	int CheckNumberType(string number);
	list<Number> numbers;
	list<char> operators;
};

#endif /* CALCULATOR_H_ */
