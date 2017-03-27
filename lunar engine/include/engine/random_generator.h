/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: random_generator.h
---------------------------------------------------
*/

#ifndef _RANDOM_GENERATOR_
#define _RANDOM_GENERATOR_

namespace lunar {

//Forward declarations
class random_manager;

class random_generator {

	friend random_manager;

protected:

	//Protected constructor
	random_generator();
	
	//Protected virtual destructor
	virtual ~random_generator();
	
	//Generate value in range [0, 4 294 967 295]
	virtual unsigned int generate_int32()=0;
	
	//Generate value in range [0, 18 446 744 073 709 551 615]
	virtual unsigned long long generate_int64()=0;
	
	//Generate value in range [0.0, 1.0] with single precision
	virtual float generate_single()=0;
	
	//Generate value in range [0.0, 1.0] with double precision
	virtual double generate_double()=0;
};

class classic_generator:public random_generator {

protected:

	unsigned int _Seed;
	
	void copy(const classic_generator &_Classic_generator);

public:

	//Constructor
	classic_generator(unsigned int _Seed);
	
	//Copy constructor
	classic_generator(const classic_generator &_Classic_generator);
	
	//Virtual destructor
	virtual ~classic_generator();
	
	classic_generator &operator=(const classic_generator &_Classic_generator);
	
	//Generate value in range [0, 4 294 967 295]
	virtual unsigned int generate_int32();
	
	//Generate value in range [0, 18 446 744 073 709 551 615]
	virtual unsigned long long generate_int64();
	
	//Generate value in range [0.0, 1.0] with single precision
	virtual float generate_single();
	
	//Generate value in range [0.0, 1.0] with double precision
	virtual double generate_double();
	
	void seed(unsigned int _Seed);
	unsigned int seed() const;
};

} //namespace lunar

#endif