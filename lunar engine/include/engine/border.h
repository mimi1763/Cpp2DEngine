/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: border.h
---------------------------------------------------
*/

#ifndef _BORDER_
#define _BORDER_

#include "rectangle.h"

namespace lunar {

class border:public rectangle {

	friend scene_manager;

protected:

	float _Border_size;
	
	//Protected constructor
	border(float _Width,float _Height,float _Border_size,
		const color &_Color=color::WHITE,bool _Visible=true);
	
	//Protected copy constructor
	border(const border &_Border);
	
	//Protected virtual destructor
	virtual ~border();
	
	void copy(const border &_Border);
	
	virtual void update_vertices();

public:

	border &operator=(const border &_Border);
	
	void border_size(float _Border_size);
	float border_size() const;
};

} //namespace lunar

#endif