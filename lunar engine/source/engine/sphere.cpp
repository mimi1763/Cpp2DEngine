/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sphere.cpp
---------------------------------------------------
*/

#include "sphere.h"

//Dependencies
#include "math_utility.h"

#ifdef _DEBUG

#include "system.h"

#endif

namespace lunar {

//Private

void sphere::copy(const sphere &_Sphere) {

_Radius=_Sphere._Radius;
_Center=_Sphere._Center;
}

//Public

sphere::sphere():_Radius(0.0f) {
	//Empty
}

sphere::sphere(float _Radius,const vector2 &_Center):
	_Radius(_Radius),_Center(_Center) {
	//Empty
}

sphere::sphere(const sphere &_Sphere) {
	copy(_Sphere);
}

sphere::~sphere() {
	//Empty
}

sphere &sphere::operator=(const sphere &_Sphere) {

	//Worth doing
	if (this!=&_Sphere) copy(_Sphere);

return *this;
}

bool sphere::operator==(const sphere &_Sphere) const {
	return (_Radius==_Sphere._Radius);
}

bool sphere::operator!=(const sphere &_Sphere) const {
	return !(*this==_Sphere);
}

bool sphere::intersects(const sphere &_Sphere) const {

float _Radii=_Radius+_Sphere._Radius;
return ((_Sphere._Center-_Center).squared_length()<=_Radii*_Radii);
}

bool sphere::intersects(const vector2 &_Vector) const {
	return ((_Vector-_Center).squared_length()<=_Radius*_Radius);
}

bool sphere::contains(const sphere &_Sphere) const {
	return ((_Sphere._Center-_Center).squared_length()+
		_Sphere._Radius*_Sphere._Radius<_Radius*_Radius);
}

bool sphere::contains(const vector2 &_Vector) const {
	return intersects(_Vector);
}

void sphere::radius(float _Radius) {
	this->_Radius=_Radius;
}

float sphere::radius() const {
	return _Radius;
}

void sphere::diameter(float _Diameter) {
	_Radius=_Diameter*0.5f;
}

float sphere::diameter() const {
	return (_Radius*2.0f);
}

void sphere::center(const vector2 &_Center) {
	this->_Center=_Center;
}

const vector2 &sphere::center() const {
	return _Center;
}

float sphere::area() const {
	return (math::PI*_Radius*_Radius);
}

float sphere::circumference() const {
	return (math::PI*diameter());
}

#ifdef _DEBUG

void sphere::draw() const {

	//Nothing to do
	if (_Radius<=0.0f) return;

glColor3f(0.0f,1.0f,0.0f);
glBegin(GL_LINE_STRIP);

float _Steps=math::TWO_PI/360;
float _Angle=0.0f;

	for (unsigned int i=0;i<360;++i) {
	
	glVertex2f(_Radius*math::cosine(_Angle)+_Center.x,
		_Radius*math::sine(_Angle)+_Center.y);
	_Angle+=_Steps;
	glVertex2f(_Radius*math::cosine(_Angle)+_Center.x,
		_Radius*math::sine(_Angle)+_Center.y);
	}

glEnd();
}

#endif

} //namespace lunar