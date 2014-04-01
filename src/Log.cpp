/*
 * Log.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: Brett
 */

#include "Log.h"

Log::Log(Number& base, Number& argument) {
	this->base = &base;
	this->argument = &argument;
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}

