/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particle.h
---------------------------------------------------
*/

#ifndef _PARTICLE_
#define _PARTICLE_

#include "vector2.h"
#include "color.h"

namespace lunar {

//Forward declarations
class emitter;

class particle {

	friend emitter;

private:

	//Private constructor
	particle();
	
	//Private constructor
	particle(const vector2 &_Position,const vector2 &_Direction,
		float _Size,float _Lifetime,float _Mass,
			const color &_Color=color::WHITE);
	
	//Private copy constructor
	particle(const particle &_Particle);
	
	//Private destructor
	~particle();
	
	void copy(const particle &_Particle);

public:

	particle &operator=(const particle &_Particle);
	
	//Public for optimization purposes
	//Using _<name> because this is a non user class
	
	vector2 _Position;
	vector2 _Direction; //Where length is velocity
	float _Size;
	
	float _Lifetime;
	float _Mass;
	color _Color;
	
	float _Total_lifetime; //Needed for interpolation
};

} //namespace lunar

#endif