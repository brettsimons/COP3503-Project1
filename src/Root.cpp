/*
 * Root.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Root.h"

Root::Root(Number& base, Number& root) {
	this->base = &base;
	this->root = &root;
}

Root::~Root() {
	// TODO Auto-generated destructor stub
}

std::string Root::operator+(Number& rhs) {

}

std::string Root::operator-(Number& rhs) {

}

std::string Root::operator*(Number& rhs) {

}

std::string Root::operator/(Number& rhs) {

}

std::string Root::toString() {

}

std::vector<std::pair<Number*, char> > Root::simplify() {

}

Number& Root::getBase() {

}

Number& Root::getRoot() {

}
