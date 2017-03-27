/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: matrix3.cpp
---------------------------------------------------
*/

#include "matrix3.h"

//Dependencies
#include "math_utility.h"

namespace lunar {

//Private

void matrix3::copy(const matrix3 &_Matrix3) {

m[0][0]=_Matrix3.m[0][0]; m[0][1]=_Matrix3.m[0][1]; m[0][2]=_Matrix3.m[0][2];
m[1][0]=_Matrix3.m[1][0]; m[1][1]=_Matrix3.m[1][1]; m[1][2]=_Matrix3.m[1][2];
m[2][0]=_Matrix3.m[2][0]; m[2][1]=_Matrix3.m[2][1]; m[2][2]=_Matrix3.m[2][2];
}

//Public

const matrix3 matrix3::ZERO(0.0f,0.0f,0.0f,
							0.0f,0.0f,0.0f,
							0.0f,0.0f,0.0f);

const matrix3 matrix3::IDENTITY(1.0f,0.0f,0.0f,
								0.0f,1.0f,0.0f,
								0.0f,0.0f,1.0f);

matrix3::matrix3() {
	copy(IDENTITY);
}

matrix3::matrix3(float _M00,float _M01,float _M02,
				 float _M10,float _M11,float _M12,
				 float _M20,float _M21,float _M22) {

m[0][0]=_M00; m[0][1]=_M01; m[0][2]=_M02;
m[1][0]=_M10; m[1][1]=_M11; m[1][2]=_M12;
m[2][0]=_M20; m[2][1]=_M21; m[2][2]=_M22;
}

matrix3::matrix3(const matrix3 &_Matrix3) {
	copy(_Matrix3);
}

matrix3::~matrix3() {
	//Empty
}

matrix3 &matrix3::operator=(const matrix3 &_Matrix3) {

	//Worth doing
	if (this!=&_Matrix3) copy(_Matrix3);

return *this;
}

bool matrix3::operator==(const matrix3 &_Matrix3) const {
	return !(*this!=_Matrix3);
}

bool matrix3::operator!=(const matrix3 &_Matrix3) const {
	return (
	m[0][0]!=_Matrix3.m[0][0] || m[0][1]!=_Matrix3.m[0][1] || m[0][2]!=_Matrix3.m[0][2] ||
	m[1][0]!=_Matrix3.m[1][0] || m[1][1]!=_Matrix3.m[1][1] || m[1][2]!=_Matrix3.m[1][2] ||
	m[2][0]!=_Matrix3.m[2][0] || m[2][1]!=_Matrix3.m[2][1] || m[2][2]!=_Matrix3.m[2][2]);
}

matrix3 matrix3::operator*(const matrix3 &_Matrix3) const {
	return matrix3(
	m[0][0]*_Matrix3.m[0][0]+m[0][1]*_Matrix3.m[1][0]+m[0][2]*_Matrix3.m[2][0],
	m[0][0]*_Matrix3.m[0][1]+m[0][1]*_Matrix3.m[1][1]+m[0][2]*_Matrix3.m[2][1],
	m[0][0]*_Matrix3.m[0][2]+m[0][1]*_Matrix3.m[1][2]+m[0][2]*_Matrix3.m[2][2],
	
	m[1][0]*_Matrix3.m[0][0]+m[1][1]*_Matrix3.m[1][0]+m[1][2]*_Matrix3.m[2][0],
	m[1][0]*_Matrix3.m[0][1]+m[1][1]*_Matrix3.m[1][1]+m[1][2]*_Matrix3.m[2][1],
	m[1][0]*_Matrix3.m[0][2]+m[1][1]*_Matrix3.m[1][2]+m[1][2]*_Matrix3.m[2][2],
	
	m[2][0]*_Matrix3.m[0][0]+m[2][1]*_Matrix3.m[1][0]+m[2][2]*_Matrix3.m[2][0],
	m[2][0]*_Matrix3.m[0][1]+m[2][1]*_Matrix3.m[1][1]+m[2][2]*_Matrix3.m[2][1],
	m[2][0]*_Matrix3.m[0][2]+m[2][1]*_Matrix3.m[1][2]+m[2][2]*_Matrix3.m[2][2]);
}

matrix3 matrix3::operator*(float _Scalar) const {
	return matrix3(
	m[0][0]*_Scalar,m[0][1]*_Scalar,m[0][2]*_Scalar,
	m[1][0]*_Scalar,m[1][1]*_Scalar,m[1][2]*_Scalar,
	m[2][0]*_Scalar,m[2][1]*_Scalar,m[2][2]*_Scalar);
}

vector2 matrix3::operator*(const vector2 &_Vector) const {

float _Invw=1.0f/(m[2][0]*_Vector.x+m[2][1]*_Vector.y+m[2][2]);

return vector2((m[0][0]*_Vector.x+m[0][1]*_Vector.y+m[0][2])*_Invw,
	(m[1][0]*_Vector.x+m[1][1]*_Vector.y+m[1][2])*_Invw);
}

matrix3 matrix3::operator+(const matrix3 &_Matrix3) const {
	return matrix3(
	m[0][0]+_Matrix3.m[0][0],m[0][1]+_Matrix3.m[0][1],m[0][2]+_Matrix3.m[0][2],
	m[1][0]+_Matrix3.m[1][0],m[1][1]+_Matrix3.m[1][1],m[1][2]+_Matrix3.m[1][2],
	m[2][0]+_Matrix3.m[2][0],m[2][1]+_Matrix3.m[2][1],m[2][2]+_Matrix3.m[2][2]);
}

matrix3 matrix3::operator+(float _Scalar) const {
	return matrix3(
	m[0][0]+_Scalar,m[0][1]+_Scalar,m[0][2]+_Scalar,
	m[1][0]+_Scalar,m[1][1]+_Scalar,m[1][2]+_Scalar,
	m[2][0]+_Scalar,m[2][1]+_Scalar,m[2][2]+_Scalar);
}

matrix3 matrix3::operator-(const matrix3 &_Matrix3) const {
	return matrix3(
	m[0][0]-_Matrix3.m[0][0],m[0][1]-_Matrix3.m[0][1],m[0][2]-_Matrix3.m[0][2],
	m[1][0]-_Matrix3.m[1][0],m[1][1]-_Matrix3.m[1][1],m[1][2]-_Matrix3.m[1][2],
	m[2][0]-_Matrix3.m[2][0],m[2][1]-_Matrix3.m[2][1],m[2][2]-_Matrix3.m[2][2]);
}

matrix3 matrix3::operator-(float _Scalar) const {
	return matrix3(
	m[0][0]-_Scalar,m[0][1]-_Scalar,m[0][2]-_Scalar,
	m[1][0]-_Scalar,m[1][1]-_Scalar,m[1][2]-_Scalar,
	m[2][0]-_Scalar,m[2][1]-_Scalar,m[2][2]-_Scalar);
}

matrix3 matrix3::concatenate(const matrix3 &_Matrix3) const {
	return ((*this)*_Matrix3);
}

matrix3 matrix3::transpose() const {

	return matrix3(
	m[0][0],m[1][0],m[2][0],
	m[0][1],m[1][1],m[2][1],
	m[0][2],m[1][2],m[2][2]);
}

//Translation transformation

void matrix3::translation(const vector2 &_Vector) {

m[0][2]=_Vector.x;
m[1][2]=_Vector.y;
}

matrix3 matrix3::translation_matrix(const vector2 &_Vector) const {

matrix3 _Matrix3; //IDENTITY matrix
_Matrix3.m[0][2]=_Vector.x;
_Matrix3.m[1][2]=_Vector.y;
return _Matrix3;
}

vector2 matrix3::translation_vector() const {
	return vector2(m[0][2],m[1][2]);
}

//Scale transformation

void matrix3::scale(const vector2 &_Vector) {

m[0][0]=_Vector.x;
m[1][1]=_Vector.y;
}

matrix3 matrix3::scale_matrix(const vector2 &_Vector) const {

matrix3 _Matrix3; //IDENTITY matrix
_Matrix3.m[0][0]=_Vector.x;
_Matrix3.m[1][1]=_Vector.y;
return _Matrix3;
}

vector2 matrix3::scale_vector() const {
	return vector2(m[0][0],m[1][1]);
}

//Transformation

void matrix3::transform(const vector2 &_Position,const vector2 &_Scale,
	float _Angle) {

//Right-hand rotation CCW (OpenGL)
//cos, -sin
//sin, cos

//Left-hand rotation CW (Direct3D)
// cos, sin
//-sin, cos

matrix3 _Rotation_matrix(math::cosine(_Angle),-math::sine(_Angle),0.0f,
						 math::sine(_Angle),math::cosine(_Angle),0.0f,
						 0.0f,0.0f,1.0f); //z axis rotation

*this=_Rotation_matrix*scale_matrix(_Scale);
translation(_Position);

m[2][0]=0.0f; m[2][1]=0.0f; m[2][2]=1.0f; //No projection term
}

//Afine matrix

bool matrix3::affine() const {
	return (m[2][0]==0.0f && m[2][1]==0.0f && m[2][2]==1.0f);
}

matrix3 matrix3::concatenate_affine(const matrix3 &_Matrix3) const {

	if (!affine() || !_Matrix3.affine()) return ZERO; //Matrices must be affine

return matrix3(
	m[0][0]*_Matrix3.m[0][0]+m[0][1]*_Matrix3.m[1][0]+m[0][2],
	m[0][0]*_Matrix3.m[0][1]+m[0][1]*_Matrix3.m[1][1]+m[0][2],
	m[0][0]*_Matrix3.m[0][2]+m[0][1]*_Matrix3.m[1][2]+m[0][2]+m[0][2],
	
	m[1][0]*_Matrix3.m[0][0]+m[1][1]*_Matrix3.m[1][0]+m[1][2],
	m[1][0]*_Matrix3.m[0][1]+m[1][1]*_Matrix3.m[1][1]+m[1][2],
	m[1][0]*_Matrix3.m[0][2]+m[1][1]*_Matrix3.m[1][2]+m[1][2]+m[1][2],
	
	0.0f,
	0.0f,
	1.0f);
}

vector2 matrix3::transform_affine(const vector2 &_Vector) const {

	if (!affine()) return vector2::ZERO; //Matrix must be affine

return vector2(m[0][0]*_Vector.x+m[0][1]*_Vector.y+m[0][2],
	m[1][0]*_Vector.x+m[1][1]*_Vector.y+m[1][2]);
}

} //namespace lunar