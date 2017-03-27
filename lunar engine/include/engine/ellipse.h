/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: ellipse.h
---------------------------------------------------
*/

#ifndef _ELLIPSE_
#define _ELLIPSE_

#include "renderable.h"

namespace lunar {

class ellipse:public renderable {

	friend scene_manager;

protected:

	//Float to support size interpolation
	float _Width;
	float _Height;
	bool _Solid;
	
	//Protected constructor
	explicit ellipse(float _Radius,const color &_Color=color::WHITE,
		bool _Visible=true);
	
	//Protected alternative constructor
	ellipse(float _Width,float _Height,
		const color &_Color=color::WHITE,bool _Visible=true);
	
	//Protected copy constructor
	ellipse(const ellipse &_Ellipse);
	
	//Protected virtual destructor
	virtual ~ellipse();
	
	void copy(const ellipse &_Ellipse);
	
	virtual void update_vertices();

public:

	ellipse &operator=(const ellipse &_Ellipse);
	
	void radius(float _Radius);
	
	void size(float _Width,float _Height);
	float width() const;
	float height() const;
	
	void solid(bool _Solid);
	bool solid() const;
	
	void steps(unsigned int _Steps);
};

} //namespace lunar

#endif