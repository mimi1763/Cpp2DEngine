/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: linear_force.h
---------------------------------------------------
*/

#ifndef _LINEAR_FORCE_
#define _LINEAR_FORCE_

#include "affector.h"
#include "vector2.h"

namespace lunar {

class linear_force:public affector {

	friend affector_manager;

public:

	enum TYPE { ADD, AVERAGE };

protected:

	TYPE _Type;
	vector2 _Force;
	
	//Protected constructor
	linear_force();
	
	//Protected copy constructor
	linear_force(const linear_force &_Linear_force);
	
	//Protected virtual destructor
	virtual ~linear_force();
	
	void copy(const linear_force &_Linear_force);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time);

public:

	linear_force &operator=(const linear_force &_Linear_force);
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void force(const vector2 &_Force);
	const vector2 &force() const;
};

} //namespace lunar

#endif