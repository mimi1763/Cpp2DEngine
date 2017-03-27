/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_table.h
---------------------------------------------------
*/

#ifndef _ANIMATION_TABLE_
#define _ANIMATION_TABLE_

#include "file_table.h"

namespace lunar {

//Forward declarations
class animation_container;

class animation_table:public file_table {

	friend resource_manager;

private:

	static animation_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Stream_column;
	
	animation_container *_Animation_container;
	
	void copy(const animation_table &_Animation_table);

public:

	//Constructor
	explicit animation_table(animation_container *_Animation_container);
	
	//Copy constructor
	animation_table(const animation_table &_Animation_table);
	
	//Virtual destructor
	virtual ~animation_table();
	
	animation_table &operator=(const animation_table &_Animation_table);
	
	static animation_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Stream_column);
	const std::string &stream_column() const;
};

} //namespace lunar

#endif