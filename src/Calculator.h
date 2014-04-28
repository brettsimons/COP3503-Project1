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
#include <iostream>
#include <stdexcept>
#include "Number.h"
#include "Exponent.h"
#include "Log.h"
#include "Root.h"
#include "Variable.h"

class Calculator {
public:
	Calculator();
	virtual ~Calculator();
	std::string SimplifyExpression(std::string equationSegment);

private:
	Number& Calculate(std::string equation);
	Number& PerformCalculations();
	Number& PerformCalculations(Placeholder * toCalculate);
	int CheckNumberType(std::string number);
	std::vector<Number*> * numbers;
	std::vector<char> * operators;
	bool IsPlaceholder(std::string number);
};

#endif /* CALCULATOR_H_ */
