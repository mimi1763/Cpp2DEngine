/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader.h
---------------------------------------------------
*/

#ifndef _SHADER_
#define _SHADER_

#include "manageable.h"
#include "shader_container.h"

namespace lunar {

//Forward declarations
class shader_manager;

class shader:public manageable {

	friend shader_manager;

protected:

	unsigned int _Glshader;
	
	//Protected constructor
	shader(shader_manager *_Owner,shader_data *_Shader_data);
	
	//Protected copy constructor
	shader(const shader &_Shader);
	
	//Protected alternative copy constructor
	shader(shader_manager *_Owner,const shader &_Shader);
	
	//Protected virtual destructor
	virtual ~shader();
	
	void copy(const shader &_Shader);

public:

	shader &operator=(const shader &_Shader);
	
	void data(shader_data *_Shader_data);
	shader_data *data() const;
	
	unsigned int glshader();
};

} //namespace lunar

#endif