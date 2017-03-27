/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: obb.cpp
---------------------------------------------------
*/

#include "obb.h"

#ifdef _DEBUG

//Dependencies
#include "system.h"

#endif

namespace lunar {

//Private

void obb::copy(const obb &_Obb) {

_Corners[0]=_Obb._Corners[0]; _Corners[1]=_Obb._Corners[1];
_Corners[2]=_Obb._Corners[2]; _Corners[3]=_Obb._Corners[3];
_Size=_Obb._Size;

_Axis[0]=_Obb._Axis[0];
_Axis[1]=_Obb._Axis[1];
_Origin=_Obb._Origin;

_Empty=_Obb._Empty;
}

bool obb::intersects_oneway(const obb &_Obb) const {

	//Axes
	for (unsigned int i=0;i<2;++i) {
	
	//Find the extent of _Obb on current axis
	float _Dot=_Obb._Corners[0].dot_product(_Axis[i]);
	float _Dotmin=_Dot;
	float _Dotmax=_Dot;
	
		//Corners
		for (unsigned int j=1;j<4;++j) {
		
		_Dot=_Obb._Corners[j].dot_product(_Axis[i]);
		
			if (_Dot<_Dotmin) _Dotmin=_Dot;
			else if (_Dot>_Dotmax) _Dotmax=_Dot;
		}
		
		//Substract off the origin and see if [_Tmin,_Tmax] intersects [0,1]
		if (_Dotmin>1.0f+_Origin.ptr[i] || _Dotmax<_Origin.ptr[i])
			return false;
			//There was no intersection along this dimension
            //The boxes cannot intersect
	}

//There was no dimension along which there is no intersection
//Therefore the boxes overlap
return true;
}

//Public

obb::obb():_Empty(true) {
	//Empty
}

obb::obb(const vector2 &_Bottom_left,const vector2 &_Bottom_right,
	const vector2 &_Top_right,const vector2 &_Top_left,
		const vector2 &_Size):_Empty(true) {
	corners(_Bottom_left,_Bottom_right,_Top_right,_Top_left,_Size);
}

obb::obb(const aabb &_Aabb):_Empty(true) {
	*this=_Aabb;
}

obb::obb(const obb &_Obb) {
	copy(_Obb);
}

obb::~obb() {
	//Empty
}

obb &obb::operator=(const obb &_Obb) {

	//Worth doing
	if (this!=&_Obb) copy(_Obb);

return *this;
}

obb &obb::operator=(const aabb &_Aabb) {

const vector2 &_Min=_Aabb.minimum();
const vector2 &_Max=_Aabb.maximum();

corners(_Min,vector2(_Max.x,_Min.y),_Max,
	vector2(_Min.x,_Max.y),_Aabb.size());
return *this;
}

bool obb::operator==(const obb &_Obb) const {

	if (_Empty!=_Obb._Empty) return false;

return (_Corners[0]==_Obb._Corners[0] && _Corners[1]==_Obb._Corners[1] &&
		_Corners[2]==_Obb._Corners[2] && _Corners[3]==_Obb._Corners[3]);
}

bool obb::operator!=(const obb &_Obb) const {
	return !(*this==_Obb);
}

void obb::transform_affine(const matrix3 &_Matrix3) {

	if (_Empty || !_Matrix3.affine()) return;

vector2 _Center=center();
vector2 _Halfsize=half_size();

vector2 _New_center(_Matrix3.transform_affine(_Center));
vector2 _X(_Matrix3.m[0][0],_Matrix3.m[1][0]);
vector2 _Y(_Matrix3.m[0][1],_Matrix3.m[1][1]);

_X*=_Halfsize.x;
_Y*=_Halfsize.y;

//Update corners after transformation
_Corners[0]=_New_center-_X-_Y;
_Corners[1]=_New_center+_X-_Y;
_Corners[2]=_New_center+_X+_Y;
_Corners[3]=_New_center-_X+_Y;

//Update axes vectors pointing out of the first corner
_Axis[0]=_Corners[1]-_Corners[0];
_Axis[1]=_Corners[3]-_Corners[0];

	for (unsigned int i=0;i<2;++i) {
	
	_Axis[i]/=_Axis[i].squared_length();
	_Origin.ptr[i]=_Corners[0].dot_product(_Axis[i]);
	}
}

bool obb::intersects(const obb &_Obb) const {
	return (intersects_oneway(_Obb) && _Obb.intersects_oneway(*this));
}

bool obb::intersects(const vector2 &_Vector) const {

obb _Obb(vector2(_Vector.x,_Vector.y),vector2(_Vector.x+1.0f,_Vector.y),
	vector2(_Vector.x+1.0f,_Vector.y+1.0f),vector2(_Vector.x,_Vector.y+1.0f),
		vector2(1.0f));
return (intersects_oneway(_Obb) && _Obb.intersects_oneway(*this));
}

void obb::clear() {

_Corners[0]=vector2::ZERO;
_Corners[1]=vector2::ZERO;
_Corners[2]=vector2::ZERO;
_Corners[3]=vector2::ZERO;
_Size=vector2::ZERO;

_Empty=true;
}

bool obb::empty() const {
	return _Empty;
}

void obb::corners(const vector2 &_Bottom_left,const vector2 &_Bottom_right,
	const vector2 &_Top_right,const vector2 &_Top_left,const vector2 &_Size) {

_Corners[0]=_Bottom_left; _Corners[1]=_Bottom_right;
_Corners[2]=_Top_right;   _Corners[3]=_Top_left;
this->_Size=_Size;

_Empty=false;
}

const vector2 *obb::corners() const {
	return _Corners;
}

vector2 obb::center() const {
	return vector2((_Corners[0]+_Corners[1]+_Corners[2]+_Corners[3])*0.25f);
}

const vector2 &obb::size() const {
	return _Size;
}

vector2 obb::half_size() const {
	return vector2(_Size*0.5f);
}

#ifdef _DEBUG

void obb::draw() const {

	//Nothing to do
	if (_Empty) return;

glColor3f(0.0f,0.0f,1.0f);

glBegin(GL_LINE_STRIP);
glVertex2fv(_Corners[3].ptr);
glVertex2fv(_Corners[0].ptr);
glVertex2fv(_Corners[1].ptr);
glVertex2fv(_Corners[2].ptr);
glVertex2fv(_Corners[3].ptr);
glEnd();
}

#endif

} //namespace lunar