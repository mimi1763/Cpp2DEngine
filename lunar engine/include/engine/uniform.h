/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: uniform.h
---------------------------------------------------
*/

#ifndef _UNIFORM_
#define _UNIFORM_

#include "shadervariable.h"

namespace lunar {

class uniform:public shadervariable {

	friend shaderprogram;
	friend shaderprogram_manager;

protected:

	unsigned int _Size;
	
	//Protected constructor
	explicit uniform(const std::string &_Name,unsigned int _Components=1,
		unsigned int _Size=1);
	
	//Protected constructor (int)
	uniform(const std::string &_Name,int *_Data,
		unsigned int _Components=1,unsigned int _Size=1);
	
	//Protected constructor (float)
	uniform(const std::string &_Name,float *_Data,
		unsigned int _Components=1,unsigned int _Size=1);
	
	//Protected copy constructor
	uniform(const uniform &_Uniform);
	
	//Protected virtual destructor
	virtual ~uniform();
	
	void copy(const uniform &_Uniform);

public:

	uniform &operator=(const uniform &_Uniform);
	
	void size(unsigned int _Size);
	unsigned int size() const;
};

} //namespace lunar

#endif