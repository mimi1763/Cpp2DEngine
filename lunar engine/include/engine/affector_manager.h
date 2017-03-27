/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: affector_manager.h
---------------------------------------------------
*/

#ifndef _AFFECTOR_MANAGER_
#define _AFFECTOR_MANAGER_

#include <list>

#include "color_fader.h"
#include "direction_randomizer.h"
#include "gravitation.h"
#include "linear_force.h"
#include "scaler.h"
#include "sine_force.h"

namespace lunar {

class affector_manager {

protected:

	typedef std::list<affector*> _Affectors;
	
	_Affectors _Myaffectors;
	
	//Protected constructor
	affector_manager();
	
	//Protected copy constructor
	affector_manager(const affector_manager &_Affector_manager);
	
	//Protected virtual destructor
	virtual ~affector_manager();
	
	void copy(const affector_manager &_Affector_manager);

public:

	typedef _Affectors::const_iterator affectors_iter; //Keep data safe
	
	affector_manager &operator=(const affector_manager &_Affector_manager);
	
	virtual void update(float _Time)=0;
	
	color_fader *create_colorfader();
	color_fader *create_colorfader(const color_fader &_Color_fader);
	
	direction_randomizer *create_directionrandomizer();
	direction_randomizer *create_directionrandomizer(
		const direction_randomizer &_Direction_randomizer);
	
	gravitation *create_gravitation();
	gravitation *create_gravitation(const gravitation &_Gravitation);
	
	linear_force *create_linearforce();
	linear_force *create_linearforce(const linear_force &_Linear_force);
	
	scaler *create_scaler();
	scaler *create_scaler(const scaler &_Scaler);
	
	sine_force *create_sineforce();
	sine_force *create_sineforce(const sine_force &_Sine_force);
	
	void remove_affector(affector *&_Affector);
	
	//Support remove by reference for derived types
	template <typename _Ty>
	void remove_affector(_Ty &_Derived) {
	
	affector *_Affector=_Derived;
	remove_affector(_Affector);
	
		//Set referenced pointer to null
		if (!_Affector) _Derived=0;
	}
	
	void clear_affectors();
	
	affectors_iter affectors_begin() const;
	affectors_iter affectors_end() const;
};

} //namespace lunar

#endif