/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_container.h
---------------------------------------------------
*/

#ifndef _SHADER_CONTAINER_
#define _SHADER_CONTAINER_

#include "file_container.h"

namespace lunar {

class shader_data:public file_data {

public:

	enum TYPE { VERTEX,FRAGMENT };

protected:

	TYPE _Type;
	
	void copy(const shader_data &_Shader_data);

public:

	//Constructor
	shader_data(const std::string &_Name,const std::string &_Filename,
		const TYPE &_Type=VERTEX);
	
	//Copy constructor
	shader_data(const shader_data &_Shader_data);
	
	//Virtual destructor
	virtual ~shader_data();
	
	shader_data &operator=(const shader_data &_Shader_data);
	
	void type(const TYPE &_Type);
	TYPE type() const;
};

class shader_container:public file_container {

	friend resource_manager;

private:

	static shader_container *_Instance; //Grant resource_manager exclusive access

protected:
	
	void copy(const shader_container &_Shader_container);

public:

	//Constructor
	shader_container();
	
	//Copy constructor
	shader_container(const shader_container &_Shader_container);
	
	//Virtual destructor
	virtual ~shader_container();
	
	shader_container &operator=(const shader_container &_Shader_container);
	
	static shader_container *instance();
	
	shader_data *add_data(const shader_data &_Shader_data);
};

} //namespace lunar

#endif