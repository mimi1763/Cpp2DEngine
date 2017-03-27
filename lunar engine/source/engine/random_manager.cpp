/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: random_manager.cpp
---------------------------------------------------
*/

#include "random_manager.h"

//Dependencies
#include <cstdlib>
#include <ctime>

#include "timeline.h"

namespace lunar {

//Private

random_generator *random_manager::_Default_generator=0;
random_generator *random_manager::_Custom_generator=0;

random_manager::random_manager() {

srand((unsigned int)time(0)); //Initialize default seed

	//Initialize default random generator
	if (!_Default_generator) {
	
	unsigned long long _Seed=timeline::ticks();
	
	static mersenne_twister _Mersenne_twister=mersenne_twister();
	_Mersenne_twister.mt32().init((unsigned int)(_Seed%0xffffffffULL));
	_Mersenne_twister.mt64().init(_Seed);
	
	_Default_generator=&_Mersenne_twister;
	}
}

random_manager::random_manager(const random_manager &_Random_manager) {
	copy(_Random_manager);
}

random_manager::~random_manager() {
	//Empty
}

void random_manager::copy(const random_manager &_Random_manager) {
	//Empty - override default memcpy behaviour
}

//Public

random_manager &random_manager::operator=(const random_manager &_Random_manager) {

	//Worth doing
	if (this!=&_Random_manager) copy(_Random_manager);

return *this;
}

void random_manager::use(random_generator *_Generator) {

_Custom_generator=_Generator;

	if (_Custom_generator) {
	
	classic_generator *_Classic_generator=dynamic_cast<
		classic_generator*>(_Custom_generator);
	
		//Initialize seed
		if (_Classic_generator) srand(_Classic_generator->seed());
	}
}

random_generator *random_manager::generator() {
	return _Custom_generator;
}

unsigned int random_manager::generate_int32() {
	return (_Custom_generator?_Custom_generator:_Default_generator)->
		generate_int32();
}

unsigned long long random_manager::generate_int64() {
	return (_Custom_generator?_Custom_generator:_Default_generator)->
		generate_int64();
}

float random_manager::generate_single() {
	return (_Custom_generator?_Custom_generator:_Default_generator)->
		generate_single();
}

double random_manager::generate_double() {
	return (_Custom_generator?_Custom_generator:_Default_generator)->
		generate_double();
}

} //namespace lunar