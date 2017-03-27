/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_table.h
---------------------------------------------------
*/

#ifndef _TEXTURE_TABLE_
#define _TEXTURE_TABLE_

#include "file_table.h"

namespace lunar {

//Forward declarations
class texture_container;

class texture_table:public file_table {

	friend resource_manager;

private:

	static texture_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Tileable_column;
	
	texture_container *_Texture_container;
	
	void copy(const texture_table &_Texture_table);

public:

	//Constructor
	explicit texture_table(texture_container *_Texture_container);
	
	//Copy constructor
	texture_table(const texture_table &_Texture_table);
	
	//Virtual destructor
	virtual ~texture_table();
	
	texture_table &operator=(const texture_table &_Texture_table);
	
	static texture_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Tileable_column);
	const std::string &tileable_column() const;
};

} //namespace lunar

#endif