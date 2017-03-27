/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: aabb.cpp
---------------------------------------------------
*/

#include "aabb.h"

//Dependencies
#include <cmath>
#include <vector>

#ifdef _DEBUG

#include "system.h"

#endif

namespace lunar {

//Private

void aabb::copy(const aabb &_Aabb) {

_Min=_Aabb._Min;
_Max=_Aabb._Max;
_Empty=_Aabb._Empty;
}

//Public

aabb::aabb():_Empty(true) {
	//Empty
}

aabb::aabb(const vector2 &_Min,const vector2 &_Max):_Empty(true) {
	extents(_Min,_Max);
}

aabb::aabb(const aabb &_Aabb) {
	copy(_Aabb);
}

aabb::~aabb() {
	//Empty
}

aabb &aabb::operator=(const aabb &_Aabb) {

	//Worth doing
	if (this!=&_Aabb) copy(_Aabb);

return *this;
}

bool aabb::operator==(const aabb &_Aabb) const {

	if (_Empty!=_Aabb._Empty) return false;

return (_Min==_Aabb._Min && _Max==_Aabb._Max);
}

bool aabb::operator!=(const aabb &_Aabb) const {
	return !(*this==_Aabb);
}

void aabb::extents(const vector2 &_Min,const vector2 &_Max) {

	if (_Min.x>_Max.x || _Min.y>_Max.y) return; //Don't want illegal values

this->_Min=_Min;
this->_Max=_Max;
_Empty=false;
}

void aabb::scale(const vector2 &_Scale) {

	if (_Empty) return;

extents(_Min*_Scale,_Max*_Scale);
}

void aabb::transform_affine(const matrix3 &_Matrix3) {

	if (_Empty || !_Matrix3.affine()) return;

vector2 _Center=center();
vector2 _Halfsize=half_size();

vector2 _New_center(_Matrix3.transform_affine(_Center));
vector2 _New_halfsize(fabs(_Matrix3.m[0][0])*_Halfsize.x+fabs(_Matrix3.m[0][1])*_Halfsize.y,
	fabs(_Matrix3.m[1][0])*_Halfsize.x+fabs(_Matrix3.m[1][1])*_Halfsize.y);

extents(_New_center-_New_halfsize,_New_center+_New_halfsize);
}

bool aabb::intersects(const aabb &_Aabb) const {

	if (_Empty || _Aabb._Empty) return false;
	
	if (_Max.x<_Aabb._Min.x) return false;
	if (_Max.y<_Aabb._Min.y) return false;
	
	if (_Min.x>_Aabb._Max.x) return false;
	if (_Min.y>_Aabb._Max.y) return false;

return true;
}

bool aabb::intersects(const vector2 &_Vector) const {

	if (_Empty) return false;

return (_Vector.x>=_Min.x && _Vector.x<=_Max.x &&
	_Vector.y>=_Min.y && _Vector.y<=_Max.y);
}

aabb aabb::intersection(const aabb &_Aabb) const {

	if (_Empty || _Aabb._Empty) return aabb();

vector2 _Intersect_min(_Min.ceil(_Aabb._Min));
vector2 _Intersect_max(_Max.floor(_Aabb._Max));

	if (_Intersect_min.x<_Intersect_max.x &&
		_Intersect_min.y<_Intersect_max.y) {
	
	return aabb(_Intersect_min,_Intersect_max);
	}

return aabb();
}

bool aabb::contains(const aabb &_Aabb) const {

	if (_Empty || _Aabb._Empty) return false;

return (_Min.x<=_Aabb._Min.x && _Min.y<=_Aabb._Min.y &&
	_Max.x>=_Aabb._Max.x && _Max.y>=_Aabb._Max.y);
}

bool aabb::contains(const vector2 &_Vector) const {
	return intersects(_Vector);
}

void aabb::merge(const aabb &_Aabb) {

	if (_Aabb._Empty) return;

	if (_Empty) extents(_Aabb._Min,_Aabb._Max);
	else extents(_Min.floor(_Aabb._Min),_Max.ceil(_Aabb._Max));
}

void aabb::merge(const vector2 &_Vector) {

	if (_Empty) extents(_Vector,_Vector);
	else extents(_Min.floor(_Vector),_Max.ceil(_Vector));
}

void aabb::clear() {

_Min=vector2::ZERO;
_Max=vector2::ZERO;
_Empty=true;
}

bool aabb::empty() const {
	return _Empty;
}

const vector2 &aabb::minimum() const {
	return _Min;
}

const vector2 &aabb::maximum() const {
	return _Max;
}

vector2 aabb::center() const {
	return vector2((_Max.x+_Min.x)*0.5f,(_Max.y+_Min.y)*0.5f);
}

vector2 aabb::size() const {
	return vector2(_Max-_Min);
}

vector2 aabb::half_size() const {
	return vector2((_Max-_Min)*0.5f);
}

float aabb::area() const {

vector2 _Difference(_Max-_Min);
return (_Difference.x*_Difference.y);
}

#ifdef _DEBUG

void aabb::draw() const {

	//Nothing to do
	if (_Empty) return;

glColor3f(1.0f,0.0f,0.0f);

glBegin(GL_LINE_STRIP);
glVertex2f(_Min.x,_Max.y);
glVertex2f(_Min.x,_Min.y);
glVertex2f(_Max.x,_Min.y);
glVertex2f(_Max.x,_Max.y);
glVertex2f(_Min.x,_Max.y);
glEnd();
}

#endif

} //namespace lunar