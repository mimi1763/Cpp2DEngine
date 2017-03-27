/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: vector2.cpp
---------------------------------------------------
*/

#include "vector2.h"

//Dependencies
#include <cmath>
#include "math_utility.h"

namespace lunar {

//Private

void vector2::copy(const vector2 &_Vector) {

x=_Vector.x;
y=_Vector.y;
}

//Public

const vector2 vector2::ZERO(0.0f,0.0f);
const vector2 vector2::UNIT_X(1.0f,0.0f);
const vector2 vector2::UNIT_Y(0.0f,1.0f);
const vector2 vector2::NEGATIVE_UNIT_X(-1.0f,0.0f);
const vector2 vector2::NEGATIVE_UNIT_Y(0.0f,-1.0f);
const vector2 vector2::UNIT_SCALE(1.0f,1.0f);

vector2::vector2():x(0.0f),y(0.0f) {
	//Empty
}

vector2::vector2(float _X,float _Y):x(_X),y(_Y) {
	//Empty
}

vector2::vector2(float _Scalar):x(_Scalar),y(_Scalar) {
	//Empty
}

vector2::vector2(const vector2 &_Vector) {
	copy(_Vector);
}

vector2::~vector2() {
	//Empty
}

vector2 &vector2::operator=(float _Scalar) {

x=y=_Scalar;
return *this;
}

vector2 &vector2::operator=(const vector2 &_Vector) {

	//Worth doing
	if (this!=&_Vector) copy(_Vector);

return *this;
}

bool vector2::operator==(const vector2 &_Vector) const {
	return (x==_Vector.x && y==_Vector.y);
}

bool vector2::operator!=(const vector2 &_Vector) const {
	return !(*this==_Vector);
}

vector2 vector2::operator-() const {
	return vector2(-x,-y); //Negate
}

vector2 vector2::operator+(float _Scalar) const {
	return vector2(x+_Scalar,y+_Scalar);
}

vector2 vector2::operator+(const vector2 &_Vector) const {
	return vector2(x+_Vector.x,y+_Vector.y);
}

vector2 vector2::operator-(float _Scalar) const {
	return vector2(x-_Scalar,y-_Scalar);
}

vector2 vector2::operator-(const vector2 &_Vector) const {
	return vector2(x-_Vector.x,y-_Vector.y);
}

vector2 vector2::operator*(float _Scalar) const {
	return vector2(x*_Scalar,y*_Scalar);
}

vector2 vector2::operator*(const vector2 &_Vector) const {
	return vector2(x*_Vector.x,y*_Vector.y);
}

vector2 vector2::operator/(float _Scalar) const {
	return vector2(x/_Scalar,y/_Scalar); //Presumes that divisor is non-zero
}

vector2 vector2::operator/(const vector2 &_Vector) const {
	return vector2(x/_Vector.x,y/_Vector.y); //Presumes that divisors are non-zero
}

vector2 &vector2::operator+=(float _Scalar) {

x+=_Scalar;
y+=_Scalar;
return *this;
}

vector2 &vector2::operator+=(const vector2 &_Vector) {

x+=_Vector.x;
y+=_Vector.y;
return *this;
}

vector2 &vector2::operator-=(float _Scalar) {

x-=_Scalar;
y-=_Scalar;
return *this;
}

vector2 &vector2::operator-=(const vector2 &_Vector) {

x-=_Vector.x;
y-=_Vector.y;
return *this;
}

vector2 &vector2::operator*=(float _Scalar) {

x*=_Scalar;
y*=_Scalar;
return *this;
}

vector2 &vector2::operator*=(const vector2 &_Vector) {

x*=_Vector.x;
y*=_Vector.y;
return *this;
}

vector2 &vector2::operator/=(float _Scalar) {

//Presumes that divisor is non-zero
x/=_Scalar;
y/=_Scalar;
return *this;
}

vector2 &vector2::operator/=(const vector2 &_Vector) {

//Presumes that divisors are non-zero
x/=_Vector.x;
y/=_Vector.y;
return *this;
}

vector2 vector2::deviant(float _Angle) const {

float _Sin_angle=math::sine(_Angle);
float _Cos_angle=math::cosine(_Angle);
return vector2(_Cos_angle*x-_Sin_angle*y,
	_Sin_angle*x+_Cos_angle*y);
}

vector2 vector2::midpoint(const vector2 &_Vector) const {
	return vector2((x+_Vector.x)*0.5f,(y+_Vector.y)*0.5f);
}

vector2 vector2::perpendicular() const {
	return vector2(-y,x);
}

vector2 vector2::reflect(const vector2 &_Normal) const {
	return vector2(*this-(_Normal*dot_product(_Normal)*2.0f));
}

vector2 vector2::ceil(const vector2 &_Vector) const {
	return vector2(_Vector.x>x?_Vector.x:x,
		_Vector.y>y?_Vector.y:y);
}

vector2 vector2::floor(const vector2 &_Vector) const {
	return vector2(_Vector.x<x?_Vector.x:x,
		_Vector.y<y?_Vector.y:y);
}

vector2 vector2::lerp(const vector2 &_Vector,float _Amount) const {

	if (_Amount<=0.0f) return *this;
	if (_Amount>=1.0f) return _Vector;

return vector2(math::lerp(x,_Vector.x,_Amount),
	math::lerp(y,_Vector.y,_Amount));
}

vector2 vector2::slerp(vector2 _Vector,float _Amount) const {

	if (_Amount<=0.0f) return *this;
	if (_Amount>=1.0f) return _Vector;

float _Dot=dot_product(_Vector);

	if (_Dot<0.0f) {
	
	_Vector=-_Vector;
	_Dot=-_Dot;
	}
	
	//Fixed potential acos domain error and division by zero
	if (_Dot>1.0f-math::EPSILON) return lerp(_Vector,_Amount);

float _Angle=acos(_Dot);
return vector2(math::slerp(x,_Vector.x,_Angle,_Amount),
	math::slerp(y,_Vector.y,_Angle,_Amount));
}

float vector2::normalize() {

float _Length=length();

	if (_Length>math::EPSILON) {
	
	x/=_Length;
	y/=_Length;
	}

return _Length;
}

vector2 vector2::normalize_copy() const {

vector2 _Vector=*this;
_Vector.normalize();
return _Vector;
}

float vector2::dot_product(const vector2 &_Vector) const {
	return (x*_Vector.x+y*_Vector.y);
}

float vector2::cross_product(const vector2 &_Vector) const {
	return (x*_Vector.y-y*_Vector.x);
}

float vector2::angle(const vector2 &_Vector) const {
	return atan2(cross_product(_Vector), dot_product(_Vector));
}

float vector2::length() const {
	return sqrt(x*x+y*y); //Expensive
}

float vector2::squared_length() const {
	return (x*x+y*y); //Inexpensive
}

bool vector2::zero_length() const {
	return (x*x+y*y<math::EPSILON);
}

float vector2::minimum() const {
	return (x<y?x:y);
}

float vector2::maximum() const {
	return (x>y?x:y);
}

} //namespace lunar