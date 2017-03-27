/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_table.h
---------------------------------------------------
*/

#ifndef _SOUND_TABLE_
#define _SOUND_TABLE_

#include "file_table.h"

namespace lunar {

//Forward declarations
class sound_container;

class sound_table:public file_table {

	friend resource_manager;

private:

	static sound_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Stream_column;
	std::string _Software_column;
	std::string _Positional_column;
	std::string _Loop_column;
	
	sound_container *_Sound_container;
	
	void copy(const sound_table &_Sound_table);

public:

	//Constructor
	explicit sound_table(sound_container *_Sound_container);
	
	//Copy constructor
	sound_table(const sound_table &_Sound_table);
	
	//Virtual destructor
	virtual ~sound_table();
	
	sound_table &operator=(const sound_table &_Sound_table);
	
	static sound_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Stream_column,const std::string &_Software_column,
		const std::string &_Positional_column,const std::string &_Loop_column);
	
	const std::string &stream_column() const;
	const std::string &software_column() const;
	const std::string &positional_column() const;
	const std::string &loop_column() const;
};

} //namespace lunar

#endif