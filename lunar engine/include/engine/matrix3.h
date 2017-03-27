/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: matrix3.h
---------------------------------------------------
*/

#ifndef _MATRIX3_
#define _MATRIX3_

#include "vector2.h"

namespace lunar {

class matrix3 {

private:

	void copy(const matrix3 &_Matrix3);

public:

	//Can be accessed as either one or two dimensional array
	union {
	
	float m[3][3];
	float ptr[9];
	};
	
	static const matrix3 ZERO;
	static const matrix3 IDENTITY;
	
	//Constructor
	matrix3();
	
	//Alternative constructor
	matrix3(float _M00,float _M01,float _M02,
			float _M10,float _M11,float _M12,
			float _M20,float _M21,float _M22);
	
	//Copy constructor
	matrix3(const matrix3 &_Matrix3);
	
	//Destructor
	~matrix3();
	
	matrix3 &operator=(const matrix3 &_Matrix3);
	
	bool operator==(const matrix3 &_Matrix3) const;
	bool operator!=(const matrix3 &_Matrix3) const;
	
	matrix3 operator*(const matrix3 &_Matrix3) const;
	matrix3 operator*(float _Scalar) const;
	vector2 operator*(const vector2 &_Vector) const;
	
	matrix3 operator+(const matrix3 &_Matrix3) const;
	matrix3 operator+(float _Scalar) const;
	
	matrix3 operator-(const matrix3 &_Matrix3) const;
	matrix3 operator-(float _Scalar) const;
	
	matrix3 concatenate(const matrix3 &_Matrix3) const;
	matrix3 transpose() const;
	
	//Not implemented yet
	//matrix3 adjoint() const;
	//matrix3 inverse() const;
	//float determinant() const;
	
	//Translation transformation
	
	void translation(const vector2 &_Vector);
	
	matrix3 translation_matrix(const vector2 &_Vector) const;
	vector2 translation_vector() const;
	
	//Scale transformation
	
	void scale(const vector2 &_Vector);
	
	matrix3 scale_matrix(const vector2 &_Vector) const;
	vector2 scale_vector() const;
	
	//Transformation
	
	void transform(const vector2 &_Position,const vector2 &_Scale,
		float _Angle);
	
	//Afine matrix
	//A matrix with last row equal to (0,0,1)
	
	bool affine() const;
	
	matrix3 concatenate_affine(const matrix3 &_Matrix3) const;
	vector2 transform_affine(const vector2 &_Vector) const;
};

} //namespace lunar

#endif