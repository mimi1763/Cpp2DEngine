/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: table.h
---------------------------------------------------
*/

#ifndef _TABLE_
#define _TABLE_

#include <string>

namespace lunar {

//Forward declarations
class resource_manager;
class connector;

class table {

protected:

	std::string _Name;
	std::string _Name_column;
	
	void copy(const table &_Table);

public:

	//Constructor
	explicit table(const std::string &_Name);
	
	//Copy constructor
	table(const table &_Table);
	
	//Virtual destructor
	virtual ~table();
	
	table &operator=(const table &_Table);
	
	virtual bool load(connector *_Connector)=0;
	
	void name(const std::string &_Name);
	const std::string &name() const;
	
	void columns(const std::string &_Name_column);
	const std::string &name_column() const;
};

} //namespace lunar

#endif