/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: random_generator.cpp
---------------------------------------------------
*/

#include "random_generator.h"

//Dependencies
#include <cstdlib>

namespace lunar {

//Random generator
//Protected

random_generator::random_generator() {
	//Empty
}

random_generator::~random_generator() {
	//Empty
}

//Classic generator
//Protected

void classic_generator::copy(const classic_generator &_Classic_generator) {
	_Seed=_Classic_generator._Seed;
}

//Public

classic_generator::classic_generator(unsigned int _Seed):
	_Seed(_Seed) {
	//Empty
}

classic_generator::classic_generator(const classic_generator &_Classic_generator) {
	copy(_Classic_generator);
}

classic_generator::~classic_generator() {
	//Empty
}

classic_generator &classic_generator::operator=(const classic_generator &_Classic_generator) {

	//Worth doing
	if (this!=&_Classic_generator) copy(_Classic_generator);

return *this;
}

unsigned int classic_generator::generate_int32() {
	return (unsigned int)rand();
}

unsigned long long classic_generator::generate_int64() {
	return (unsigned long long)rand();
}

float classic_generator::generate_single() {
	return (float)rand()*(1.0f/RAND_MAX);
}

double classic_generator::generate_double() {
	return (double)rand()*(1.0/RAND_MAX);
}

void classic_generator::seed(unsigned int _Seed) {
	this->_Seed=_Seed;
}

unsigned int classic_generator::seed() const {
	return _Seed;
}

} //namespace lunar