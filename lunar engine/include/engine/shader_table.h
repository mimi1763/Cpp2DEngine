/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_table.h
---------------------------------------------------
*/

#ifndef _SHADER_TABLE_
#define _SHADER_TABLE_

#include "file_table.h"

namespace lunar {

//Forward declarations
class shader_container;

class shader_table:public file_table {

	friend resource_manager;

private:

	static shader_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Type_column;
	shader_container *_Shader_container;
	
	void copy(const shader_table &_Shader_table);

public:

	//Constructor
	explicit shader_table(shader_container *_Shader_container);
	
	//Copy constructor
	shader_table(const shader_table &_Shader_table);
	
	//Virtual destructor
	virtual ~shader_table();
	
	shader_table &operator=(const shader_table &_Shader_table);
	
	static shader_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Type_column);
	const std::string &type_column() const;
};

} //namespace lunar

#endif