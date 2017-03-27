/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: ellipse.cpp
---------------------------------------------------
*/

#include "ellipse.h"

//Dependencies
#include "system.h"
#include "math_utility.h"

namespace lunar {

//Protected

ellipse::ellipse(float _Radius,const color &_Color,bool _Visible):
	renderable(GL_TRIANGLE_FAN,36,_Color,_Visible),_Width(_Radius*2.0f<0.0f?
		0.0f:_Radius*2.0f),_Height(_Radius*2.0f<0.0f?0.0f:_Radius*2.0f),
			_Solid(true) {
	//Empty
}

ellipse::ellipse(float _Width,float _Height,const color &_Color,
	bool _Visible):renderable(GL_TRIANGLE_FAN,36,_Color,_Visible),
		_Width(_Width<0.0f?0.0f:_Width),_Height(_Height<0.0f?0.0f:_Height),
			_Solid(true) {
	//Empty
}

ellipse::ellipse(const ellipse &_Ellipse):renderable(_Ellipse) {
	copy(_Ellipse);
}

ellipse::~ellipse() {
	//Empty
}

void ellipse::copy(const ellipse &_Ellipse) {

_Width=_Ellipse._Width;
_Height=_Ellipse._Height;
_Solid=_Ellipse._Solid;
}

void ellipse::update_vertices() {

float _Steps=math::TWO_PI/(_Vertex_count-
	(!_Solid && _Vertex_count>1?1:0));
float _Angle=0.0f;

	for (unsigned int i=0;i<_Vertex_count;++i,_Angle+=_Steps) {
	
	_Vertices[i*2]=(_Width*0.5f)*math::cosine(_Angle);
	_Vertices[i*2+1]=(_Height*0.5f)*math::sine(_Angle);
	}
}

//Public

ellipse &ellipse::operator=(const ellipse &_Ellipse) {

	//Worth doing
	if (this!=&_Ellipse) {
	
	renderable::operator=(_Ellipse); //Invoke base operator=
	copy(_Ellipse);
	}

return *this;
}

void ellipse::radius(float _Radius) {
	size(_Radius*2.0f,_Radius*2.0f);
}

void ellipse::size(float _Width,float _Height) {

float _New_width=(_Width<0.0f?0.0f:_Width);
float _New_height=(_Height<0.0f?0.0f:_Height);

	if (this->_Width!=_New_width || this->_Height!=_New_height) {
	
	this->_Width=_New_width;
	this->_Height=_New_height;
	_Update_vertices=true; //Update vertices
	}
}

float ellipse::width() const {
	return _Width;
}

float ellipse::height() const {
	return _Height;
}

void ellipse::solid(bool _Solid) {

	if (this->_Solid!=_Solid) {
	
		if (_Solid) _Draw_mode=GL_TRIANGLE_FAN;
		else _Draw_mode=GL_LINE_STRIP;
	
	this->_Solid=_Solid;
	_Update_vertices=true; //Update vertices
	}
}

bool ellipse::solid() const {
	return _Solid;
}

void ellipse::steps(unsigned int _Steps) {

	if (_Steps>2 && _Steps!=_Vertex_count) {
	
	_Vertex_count=_Steps;
	_Update_vertices=true; //Update vertices
	_Update_colors=true; //Update colors
	_Reallocate_buffers=true; //More/less vertices, reallocate
	}
}

} //namespace lunar