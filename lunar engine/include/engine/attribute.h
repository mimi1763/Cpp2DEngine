/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: attribute.h
---------------------------------------------------
*/

#ifndef _ATTRIBUTE_
#define _ATTRIBUTE_

#include "shadervariable.h"

namespace lunar {

class attribute:public shadervariable {

	friend shaderprogram;
	friend shaderprogram_manager;

protected:

	unsigned int _Stride;
	bool _Normalized;
	
	//Protected constructor
	explicit attribute(const std::string &_Name,unsigned int _Components=1,
		unsigned int _Stride=0,bool _Normalized=false);
	
	//Protected constructor (int)
	attribute(const std::string &_Name,int *_Data,
		unsigned int _Components=1,unsigned int _Stride=0,
			bool _Normalized=false);
	
	//Protected constructor (float)
	attribute(const std::string &_Name,float *_Data,
		unsigned int _Components=1,unsigned int _Stride=0,
			bool _Normalized=false);
	
	//Protected copy constructor
	attribute(const attribute &_Attribute);
	
	//Protected virtual destructor
	virtual ~attribute();
	
	void copy(const attribute &_Attribute);

public:

	attribute &operator=(const attribute &_Attribute);
	
	void stride(unsigned int _Stride);
	unsigned int stride() const;
	
	void normalized(bool _Normalized);
	bool normalized() const;
};

} //namespace lunar

#endif