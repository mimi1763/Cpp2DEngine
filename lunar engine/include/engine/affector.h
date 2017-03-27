/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: affector.h
---------------------------------------------------
*/

#ifndef _AFFECTOR_
#define _AFFECTOR_

namespace lunar {

//Forward declarations
class affector_manager;
class particle;

class affector {

	friend affector_manager;

protected:

	bool _Enabled;
	
	//Protected constructor
	affector();
	
	//Protected copy constructor
	affector(const affector &_Affector);
	
	//Protected virtual destructor
	virtual ~affector();
	
	void copy(const affector &_Affector);

public:

	affector &operator=(const affector &_Affector);
	
	virtual void affect(particle *_Particle,particle *_Particle_end,
		float _Time)=0;
	
	void enable();
	void disable();
	
	bool enabled() const;
};

} //namespace lunar

#endif