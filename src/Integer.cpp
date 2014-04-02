/*
 * Integer.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: Brett
 */

#include "Integer.h"
#include <typeinfo>
#include <sstream>
#include <vector>

Integer::Integer(int toContain) {
	this->intContainer = toContain;
}

Integer::~Integer() {
	// TODO Auto-generated destructor stub
}

std::string Integer::operator+(Number& rhs) {
	std::stringstream ss;

	if (Integer * rhsCast = dynamic_cast<Integer*>(&rhs)) {
		int answer = this->intContainer + rhsCast->intContainer;
		delete rhsCast;

		ss << answer;
		return ss.str();
	} else {
		delete rhsCast;
		return this->toString() + "+" + rhs.toString();
	}
}

std::string Integer::operator-(Number& rhs) {
	std::stringstream ss;

	if (Integer * intCast = dynamic_cast<Integer*>(&rhs)) {
		int answer = this->intContainer - intCast->intContainer;
		delete intCast;

		ss << answer;
		return ss.str();
	} else {
		delete intCast;
		return this->toString() + "-" + rhs.toString();
	}
}

std::string Integer::operator*(Number& rhs) {
	std::stringstream ss;

	if (Integer * rhsCast = dynamic_cast<Integer*>(&rhs)) {
		int answer = this->intContainer * rhsCast->intContainer;
		delete rhsCast;

		ss << answer;
		return ss.str();
	} else {
		delete rhsCast;
		return this->toString() + "*" + rhs.toString();
	}
}

std::string Integer::operator/(Number& rhs) {
	std::stringstream ss;

	if (Integer * rhsCast = dynamic_cast<Integer*>(&rhs)) {
		if (this->intContainer % rhsCast->intContainer == 0) {
			int answer = this->intContainer / rhsCast->intContainer;
			delete rhsCast;

			ss << answer;
		} else {
			int gcdResult = gcd(this->intContainer, rhsCast->intContainer);
			int numerator = this->intContainer / gcdResult;
			int denominator = rhsCast->intContainer / gcdResult;
			ss << numerator << "/" << denominator;
		}
		return ss.str();
	} else {
		return this->toString() + "/" + rhs.toString();
	}
}

std::string Integer::toString() {
	std::stringstream ss;
	ss << this->intContainer;

	return ss.str();
}

std::vector<std::pair<Number*, char> > Integer::simplify() {
	std::vector<std::pair<Number*, char> > vectorPair;
	Number * num = this;
	vectorPair.push_back(std::make_pair<Number*, char>(num, NULL));

	return vectorPair;
}

int Integer::getInt() {
	return intContainer;
}
