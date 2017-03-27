/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: border.cpp
---------------------------------------------------
*/

#include "border.h"

namespace lunar {

//Protected

border::border(float _Width,float _Height,
	float _Border_size,const color &_Color,bool _Visible):
		rectangle(_Width,_Height,_Color,_Visible),_Border_size(
			_Border_size<0.0f?0.0f:_Border_size) {
	_Vertex_count*=4; //Modify rectangle vertex count
}

border::border(const border &_Border):rectangle(_Border) {
	copy(_Border);
}

border::~border() {
	//Empty
}

void border::copy(const border &_Border) {
	_Border_size=_Border._Border_size;
}

void border::update_vertices() {

update_origin();

//Top rectangle
_Vertices[0]-=_Border_size;
_Vertices[1]+=_Border_size;

_Vertices[2]=_Vertices[0];
_Vertices[3]=_Vertices[1]-_Border_size;

_Vertices[4]=_Vertices[0]+_Width+(_Border_size*2.0f);
_Vertices[5]=_Vertices[3];

_Vertices[6]=_Vertices[4];
_Vertices[7]=_Vertices[1];

//Left rectangle
_Vertices[8]=_Vertices[0];
_Vertices[9]=_Vertices[3];

_Vertices[10]=_Vertices[0];
_Vertices[11]=_Vertices[3]-_Height;

_Vertices[12]=_Vertices[0]+_Border_size;
_Vertices[13]=_Vertices[11];

_Vertices[14]=_Vertices[12];
_Vertices[15]=_Vertices[3];

//Bottom rectangle
_Vertices[16]=_Vertices[0];
_Vertices[17]=_Vertices[11];

_Vertices[18]=_Vertices[0];
_Vertices[19]=_Vertices[11]-_Border_size;

_Vertices[20]=_Vertices[4];
_Vertices[21]=_Vertices[19];

_Vertices[22]=_Vertices[4];
_Vertices[23]=_Vertices[11];

//Right rectangle
_Vertices[24]=_Vertices[4]-_Border_size;
_Vertices[25]=_Vertices[3];

_Vertices[26]=_Vertices[24];
_Vertices[27]=_Vertices[11];

_Vertices[28]=_Vertices[4];
_Vertices[29]=_Vertices[11];

_Vertices[30]=_Vertices[4];
_Vertices[31]=_Vertices[3];
}

//Public

border &border::operator=(const border &_Border) {

	//Worth doing
	if (this!=&_Border) {
	
	rectangle::operator=(_Border); //Invoke base operator=
	copy(_Border);
	}

return *this;
}

void border::border_size(float _Border_size) {

float _New_size=(_Border_size<0.0f?0.0f:_Border_size);

	if (this->_Border_size!=_New_size) {
	
	this->_Border_size=_New_size;
	_Update_vertices=true; //Update vertices
	}
}

float border::border_size() const {
	return _Border_size;
}

} //namespace lunar