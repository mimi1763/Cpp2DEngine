/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_container.h
---------------------------------------------------
*/

#ifndef _TEXTURE_CONTAINER_
#define _TEXTURE_CONTAINER_

#include "file_container.h"

namespace lunar {

class texture_data:public file_data {

protected:

	bool _Tileable;
	
	void copy(const texture_data &_Texture_data);

public:

	//Constructor
	texture_data(const std::string &_Name,const std::string &_Filename,
		bool _Tileable=false);
	
	//Copy constructor
	texture_data(const texture_data &_Texture_data);
	
	//Virtual destructor
	virtual ~texture_data();
	
	texture_data &operator=(const texture_data &_Texture_data);
	
	void tileable(bool _Tileable);
	bool tileable() const;
};

class texture_container:public file_container {

	friend resource_manager;

private:

	static texture_container *_Instance; //Grant resource_manager exclusive access

protected:
	
	void copy(const texture_container &_Texture_container);

public:

	//Constructor
	texture_container();
	
	//Copy constructor
	texture_container(const texture_container &_Texture_container);
	
	//Virtual destructor
	virtual ~texture_container();
	
	texture_container &operator=(const texture_container &_Texture_container);
	
	static texture_container *instance();
	
	texture_data *add_data(const texture_data &_Texture_data);
};

} //namespace lunar

#endif