/*
 * Calculator.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Calculator.h"

using namespace std;

Calculator::Calculator() {
	operators = new vector<char>();
	numbers = new vector<Number*>();
}

Calculator::~Calculator() {
	// TODO Auto-generated destructor stub
}

string Calculator::UnpackExpression(string equation) {
	equation.erase(std::remove(equation.begin(), equation.end(), ' '), equation.end());
	string equationSegment = equation;

	for (int i = 0; i < equation.length(); i++) {
		if (equation[i] == ')') {
			for (int x = i; x > 0; x--) {
				if (equation[x] == '(') {
					Calculate(equationSegment.substr(x, i-x));
				}
				else {
					// TODO: invalid equation exception
				}
			}
		}
	}
}

string Calculator::Calculate(string equationSegment) {
	for (int i = 0; i < equationSegment.length(); i++) {
		string segmentPart = equationSegment;
		if (equationSegment[i] == '*') {
			int numType = CheckNumberType(segmentPart.substr(0, i - 1));
			if (numType == 0) {
				// TODO: Create integer type and add to numbers list.
				Number * integer = new Integer(atoi(segmentPart.substr(0, i - 1).c_str()));
				numbers->push_back(integer);
			} else if (numType == 1) {

			} else if (numType == 2) {
				// TODO: Create log type and add to numbers list.
			} else if (numType == 3) {
				// TODO: Create root type and add to numbers list.
			}

			operators->push_back('*');
		} else if (equationSegment[i] == '/') {
			int numType = CheckNumberType(segmentPart.substr(0, i - 1));
			if (numType == 0) {
				// TODO: Create integer type and add to numbers list.
			} else if (numType == 1) {
				// TODO: Create exponent type and add to numbers list.
			} else if (numType == 2) {
				// TODO: Create log type and add to numbers list.
			} else if (numType == 3) {
				// TODO: Create root type and add to numbers list.
			}

			operators->push_back('/');
		}
	}

	for (int i = 0; i < equationSegment.length(); i++) {
		string segmentPart = equationSegment;
		if (equationSegment[i] == '+') {
			int numType = CheckNumberType(segmentPart.substr(0, i - 1));
			if (numType == 0) {
				// TODO: Create integer type and add to numbers list.
			} else if (numType == 1) {
				// TODO: Create exponent type and add to numbers list.
			} else if (numType == 2) {
				// TODO: Create log type and add to numbers list.
			} else if (numType == 3) {
				// TODO: Create root type and add to numbers list.
			}

			operators->push_back('+');
		} else if (equationSegment[i] == '-') {
			int numType = CheckNumberType(segmentPart.substr(0, i - 1));
			if (numType == 0) {
				// TODO: Create integer type and add to numbers list.
			} else if (numType == 1) {
				// TODO: Create exponent type and add to numbers list.
			} else if (numType == 2) {
				// TODO: Create log type and add to numbers list.
			} else if (numType == 3) {
				// TODO: Create root type and add to numbers list.
			}

			operators->push_back('-');
		}
	}
}

int Calculator::CheckNumberType(string number) {

}

Number& Calculator::CreateObject(string object) {

}
