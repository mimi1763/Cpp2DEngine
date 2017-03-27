/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sine_force.h
---------------------------------------------------
*/

#ifndef _SINE_FORCE_
#define _SINE_FORCE_

#include "affector.h"
#include "vector2.h"

namespace lunar {

class sine_force:public affector {

	friend affector_manager;

public:

	enum TYPE { ADD, AVERAGE };

protected:

	TYPE _Type;
	vector2 _Force;
	
	float _Frequency;
	float _Frequency_min;
	float _Frequency_max;
	float _Angle;
	
	//Protected constructor
	sine_force();
	
	//Protected copy constructor
	sine_force(const sine_force &_Sine_force);
	
	//Protected virtual destructor
	virtual ~sine_force();
	
	void copy(const sine_force &_Sine_force);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	sine_force &operator=(const sine_force &_Sine_force);
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void force(const vector2 &_Force);
	const vector2 &force() const;
	
	void frequency(float _Frequency_min,float _Frequency_max);
	float frequency_min() const;
	float frequency_max() const;
};

} //namespace lunar

#endif