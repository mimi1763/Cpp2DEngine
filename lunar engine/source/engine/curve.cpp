/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: curve.cpp
---------------------------------------------------
*/

#include "curve.h"

//Dependencies
#include <cmath>
#include "system.h"

#include "math_utility.h"

namespace lunar {

//Protected

curve::curve(float _Line_size,const color &_Color,bool _Visible):
	renderable(GL_LINE_STRIP,0,_Color,_Visible),_Line_size(_Line_size),
		_Smooth_level(5) {
	//Empty
}

curve::curve(const curve &_Curve):renderable(_Curve) {
	copy(_Curve);
}

curve::~curve() {
	//Empty
}

void curve::copy(const curve &_Curve) {

_Line_size=_Curve._Line_size;
_Smooth_level=_Curve._Smooth_level;
_Mycontrolpoints=_Curve._Mycontrolpoints;
}

void curve::pre_draw() {

	if (!_Point_rendering) glLineWidth(_Line_size);

renderable::pre_draw(); //Use base functionality
}

void curve::update_vertices() {

	//Don't use bezier
	if (_Smooth_level==0 || _Vertex_count<2) {
	
		for (unsigned int i=0;i<_Vertex_count;++i) {
		
		_Vertices[i*2]=_Mycontrolpoints[i].x;
		_Vertices[i*2+1]=_Mycontrolpoints[i].y;
		}
	
	return;
	}

//Use bezier
float _X=0.0f,_Y=0.0f;
float _Percent=1.0f;
float _Step=1.0f/(_Vertex_count-1); //vertex count is always greater than 1

unsigned int _Size=(unsigned int)_Mycontrolpoints.size();

	//Calculate each coordinate in the bezier curve
	for (unsigned int i=0;i<_Vertex_count;++i,_Percent-=_Step,_X=_Y=0) {
	
		for (unsigned int j=0;j<_Size;++j) {
		
		float _Bezier=math::choose(_Size-1,j)*pow(_Percent,(float)_Size-j-1)*
			pow(1.0f-_Percent,(float)j);
		
		_X+=_Mycontrolpoints[j].x*_Bezier;
		_Y+=_Mycontrolpoints[j].y*_Bezier;
		}
	
	_Vertices[i*2]=_X;
	_Vertices[i*2+1]=_Y;
	}
}

void curve::update_vertexcount() {

_Vertex_count=(_Smooth_level>1?_Smooth_level:1)*_Mycontrolpoints.size();

_Update_vertices=true; //Update vertices
_Update_colors=true; //Update colors
_Reallocate_buffers=true; //More/less vertices, reallocate
}

//Public

const unsigned int curve::MAX_CONTROLPOINTS=35; //Highest factorial input is 35-1

curve &curve::operator=(const curve &_Curve) {

	//Worth doing
	if (this!=&_Curve) {
	
	renderable::operator=(_Curve); //Invoke base operator=
	
	//Check if vertex count will differ
	bool _Differ=(_Smooth_level!=_Curve._Smooth_level ||
		_Mycontrolpoints.size()!=_Curve._Mycontrolpoints.size());
	copy(_Curve);
	
		if (_Differ) update_vertexcount();
	}

return *this;
}

bool curve::prepare() {

	//Nothing to do
	if (_Vertex_count<2) {
	
	clear(); //Clean up
	return false;
	}

return renderable::prepare(); //Use base functionality
}

void curve::line_size(float _Line_size) {
	this->_Line_size=_Line_size;
}

float curve::line_size() const {
	return _Line_size;
}

void curve::smooth_level(unsigned int _Smooth_level) {

	if (this->_Smooth_level!=_Smooth_level) {
	
	this->_Smooth_level=_Smooth_level;
	update_vertexcount();
	}
}

unsigned int curve::smooth_level() const {
	return _Smooth_level;
}

void curve::add_controlpoint(const vector2 &_Controlpoint) {

	if (_Mycontrolpoints.size()<MAX_CONTROLPOINTS) {
	
	_Mycontrolpoints.push_back(_Controlpoint);
	update_vertexcount();
	}
}

void curve::change_controlpoint(const size_t &_Index,const vector2 &_Controlpoint) {

	if (_Index<_Mycontrolpoints.size()) {
	
	_Mycontrolpoints[_Index]=_Controlpoint;
	_Update_vertices=true; //No need to reallocate
	}
}

void curve::remove_controlpoint(const size_t &_Index) {

	if (_Index<_Mycontrolpoints.size()) {
	
	_Mycontrolpoints.erase(_Mycontrolpoints.begin()+_Index);
	update_vertexcount();
	}
}

void curve::clear_controlpoints() {

	if (!_Mycontrolpoints.empty()) {
	
	_Mycontrolpoints.clear();
	update_vertexcount();
	}
}

} //namespace lunar