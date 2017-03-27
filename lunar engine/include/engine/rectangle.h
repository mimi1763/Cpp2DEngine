/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: rectangle.h
---------------------------------------------------
*/

#ifndef _RECTANGLE_
#define _RECTANGLE_

#include "renderable.h"

namespace lunar {

class rectangle:public renderable {

	friend scene_manager;

public:

	enum ORIGIN {	TOP_LEFT,		TOP_CENTER,		TOP_RIGHT,
					CENTER_LEFT,	CENTER,			CENTER_RIGHT,
					BOTTOM_LEFT,	BOTTOM_CENTER,	BOTTOM_RIGHT };

protected:

	//Float to support size interpolation
	float _Width;
	float _Height;
	
	ORIGIN _Origin;
	
	//Protected constructor
	rectangle(float _Width,float _Height,
		const color &_Color=color::WHITE,bool _Visible=true);
	
	//Protected copy constructor
	rectangle(const rectangle &_Rectangle);
	
	//Protected virtual destructor
	virtual ~rectangle();
	
	void copy(const rectangle &_Rectangle);
	
	virtual void update_vertices();
	
	void update_origin();

public:

	rectangle &operator=(const rectangle &_Rectangle);
	
	void size(float _Width,float _Height);
	float width() const;
	float height() const;
	
	void origin(const ORIGIN &_Origin);
	ORIGIN origin() const;
};

} //namespace lunar

#endif