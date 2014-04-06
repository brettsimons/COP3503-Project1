/*
 * Calculator.h
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

#include <string>
#include <algorithm>
#include <vector>
#include "Number.h"
#include "Exponent.h"
#include "Log.h"
#include "Root.h"
#include "Variable.h"

class Calculator {
public:
	Calculator();
	virtual ~Calculator();

private:
	std::string Calculate(std::string equation);
	std::string UnpackExpression(std::string equationSegment);
	int CheckNumberType(std::string number);
	std::vector<Number*> * numbers;
	std::vector<char> * operators;
	Number& CreateObject(std::string object);
};

#endif /* CALCULATOR_H_ */
