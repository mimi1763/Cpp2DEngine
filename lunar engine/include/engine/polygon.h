/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: polygon.h
---------------------------------------------------
*/

#ifndef _POLYGON_
#define _POLYGON_

#include <vector>
#include "renderable.h"

namespace lunar {

class polygon:public renderable {

	friend scene_manager;

protected:

	typedef std::vector<vector2> _Points; //Need random access
	
	_Points _Mypoints;
	
	//Protected constructor
	explicit polygon(const color &_Color=color::WHITE,
		bool _Visible=true);
	
	//Protected copy constructor
	polygon(const polygon &_Polygon);
	
	//Protected virtual destructor
	virtual ~polygon();
	
	void copy(const polygon &_Polygon);
	
	virtual void update_vertices();
	
	void update_vertexcount();

public:

	polygon &operator=(const polygon &_Polygon);
	
	virtual bool prepare();
	
	unsigned int sides();
	
	void add_point(const vector2 &_Point);
	void change_point(const size_t &_Index,const vector2 &_Point);
	
	void remove_point(const size_t &_Index);
	void clear_points();
};

} //namespace lunar

#endif