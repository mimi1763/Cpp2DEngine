/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: obb.h
---------------------------------------------------
*/

#ifndef _OBB_
#define _OBB_

#include "aabb.h"

namespace lunar {

class obb {

private:

	//Corners:
	//
	// 4     3
	// x-----x
	// |     |
	// |     |
	// x-----x
	// 1     2
	
	vector2 _Corners[4];
	vector2 _Size;
	
	vector2 _Axis[2];
	vector2 _Origin;
	
	bool _Empty;
	
	void copy(const obb &_Obb);
	
	bool intersects_oneway(const obb &_Obb) const;

public:

	//Constructor
	obb();
	
	//Alternative constructor
	obb(const vector2 &_Bottom_left,const vector2 &_Bottom_right,
		const vector2 &_Top_right,const vector2 &_Top_left,
			const vector2 &_Size);
	
	//Alternative constructor
	obb(const aabb &_Aabb);
	
	//Copy constructor
	obb(const obb &_Obb);
	
	//Destructor
	~obb();
	
	obb &operator=(const obb &_Obb);
	obb &operator=(const aabb &_Aabb);
	
	bool operator==(const obb &_Obb) const;
	bool operator!=(const obb &_Obb) const;
	
	void transform_affine(const matrix3 &_Matrix3);
	
	bool intersects(const obb &_Obb) const;
	bool intersects(const vector2 &_Vector) const;
	
	void clear();
	bool empty() const;
	
	void corners(const vector2 &_Bottom_left,const vector2 &_Bottom_right,
		const vector2 &_Top_right,const vector2 &_Top_left,
			const vector2 &_Size);
	const vector2 *corners() const;
	
	vector2 center() const;
	
	const vector2 &size() const;
	vector2 half_size() const;
	
	#ifdef _DEBUG
	void draw() const;
	#endif
};

} //namespace lunar

#endif