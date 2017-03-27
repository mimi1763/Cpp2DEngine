/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shaderprogram.h
---------------------------------------------------
*/

#ifndef _SHADERPROGRAM_
#define _SHADERPROGRAM_

#include "manageable.h"
#include "file_container.h"
#include "render_manager.h" //enum BLENDING

#include "uniform.h"
#include "attribute.h"
#include "sampler.h"

namespace lunar {

//Forward declarations
class shaderprogram_manager;
class shader;

class shaderprogram:public manageable,public manageable_listener {

	friend shaderprogram_manager;

protected:

	typedef std::vector<shader*> _Shaders; //Need random access
	
	typedef std::list<uniform*> _Uniforms;
	typedef std::list<attribute*> _Attributes;
	typedef std::list<sampler*> _Samplers;
	
	unsigned int _Glprogram;
	render_manager::BLENDING _Blending;
	_Shaders _Myshaders;
	
	//Shader variables
	
	_Uniforms _Myuniforms;
	_Attributes _Myattributes;
	_Samplers _Mysamplers;
	
	//Protected constructor
	shaderprogram(shaderprogram_manager *_Owner,file_data *_Script_data);
	
	//Protected copy constructor
	shaderprogram(const shaderprogram &_Shaderprogram);
	
	//Protected alternative copy constructor
	shaderprogram(shaderprogram_manager *_Owner,
		const shaderprogram &_Shaderprogram);
	
	//Protected virtual destructor
	virtual ~shaderprogram();
	
	void copy(const shaderprogram &_Shaderprogram);
	
	virtual void status_changed();
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);

public:

	typedef _Uniforms::const_iterator uniforms_iter; //Keep data safe
	typedef _Attributes::const_iterator attributes_iter; //Keep data safe
	typedef _Samplers::const_iterator samplers_iter; //Keep data safe
	
	shaderprogram &operator=(const shaderprogram &_Shaderprogram);
	
	void data(file_data *_Script_data);
	file_data *data() const;
	
	unsigned int glprogram() const;
	
	void blending(const render_manager::BLENDING &_Blending);
	render_manager::BLENDING blending() const;
	
	//Shader variable functions
	
	uniform *create_uniform(const std::string &_Name,
		unsigned int _Components=1,unsigned int _Size=1);
	uniform *create_uniform(const std::string &_Name,int *_Data,
		unsigned int _Components=1,unsigned int _Size=1);
	uniform *create_uniform(const std::string &_Name,float *_Data,
		unsigned int _Components=1,unsigned int _Size=1);
	uniform *create_uniform(const uniform &_Uniform);
	
	attribute *create_attribute(const std::string &_Name,
		unsigned int _Components=1,unsigned int _Stride=0,
			bool _Normalized=false);
	attribute *create_attribute(const std::string &_Name,int *_Data,
		unsigned int _Components=1,unsigned int _Stride=0,
			bool _Normalized=false);
	attribute *create_attribute(const std::string &_Name,float *_Data,
		unsigned int _Components=1,unsigned int _Stride=0,
			bool _Normalized=false);
	attribute *create_attribute(const attribute &_Attribute);
	
	sampler *create_sampler(const std::string &_Name,
		unsigned int _Size=1);
	sampler *create_sampler(const std::string &_Name,texture *_Texture,
		unsigned int _Size=1);
	sampler *create_sampler(const std::string &_Name,animation *_Animation,
		unsigned int _Size=1);
	sampler *create_sampler(const sampler &_Sampler);
	
	void remove_uniform(uniform *&_Uniform);
	void remove_attribute(attribute *&_Attribute);
	void remove_sampler(sampler *&_Sampler);
	
	void clear_uniforms();
	void clear_attributes();
	void clear_samplers();
	
	uniforms_iter uniforms_begin() const;
	uniforms_iter uniforms_end() const;
	
	attributes_iter attributes_begin() const;
	attributes_iter attributes_end() const;
	
	samplers_iter samplers_begin() const;
	samplers_iter samplers_end() const;
};

} //namespace lunar

#endif