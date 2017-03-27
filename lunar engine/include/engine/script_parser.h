/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: script_parser.h
---------------------------------------------------
*/

#ifndef _SCRIPT_PARSER_
#define _SCRIPT_PARSER_

#include "script_class.h"
#include "script_scope.h"

#include "file_container.h"

namespace lunar {

//--------------------------------
//Script file syntax
//--------------------------------
//
//	class <name>
//	{
//		property1 <parameters>
//		property2 <parameters>
//		
//		class <name>
//		{
//			property1 <parameters>
//			porperty2 <parameters>
//		}
//	}
//	
//	class <name>
//	{
//		property1 <parameters>
//		property2 <parameters>
//	}
//
//--------------------------------

class script_parser {

public:

	struct result {
	
		friend script_parser;
	
	public:
	
		enum TYPE { TREE, LINEAR };
	
	private:
	
		TYPE _Type;
		bool _Merge;
		std::string _Script_name;
		
		script_class::object::_Objects _Myobjects;
		
		void copy(const result &_Result);
	
	public:
	
		//Constructor
		explicit result(const TYPE &_Type=TREE,bool _Merge=false);
		
		//Copy constructor
		result(const result &_Result);
		
		//Destructor
		~result();
		
		result &operator=(const result &_Result);
		
		TYPE type() const;
		bool merge() const;
		const std::string &script_name() const;
		
		script_class::object::objects_iterator objects_begin() const;
		script_class::object::objects_iterator objects_end() const;
	};

private:

	typedef std::list<script_class*> _Classes;
	typedef std::list<script_scope*> _Scopes;
	
	script_class *_Global_class;	
	_Classes _Myclasses;
	_Scopes _Myscopes;
	
	file_container *_Script_container;
	
	void copy(const script_parser &_Script_parser);
	
	bool parse(std::string _Script_data,result &_Result) const;
	
	static void compile_classes(script_class *_Script_class,
		script_class::_Classes &_Myclasses);

	static void compile_scopes(script_class *_Script_class,
		script_class::_Scopes &_Myscopes);

	static void compile_scopes(script_class *_Script_class,
		script_class::_Scopes &_Myscopes,
			_Classes &_Inherited_classes);

	static void compile_properties(script_class *_Script_class,
		script_class::_Properties &_Myproperties);
	
	static void compile_properties(script_class *_Script_class,
		script_class::_Properties &_Myproperties,
			_Classes &_Inherited_classes);
	
	static void merge_objects(script_class::object::_Objects &_Myobjects);
	static void cleanup_objects(script_class::object::_Objects &_Myobjects);
	static void linearize(const script_class::object::_Objects &_Source,
		script_class::object::_Objects &_Destination);

public:

	typedef _Classes::const_iterator classes_iterator; //Keep data safe
	typedef _Scopes::const_iterator scopes_iterator; //Keep data safe
	
	//Constructor
	script_parser();
	
	//Copy constructor
	script_parser(const script_parser &_Script_parser);
	
	//Destructor
	~script_parser();
	
	script_parser &operator=(const script_parser &_Script_parser);
	
	bool parse_file(const std::string &_Filename,result &_Result) const;
	bool parse_script(const std::string &_Script_name,result &_Result) const;
	bool parse_data(const std::string &_Script_data,result &_Result) const;
	bool parse_data(const std::string &_Script_data,
		const std::string &_Script_name,result &_Result) const;
	
	void script_source(file_container *_Script_container);
	file_container *script_source() const;
	
	//Classes
	
	script_class *create_class(const std::string &_Name);
	script_class *create_class(const std::string &_Name,
		const script_class &_Script_class);
	
	void remove_class(script_class *&_Script_class);
	void clear_classes();
	
	classes_iterator classes_begin() const;
	classes_iterator classes_end() const;
	
	script_class *class_pointer(const std::string &_Name) const;
	
	//Scopes
	
	script_scope *create_scope(const std::string &_Name);
	script_scope *create_scope(const std::string &_Name,
		const script_scope &_Script_scope);
	
	void remove_scope(script_scope *&_Script_scope);
	void clear_scopes();
	
	scopes_iterator scopes_begin() const;
	scopes_iterator scopes_end() const;
	
	script_scope *scope_pointer(const std::string &_Name) const;
	
	//Registration
	
	void register_scope(script_scope *_Script_scope);
	void unregister_scope(script_scope *_Script_scope);
	
	void unregister_scopes();
};

} //namespace lunar

#endif