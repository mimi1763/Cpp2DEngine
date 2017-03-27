/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_class.h
---------------------------------------------------
*/

#ifndef _SCRIPT_CLASS_
#define _SCRIPT_CLASS_

#include <list>
#include "script_property.h"

namespace lunar {

//Forward declarations
class script_parser;
class script_scope;

class script_class {

	friend script_parser;

public:

	//A struct that describes a class object
	struct object {
	
		friend script_parser;
	
	private:
	
		typedef std::list<object> _Objects;
		typedef std::list<script_property::data> _Data;
		
		std::string _Class_name;
		std::string _Name;
		
		std::string _Tag;
		std::string _Parent;
		unsigned int _Depth;
		bool _Templated;
		
		_Objects _Myobjects;
		_Data _Mydata;
		
		void copy(const object &_Object);
		void copy_linear(const object &_Object);
		
		void append(const object &_Object);
		void append(const _Objects &_Myobjects);
		
		void append_front(const _Objects &_Myobjects);
	
	public:
	
		typedef _Objects::const_iterator objects_iterator; //Keep data safe
		typedef _Data::const_iterator data_iterator; //Keep data safe
		
		//Constructor
		object();
		
		//Copy constructor
		object(const object &_Object);
		
		//Destructor
		~object();
		
		object &operator=(const object &_Object);
		
		bool operator==(const std::string &_Class_name) const;
		bool operator!=(const std::string &_Class_name) const;
		
		const std::string &class_name() const;
		const std::string &name() const;
		
		const std::string &tag() const;
		const std::string &parent() const;
		unsigned int depth() const;
		bool templated() const;
		
		objects_iterator objects_begin() const;
		objects_iterator objects_end() const;
		
		data_iterator data_begin() const;
		data_iterator data_end() const;
	};

private:

	typedef std::list<script_property*> _Properties;
	typedef std::list<script_scope*> _Scopes;
	typedef std::list<script_class*> _Classes;
	
	script_parser *_Owner;
	std::string _Name;
	std::string _Tag;
	_Properties _Myproperties;
	
	_Scopes _Myregistered_scopes;
	_Classes _Myregistered_inheritances;
	
	//Private constructor
	script_class(script_parser *_Owner,const std::string &_Name);
	
	//Private copy constructor
	script_class(const script_class &_Script_class);
	
	//Private alternative copy constructor
	script_class(script_parser *_Owner,const std::string &_Name,
		const script_class &_Script_class);
	
	//Private destructor
	~script_class();
	
	void copy(const script_class &_Script_class);

public:

	typedef _Properties::const_iterator properties_iterator; //Keep data safe
	
	script_class &operator=(const script_class &_Script_class);
	
	script_parser *owner() const;
	const std::string &name() const;
	
	void tag(const std::string &_Tag);
	const std::string &tag() const;
	
	//Properties
	
	script_property *create_property(const std::string &_Name);
	script_property *create_property(const std::string &_Name,
		const script_property &_Script_property);
	
	void remove_property(script_property *&_Script_property);
	void clear_properties();
	
	properties_iterator properties_begin() const;
	properties_iterator properties_end() const;
	
	script_property *property_pointer(const std::string &_Name) const;
	
	//Registration
	
	void register_scope(script_scope *_Script_scope);
	void unregister_scope(script_scope *_Script_scope);
	
	void unregister_scopes();
	
	void register_inheritance(script_class *_Script_class);
	void unregister_inheritance(script_class *_Script_class);
	
	void unregister_inheritances();
};

} //namespace lunar

#endif