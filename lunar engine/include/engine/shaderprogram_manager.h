/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shaderprogram_manager.h
---------------------------------------------------
*/

#ifndef _SHADERPROGRAM_MANAGER_
#define _SHADERPROGRAM_MANAGER_

#include "manager.h"
#include "shaderprogram.h"

namespace lunar {

//Forward declarations
class core;
class shader_manager;
class texture_manager;
class animation_manager;
class script_parser;

class shaderprogram_manager:public manager {

	friend core;

private:

	static shaderprogram_manager *_Instance; //Grant core exclusive access

protected:

	file_container *_Script_container;
	shader_manager *_Shader_manager;
	texture_manager *_Texture_manager;
	animation_manager *_Animation_manager;
	
	script_parser *_Script_parser;
	
	void copy(const shaderprogram_manager &_Shaderprogram_manager);
	
	virtual void removed(manageable *_Manageable);
	
	//GL program/script specific
	static void init_particlescript(script_parser *_Script_parser);
	
	bool init_shaderprogram(shaderprogram *_Shaderprogram,
		file_data *_Script_data);
		
	static void save_shaderprogramlog(unsigned int _Glprogram);
	
	//Shader variable specific
	static bool init_uniform(uniform *_Uniform,shaderprogram *_Shaderprogram);
	static bool init_attribute(attribute *_Attribute,shaderprogram *_Shaderprogram);
	static bool init_sampler(sampler *_Sampler,shaderprogram *_Shaderprogram);
	
	void update_uniforms(shaderprogram *_Shaderprogram) const;
	void update_attributes(shaderprogram *_Shaderprogram) const;
	void update_samplers(shaderprogram *_Shaderprogram) const;

public:

	//Constructor
	shaderprogram_manager();
	
	//Alternative constructor
	explicit shaderprogram_manager(shader_manager *_Shader_manager);
	
	//Alternative constructor
	shaderprogram_manager(shader_manager *_Shader_manager,
		texture_manager *_Texture_manager,animation_manager *_Animation_manager);
	
	//Copy constructor
	shaderprogram_manager(const shaderprogram_manager& _Shaderprogram_manager);
	
	//Virtual destructor
	virtual ~shaderprogram_manager();
	
	shaderprogram_manager &operator=(const shaderprogram_manager &_Shaderprogram_manager);
	
	static shaderprogram_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void update_shadervariables(shaderprogram *_Shaderprogram);
	
	void script_source(file_container *_Script_container);
	file_container *script_source() const;
	
	//Shaderprogram functions
	
	shaderprogram *create_shaderprogram(const std::string &_Script_name,
		bool _Secured=false);
	
	shaderprogram *create_shaderprogram(const file_data &_Script_data,
		bool _Secured=false);
	
	shaderprogram *create_shaderprogram(file_data *_Script_data,
		bool _Secured=false);
	
	shaderprogram *create_shaderprogram(const shaderprogram &_Shaderprogram,
		bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif