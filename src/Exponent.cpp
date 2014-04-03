/*
 * Exponent.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Exponent.h"

Exponent::Exponent(Number& base, Number& exponent) {
	this->base = &base;
	this->exponent = &exponent;
}

Exponent::~Exponent() {
	// TODO Auto-generated destructor stub
}

Number& Exponent::getBase() {
	return *this->base;
}

Number& Exponent::getExponent() {
	return *this->exponent;
}

std::string Exponent::operator+(Number& rhs) {

}

std::string Exponent::operator-(Number& rhs) {

}

std::string Exponent::operator*(Number& rhs) {

}

std::string Exponent::operator/(Number& rhs) {

}

std::string Exponent::toString() {

}

std::vector<std::pair<Number*, char> > Exponent::simplify() {

}
