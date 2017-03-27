/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: vector2.h
---------------------------------------------------
*/

#ifndef _VECTOR2_
#define _VECTOR2_

namespace lunar {

class vector2 {

private:

	void copy(const vector2 &_Vector);

public:

	union {
	
		struct {
		
		float x;
		float y;
		};
	
	float ptr[2];
	};
	
	static const vector2 ZERO;
	static const vector2 UNIT_X;
	static const vector2 UNIT_Y;
	static const vector2 NEGATIVE_UNIT_X;
	static const vector2 NEGATIVE_UNIT_Y;
	static const vector2 UNIT_SCALE;
	
	//Constructor
	vector2();
	
	//Alternative constructor
	vector2(float _X,float _Y);
	
	//Alternative constructor
	explicit vector2(float _Scalar);
	
	//Copy constructor
	vector2(const vector2 &_Vector);
	
	//Destructor
	~vector2();
	
	vector2 &operator=(float _Scalar);
	vector2 &operator=(const vector2 &_Vector);
	
	bool operator==(const vector2 &_Vector) const;
	bool operator!=(const vector2 &_Vector) const;
	
	vector2 operator-() const;
	
	vector2 operator+(float _Scalar) const;
	vector2 operator+(const vector2 &_Vector) const;
	
	vector2 operator-(float _Scalar) const;
	vector2 operator-(const vector2 &_Vector) const;
	
	vector2 operator*(float _Scalar) const;
	vector2 operator*(const vector2 &_Vector) const;
	
	vector2 operator/(float _Scalar) const;
	vector2 operator/(const vector2 &_Vector) const;
	
	vector2 &operator+=(float _Scalar);
	vector2 &operator+=(const vector2 &_Vector);
	
	vector2 &operator-=(float _Scalar);
	vector2 &operator-=(const vector2 &_Vector);
	
	vector2 &operator*=(float _Scalar);
	vector2 &operator*=(const vector2 &_Vector);
	
	vector2 &operator/=(float _Scalar);
	vector2 &operator/=(const vector2 &_Vector);
	
	//Return a vector that deviates from this vector by a given angle
	vector2 deviant(float _Angle) const;
	
	//Return the middle point from this and the given vector
	vector2 midpoint(const vector2 &_Vector) const;
	
	//Return an orthogonal vector with congruent adjacent angles
	vector2 perpendicular() const;
	
	//Return a reflection vector to the plane with the given normal
	vector2 reflect(const vector2 &_Normal) const;
	
	//Return a vector maximum of this and the given vector
	vector2 ceil(const vector2 &_Vector) const;
	
	//Return a vector minimum of this and the given vector
	vector2 floor(const vector2 &_Vector) const;
	
	//Return a vector based on lerp between this and the given vector
	vector2 lerp(const vector2 &_Vector,float _Amount) const;
	
	//Return a vector based on slerp between this and the given vector
	vector2 slerp(vector2 _Vector,float _Amount) const;
	
	//Normalize and return the length of the vector
	float normalize();
	
	//Return the normalized vector as a copy
	vector2 normalize_copy() const;
	
	//Return the dot product between this and the given vector
	float dot_product(const vector2 &_Vector) const;

	//Return the cross product between this and the given vector
	float cross_product(const vector2 &_Vector) const;

	//Return the angle between this and the given vector
	float angle(const vector2 &_Vector) const;
	
	//Return the length of the vector (expensive square root)
	float length() const;
	
	//Return the squared length of the vector (for distance comparison)
	float squared_length() const;
	
	//Returns if this vector is approximately zero in length
	bool zero_length() const;
	
	//Returns the minimum component of the vector
	float minimum() const;
	
	//Returns the maximum component of the vector
	float maximum() const;
};

} //namespace lunar

#endif