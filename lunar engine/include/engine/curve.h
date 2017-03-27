/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: curve.h
---------------------------------------------------
*/

#ifndef _CURVE_
#define _CURVE_

#include <vector>
#include "renderable.h"

namespace lunar {

class curve:public renderable {

	friend scene_manager;

protected:

	typedef std::vector<vector2> _Controlpoints; //Need random access
	
	float _Line_size;
	unsigned int _Smooth_level;
	_Controlpoints _Mycontrolpoints;
	
	//Protected constructor
	explicit curve(float _Line_size=2.0f,const color &_Color=color::WHITE,
		bool _Visible=true);
	
	//Protected copy constructor
	curve(const curve &_Curve);
	
	//Protected virtual destructor
	virtual ~curve();
	
	void copy(const curve &_Curve);
	
	virtual void pre_draw();
	//virtual void post_draw(); //Not in use
	
	virtual void update_vertices();
	
	void update_vertexcount();

public:

	static const unsigned int MAX_CONTROLPOINTS;
	
	curve &operator=(const curve &_Curve);
	
	virtual bool prepare();
	
	void line_size(float _Line_size);
	float line_size() const;
	
	void smooth_level(unsigned int _Smooth_level);
	unsigned int smooth_level() const;
	
	void add_controlpoint(const vector2 &_Controlpoint);
	void change_controlpoint(const size_t &_Index,const vector2 &_Controlpoint);
	
	void remove_controlpoint(const size_t &_Index);
	void clear_controlpoints();
};

} //namespace lunar

#endif