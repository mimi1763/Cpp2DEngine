/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_manager.h
---------------------------------------------------
*/

#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_

#include "manager.h"
#include "shader.h"

namespace lunar {

//Forward declarations
class core;

class shader_manager:public manager {

	friend core;

private:

	static shader_manager *_Instance; //Grant core exclusive access

protected:

	struct _Created_glshader {
	
	shader_data *_Shader_data;
	unsigned int _Glshader;
	};
	
	typedef std::list<_Created_glshader> _Created_glshaders;
	
	_Created_glshaders _Mycreated_glshaders;
	shader_container *_Shader_container;
	
	void copy(const shader_manager &_Shader_manager);
	
	//GL shader specific
	bool create_glshader(shader_data *_Shader_data);
	static void init_shader(shader *_Shader,const _Created_glshader &_Mycreated_glshader);
	
	static void save_shaderlog(unsigned int _Glshader);

public:

	//Constructor
	shader_manager();
	
	//Copy constructor
	shader_manager(const shader_manager &_Shader_manager);
	
	//Virtual destructor
	virtual ~shader_manager();
	
	shader_manager &operator=(const shader_manager &_Shader_manager);
	
	static shader_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void shader_source(shader_container *_Shader_container);
	shader_container *shader_source() const;
	
	//Shader functions
	
	shader *create_shader(const std::string &_Name,bool _Secured=false);
	shader *create_shader(shader_data *_Shader_data,bool _Secured=false);
	shader *create_shader(const shader_data &_Shader_data,bool _Secured=false);
	shader *create_shader(const shader &_Shader,bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif