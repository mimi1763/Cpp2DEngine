/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_container.h
---------------------------------------------------
*/

#ifndef _ANIMATION_CONTAINER_
#define _ANIMATION_CONTAINER_

#include "file_container.h"

namespace lunar {

class animation_data:public file_data {

protected:

	bool _Stream;
	
	void copy(const animation_data &_Animation_data);

public:

	//Constructor
	explicit animation_data(const std::string &_Name,const std::string &_Filename,
		bool _Stream=false);
	
	//Copy constructor
	animation_data(const animation_data &_Animation_data);
	
	//Virtual destructor
	virtual ~animation_data();
	
	animation_data &operator=(const animation_data &_Animation_data);
	
	void stream(bool _Stream);
	bool stream() const;
};

class animation_container:public file_container {

	friend resource_manager;

private:

	static animation_container *_Instance; //Grant resource_manager exclusive access

protected:

	void copy(const animation_container &_Animation_container);

public:

	//Constructor
	animation_container();
	
	//Copy constructor
	animation_container(const animation_container &_Animation_container);
	
	//Virtual destructor
	virtual ~animation_container();
	
	animation_container &operator=(const animation_container &_Animation_container);
	
	static animation_container *instance();
	
	animation_data *add_data(const animation_data &_Animation_data);
};

} //namespace lunar

#endif