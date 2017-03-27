/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: aabb.h
---------------------------------------------------
*/

#ifndef _AABB_
#define _AABB_

#include "matrix3.h"

namespace lunar {

class aabb {

private:

	//Extents:
	//
	//      max
	// x-----x
	// |     |
	// |     |
	// x-----x
	//min
	
	vector2 _Min;
	vector2 _Max;
	bool _Empty;
	
	void copy(const aabb &_Aabb);

public:

	//Constructor
	aabb();
	
	//Alternative constructor
	aabb(const vector2 &_Min,const vector2 &_Max);
	
	//Copy constructor
	aabb(const aabb &_Aabb);
	
	//Destructor
	~aabb();
	
	aabb &operator=(const aabb &_Aabb);
	
	bool operator==(const aabb &_Aabb) const;
	bool operator!=(const aabb &_Aabb) const;
	
	void extents(const vector2 &_Min,const vector2 &_Max);
	
	void scale(const vector2 &_Scale);
	void transform_affine(const matrix3 &_Matrix3);
	
	bool intersects(const aabb &_Aabb) const;
	bool intersects(const vector2 &_Vector) const;
	aabb intersection(const aabb &_Aabb) const;
	
	bool contains(const aabb &_Aabb) const;
	bool contains(const vector2 &_Vector) const;
		
	void merge(const aabb &_Aabb);
	void merge(const vector2 &_Vector);
	
	void clear();
	bool empty() const;
	
	const vector2 &minimum() const;
	const vector2 &maximum() const;
	
	vector2 center() const;
	
	vector2 size() const;
	vector2 half_size() const;
	
	float area() const;
	
	#ifdef _DEBUG
	void draw() const;
	#endif
};

} //namespace lunar

#endif