/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: random_manager.h
---------------------------------------------------
*/

#ifndef _RANDOM_MANAGER_
#define _RANDOM_MANAGER_

#include "mersenne_twister.h"

namespace lunar {

//Forward declarations
class core;

class random_manager {

	friend core; //Grant core exclusive access to initialize this

private:

	static random_generator *_Default_generator;
	static random_generator *_Custom_generator;
	
	//Private constructor
	random_manager();
	
	//Private copy constructor
	random_manager(const random_manager &_Random_manager);
	
	//Private destructor
	~random_manager();
	
	void copy(const random_manager &_Random_manager);

public:

	random_manager &operator=(const random_manager &_Random_manager);
	
	static void use(random_generator *_Generator);
	static random_generator *generator();
	
	//Generate value in range [0, 4 294 967 295]
	static unsigned int generate_int32();
	
	//Generate value in range [0, 18 446 744 073 709 551 615]
	static unsigned long long generate_int64();
	
	//Generate value in range [0.0, 1.0] with single precision
	static float generate_single();
	
	//Generate value in range [0.0, 1.0] with double precision
	static double generate_double();
};

} //namespace lunar

#endif