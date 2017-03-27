/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: file_table.h
---------------------------------------------------
*/

#ifndef _FILE_TABLE_
#define _FILE_TABLE_

#include "table.h"

namespace lunar {

//Forward declarations
class file_container;

class file_table:public table {

	friend resource_manager;

private:

	static file_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Filename_column;
	file_container *_File_container;
	
	void copy(const file_table &_File_table);

public:

	//Constructor
	explicit file_table(const std::string &_Name,
		file_container *_File_container=0);
	
	//Copy constructor
	file_table(const file_table &_File_table);
	
	//Virtual destructor
	virtual ~file_table();
	
	file_table &operator=(const file_table &_File_table);
	
	static file_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Filename_column);
	const std::string &filename_column() const;
};

} //namespace lunar

#endif