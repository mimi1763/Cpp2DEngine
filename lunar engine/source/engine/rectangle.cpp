/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: rectangle.cpp
---------------------------------------------------
*/

#include "rectangle.h"

//Dependencies
#include "system.h"

namespace lunar {

//Protected

rectangle::rectangle(float _Width,float _Height,const color &_Color,
	bool _Visible):renderable(GL_QUADS,4,_Color,_Visible),
		_Width(_Width<0.0f?0.0f:_Width),_Height(_Height<0.0f?0.0f:_Height),
			_Origin(CENTER) {
	//Empty
}

rectangle::rectangle(const rectangle &_Rectangle):renderable(_Rectangle) {
	copy(_Rectangle);
}

rectangle::~rectangle() {
	//Empty
}

void rectangle::copy(const rectangle &_Rectangle) {

_Width=_Rectangle._Width;
_Height=_Rectangle._Height;
_Origin=_Rectangle._Origin;
}

void rectangle::update_vertices() {

update_origin();

_Vertices[2]=_Vertices[0];
_Vertices[3]=_Vertices[1]-_Height;

_Vertices[4]=_Vertices[0]+_Width;
_Vertices[5]=_Vertices[3];

_Vertices[6]=_Vertices[4];
_Vertices[7]=_Vertices[1];
}

void rectangle::update_origin() {

	switch (_Origin) {
	
		case TOP_LEFT:
		_Vertices[0]=0;
		_Vertices[1]=0;
		break;
		
		case TOP_CENTER:
		_Vertices[0]=-_Width*0.5f;
		_Vertices[1]=0;
		break;
		
		case TOP_RIGHT:
		_Vertices[0]=-_Width;
		_Vertices[1]=0;
		break;
		
		case CENTER_LEFT:
		_Vertices[0]=0;
		_Vertices[1]=_Height*0.5f;
		break;
		
		case CENTER:
		_Vertices[0]=-_Width*0.5f;
		_Vertices[1]=_Height*0.5f;
		break;
		
		case CENTER_RIGHT:
		_Vertices[0]=-_Width;
		_Vertices[1]=_Height*0.5f;
		break;
		
		case BOTTOM_LEFT:
		_Vertices[0]=0;
		_Vertices[1]=_Height;
		break;
		
		case BOTTOM_CENTER:
		_Vertices[0]=-_Width*0.5f;
		_Vertices[1]=_Height;
		break;
		
		case BOTTOM_RIGHT:
		_Vertices[0]=-_Width;
		_Vertices[1]=_Height;
		break;
	}
}

//Public

rectangle &rectangle::operator=(const rectangle &_Rectangle) {

	//Worth doing
	if (this!=&_Rectangle) {
	
	renderable::operator=(_Rectangle); //Invoke base operator=
	copy(_Rectangle);
	}

return *this;
}

void rectangle::size(float _Width,float _Height) {

float _New_width=(_Width<0.0f?0.0f:_Width);
float _New_height=(_Height<0.0f?0.0f:_Height);

	if (this->_Width!=_New_width || this->_Height!=_New_height) {
	
	this->_Width=_New_width;
	this->_Height=_New_height;
	_Update_vertices=true; //Update vertices
	}
}

float rectangle::width() const {
	return _Width;
}

float rectangle::height() const {
	return _Height;
}

void rectangle::origin(const ORIGIN &_Origin) {

	if (this->_Origin!=_Origin) {
	
	this->_Origin=_Origin;
	_Update_vertices=true; //Update vertices
	}
}

rectangle::ORIGIN rectangle::origin() const {
	return _Origin;
}

} //namespace lunar