/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: gravitation.h
---------------------------------------------------
*/

#ifndef _GRAVITATION_
#define _GRAVITATION_

#include "affector.h"
#include "vector2.h"

namespace lunar {

class gravitation:public affector {

	friend affector_manager;

protected:
	
	vector2 _Position;
	float _Gravity;
	float _Mass;
	
	//Protected constructor
	gravitation();
	
	//Protected copy constructor
	gravitation(const gravitation &_Gravitation);
	
	//Protected virtual destructor
	virtual ~gravitation();
	
	void copy(const gravitation &_Gravitation);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	gravitation &operator=(const gravitation &_Gravitation);
	
	void position(const vector2 &_Position);
	const vector2 &position() const;
	
	void gravity(float _Gravity);
	float gravity() const;
	
	void mass(float _Mass);
	float mass() const;
};

} //namespace lunar

#endif