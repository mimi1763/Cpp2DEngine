/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: direction_randomizer.h
---------------------------------------------------
*/

#ifndef _DIRECTION_RANDOMIZER_
#define _DIRECTION_RANDOMIZER_

#include "affector.h"

namespace lunar {

class direction_randomizer:public affector {

	friend affector_manager;

protected:

	float _Randomness;
	float _Scope;
	bool _Keep_velocity;
	
	//Protected constructor
	direction_randomizer();
	
	//Protected copy constructor
	direction_randomizer(const direction_randomizer &_Direction_randomizer);
	
	//Protected virtual destructor
	virtual ~direction_randomizer();
	
	void copy(const direction_randomizer &_Direction_randomizer);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	direction_randomizer &operator=(const direction_randomizer &_Direction_randomizer);
	
	void randomness(float _Randomness);
	float randomness() const;
	
	void scope(float _Scope);
	float scope() const;
	
	void keep_velocity(bool _Keep_velocity);
	bool keep_velocity() const;
};

} //namespace lunar

#endif