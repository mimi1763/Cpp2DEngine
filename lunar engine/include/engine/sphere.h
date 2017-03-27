/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sphere.h
---------------------------------------------------
*/

#ifndef _SPHERE_
#define _SPHERE_

#include "vector2.h"

namespace lunar {

class sphere {

private:

	float _Radius;
	vector2 _Center;
	
	void copy(const sphere &_Sphere);

public:

	//Constructor
	sphere();
	
	//Alternative constructor
	sphere(float _Radius,const vector2 &_Center=vector2::ZERO);
	
	//Copy constructor
	sphere(const sphere &_Sphere);
	
	//Destructor
	~sphere();
	
	sphere &operator=(const sphere &_Sphere);
	
	bool operator==(const sphere &_Sphere) const;
	bool operator!=(const sphere &_Sphere) const;
	
	bool intersects(const sphere &_Sphere) const;
	bool intersects(const vector2 &_Vector) const;
	
	bool contains(const sphere &_Sphere) const;
	bool contains(const vector2 &_Vector) const;
	
	void radius(float _Radius);
	float radius() const;
	
	void diameter(float _Diameter);
	float diameter() const;
	
	void center(const vector2 &_Center);
	const vector2 &center() const;
	
	float area() const;
	float circumference() const;
	
	#ifdef _DEBUG
	void draw() const;
	#endif
};

} //namespace lunar

#endif