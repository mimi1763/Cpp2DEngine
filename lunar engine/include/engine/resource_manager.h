/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: resource_manager.h
---------------------------------------------------
*/

#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include <list>

namespace lunar {

//Forward declarations
class core;

class file_container;
class font_container;
class shader_container;
class sound_container;
class texture_container;
class animation_container;

class table;
class file_table;
class font_table;
class shader_table;
class sound_table;
class texture_table;
class animation_table;

class script_parser;
class connector;

class resource_manager {

	friend core;

private:

	static resource_manager *_Instance; //Grant core exclusive access

private:

	typedef std::list<table*> _Tables;
	
	std::string _Font_extensions;
	std::string _Shader_extensions;
	std::string _Sound_extensions;
	std::string _Texture_extensions;
	std::string _Animation_extensions;
	
	file_container *_File_container;
	font_container *_Font_container;
	shader_container *_Shader_container;
	sound_container *_Sound_container;
	texture_container *_Texture_container;
	animation_container *_Animation_container;
	
	script_parser *_Script_parser;
	
	file_table *_File_table;
	font_table *_Font_table;
	shader_table *_Shader_table;
	sound_table *_Sound_table;
	texture_table *_Texture_table;
	animation_table *_Animation_table;
	
	_Tables _Mytables;
	
	//Private constructor
	resource_manager();
	
	//Private copy constructor
	resource_manager(const resource_manager &_Resource_manager);
	
	//Private destructor
	~resource_manager();
	
	resource_manager &operator=(const resource_manager &_Resource_manager);
	
	void copy(const resource_manager &_Resource_manager);
	
	//Script specific
	static void init_resourcescript(script_parser *_Script_parser);
	
	bool init_resources(const std::string &_Filename);

public:

	typedef _Tables::const_iterator tables_iter; //Keep data safe
	
	//Singleton
	static resource_manager *instance();
	
	bool load(const std::string &_Filename);
	bool load(connector *_Connector);
	
	//Extension functions
	
	void font_extensions(const std::string &_Font_extensions);
	const std::string &font_extensions() const;
	
	void shader_extensions(const std::string &_Shader_extensions);
	const std::string &shader_extensions() const;
	
	void sound_extensions(const std::string &_Sound_extensions);
	const std::string &sound_extensions() const;
	
	void texture_extensions(const std::string &_Texture_extensions);
	const std::string &texture_extensions() const;
	
	void animation_extensions(const std::string &_Animation_extensions);
	const std::string &animation_extensions() const;
	
	//Container functions
	
	void use_containers(file_container *_File_container=0,font_container *_Font_container=0,
		shader_container *_Shader_container=0,sound_container *_Sound_container=0,
			texture_container *_Texture_container=0,animation_container *_Animation_container=0);
	
	//Table functions
	
	void use_tables(file_table *_File_table=0,font_table *_Font_table=0,
		shader_table *_Shader_table=0,sound_table *_Sound_table=0,
			texture_table *_Texture_table=0,animation_table *_Animation_table=0);
	
	void add_table(table *_Table);
	
	void remove_table(table *_Table);
	void clear_tables();
	
	tables_iter tables_begin() const;
	tables_iter tables_end() const;
};

} //namespace lunar

#endif