/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: generic_utility.h
---------------------------------------------------
*/

#ifndef _GENERIC_UTILITY_
#define _GENERIC_UTILITY_

#include <string>
#include "rectangle.h"

namespace lunar {

//Forward declarations
class render_node;

class utility {

public:

	enum DUMP_MODE { NODES_ONLY, PROPERTIES_IN_LIST, PROPERTIES_IN_NODE };

private:

	//Private constructor
	utility();
	
	//Private destructor
	~utility();
	
	//Other
	
	static std::string attachable_type(attachable *_Attachable);
	static std::string manageable_data(manageable *_Manageable);

public:

	//Conversions
	
	static std::string base(unsigned int _Value,unsigned int _Base=16);
	
	//Move position to new origin
	//without changing rectangle origin
	//CENTER (x) -> TOP_LEFT (y)
	//  -------
	// |       |
	// |   x---|---
	// |   | \ |   |
	//  -------y   |
	//     |       |
	//      -------
	static vector2 position(const vector2 &_Position,float _Width,
		float _Height,const rectangle::ORIGIN &_From,
			const rectangle::ORIGIN &_To=rectangle::CENTER);
	
	//Move position to new origin
	//without moving rectangle
	//CENTER (x) -> TOP_LEFT (y)
	// y-------
	// | \     |
	// |   x   |
	// |       |
	//  -------
	static vector2 origin(const vector2 &_Position,float _Width,
		float _Height,const rectangle::ORIGIN &_From,
			const rectangle::ORIGIN &_To=rectangle::CENTER);
	
	//Other
	
	static bool dump_graph(const std::string &_Filename,
		render_node *_Parent_node=0,const DUMP_MODE &_Mode=PROPERTIES_IN_LIST);
};

} //namespace lunar

#endif