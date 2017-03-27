/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_scope.h
---------------------------------------------------
*/

#ifndef _SCRIPT_SCOPE_
#define _SCRIPT_SCOPE_

#include <list>

namespace lunar {

//Forward declarations
class script_parser;
class script_class;

class script_scope {

	friend script_parser;

private:

	typedef std::list<script_class*> _Classes;
	
	script_parser *_Owner;
	std::string _Name;
	
	_Classes _Myregistered_classes;
	
	//Private constructor
	script_scope(script_parser *_Owner,const std::string &_Name);
	
	//Private copy constructor
	script_scope(const script_scope &_Script_scope);
	
	//Private alternative copy constructor
	script_scope(script_parser *_Owner,const std::string &_Name,
		const script_scope &_Script_scope);
	
	//Private destructor
	~script_scope();
	
	void copy(const script_scope &_Script_scope);

public:

	script_scope &operator=(const script_scope &_Script_scope);
	
	script_parser *owner() const;
	const std::string &name() const;
	
	//Registration
	
	void register_class(script_class *_Script_class);
	void unregister_class(script_class *_Script_class);
	
	void unregister_classes();
};

} //namespace lunar

#endif