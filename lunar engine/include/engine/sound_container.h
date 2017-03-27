/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_container.h
---------------------------------------------------
*/

#ifndef _SOUND_CONTAINER_
#define _SOUND_CONTAINER_

#include "file_container.h"

namespace lunar {

class sound_data:public file_data {

protected:

	bool _Stream;
	bool _Software;
	bool _Positional;
	bool _Loop;
	
	void copy(const sound_data &_Sound_data);

public:

	//Constructor
	sound_data(const std::string &_Name,const std::string &_Filename,
		bool _Stream=false,bool _Software=false,
			bool _Positional=false,bool _Loop=false);
	
	//Copy constructor
	sound_data(const sound_data &_Sound_data);
	
	//Virtual destructor
	virtual ~sound_data();
	
	sound_data &operator=(const sound_data &_Sound_data);
	
	void stream(bool _Stream);
	bool stream() const;
	
	void software(bool _Software);
	bool software() const;
	
	void positional(bool _Positional);
	bool positional() const;
	
	void loop(bool _Loop);
	bool loop() const;
};

class sound_container:public file_container {

	friend resource_manager;

private:

	static sound_container *_Instance; //Grant resource_manager exclusive access

protected:
	
	void copy(const sound_container &_Sound_container);

public:

	//Constructor
	sound_container();
	
	//Copy constructor
	sound_container(const sound_container &_Sound_container);
	
	//Virtual destructor
	virtual ~sound_container();
	
	sound_container &operator=(const sound_container &_Sound_container);
	
	static sound_container *instance();
	
	sound_data *add_data(const sound_data &_Sound_data);
};

} //namespace lunar

#endif