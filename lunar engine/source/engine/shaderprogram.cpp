/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shaderprogram.cpp
---------------------------------------------------
*/

#include "shaderprogram.h"

//Dependencies
#include "shaderprogram_manager.h"
#include "shader.h"

namespace lunar {

//Protected

shaderprogram::shaderprogram(shaderprogram_manager *_Owner,
	file_data *_Script_data):manageable(_Owner,_Script_data),
		_Glprogram(0),_Blending(render_manager::DEFAULT_ALPHA_BLEND) {
	//Empty
}

shaderprogram::shaderprogram(const shaderprogram &_Shaderprogram):
	manageable(_Shaderprogram) {

_Glprogram=0; //Not copyable
copy(_Shaderprogram);
}

shaderprogram::shaderprogram(shaderprogram_manager *_Owner,
	const shaderprogram &_Shaderprogram):manageable(_Owner,_Shaderprogram) {

_Glprogram=0; //Not copyable
copy(_Shaderprogram);
}

shaderprogram::~shaderprogram() {

_Listening=false; //Ignore notifications

	if (!_Myshaders.empty()) _Myshaders.front()->owner()->remove(
		_Myshaders.front(),(unsigned int)_Myshaders.size());

clear_uniforms();
clear_attributes();
clear_samplers();
}

void shaderprogram::copy(const shaderprogram &_Shaderprogram) {
	_Blending=_Shaderprogram._Blending;
}

void shaderprogram::status_changed() {

manageable::status_changed(); //Use base functionality

	//Update all shader variables
	if (_Status==NEED_UPDATE) {
	
		for (_Uniforms::const_iterator _Iter=_Myuniforms.begin(),
			_End=_Myuniforms.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Status=shadervariable::NEED_UPDATE;
		
		for (_Attributes::const_iterator _Iter=_Myattributes.begin(),
			_End=_Myattributes.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Status=shadervariable::NEED_UPDATE;
		
		for (_Samplers::const_iterator _Iter=_Mysamplers.begin(),
			_End=_Mysamplers.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Status=shadervariable::NEED_UPDATE;
	}
}

void shaderprogram::manageable_changed(manageable *_Manageable) {

//Shader has been changed, notify shaderprogram

	//Presumes that the shader is attached to the program
	//Update explicit (not changed)
	if (dynamic_cast<shader*>(_Manageable)) status(NEED_UPDATE);
}

void shaderprogram::manageable_statuschanged(manageable *_Manageable) {

//Shader status has been changed, notify shaderprogram

	//Presumes that the shader is attached to the program
	//Update explicit (not changed)
	if (_Manageable->status()==NEED_UPDATE &&
		dynamic_cast<shader*>(_Manageable)) status(NEED_UPDATE);
}

void shaderprogram::manageable_removed(manageable *_Manageable) {

//Shader has been destroyed, notify shaderprogram

	//Find and remove shader from shaderprogram
	for (size_t i=0;i<_Myshaders.size();++i) {
	
		if (_Myshaders[i]==_Manageable) {
		
		_Myshaders.erase(_Myshaders.begin()+i);
		break;
		}
	}

status(FAILED); //Invalidate shaderprogram
}

//Public

shaderprogram &shaderprogram::operator=(const shaderprogram &_Shaderprogram) {

	//Worth doing
	if (this!=&_Shaderprogram) {
	
	manageable::operator=(_Shaderprogram); //Invoke base operator=
	copy(_Shaderprogram);
	}

return *this;
}

void shaderprogram::data(file_data *_Script_data) {
	manageable::data(_Script_data); //Data contains the correct derived data type
}

file_data *shaderprogram::data() const {
	return static_cast<file_data*>(manageable::data());
}

unsigned int shaderprogram::glprogram() const {
	return _Glprogram;
}

void shaderprogram::blending(const render_manager::BLENDING &_Blending) {
	this->_Blending=_Blending;
}

render_manager::BLENDING shaderprogram::blending() const {
	return _Blending;
}

//Shader variable functions

uniform *shaderprogram::create_uniform(const std::string &_Name,
	unsigned int _Components,unsigned int _Size) {

uniform *_Myuniform=new uniform(_Name,_Components,_Size);
_Myuniforms.push_back(_Myuniform);
return _Myuniform;
}

uniform *shaderprogram::create_uniform(const std::string &_Name,
	int *_Data,unsigned int _Components,unsigned int _Size) {

uniform *_Myuniform=new uniform(_Name,_Data,_Components,_Size);
_Myuniforms.push_back(_Myuniform);
return _Myuniform;
}

uniform *shaderprogram::create_uniform(const std::string &_Name,
	float *_Data,unsigned int _Components,unsigned int _Size) {

uniform *_Myuniform=new uniform(_Name,_Data,_Components,_Size);
_Myuniforms.push_back(_Myuniform);
return _Myuniform;
}

uniform *shaderprogram::create_uniform(const uniform &_Uniform) {

uniform *_Myuniform=new uniform(_Uniform);
_Myuniforms.push_back(_Myuniform);
return _Myuniform;
}

attribute *shaderprogram::create_attribute(const std::string &_Name,
	unsigned int _Components,unsigned int _Stride,
		bool _Normalized) {

attribute *_Myattribute=new attribute(_Name,_Components,
	_Stride,_Normalized);
_Myattributes.push_back(_Myattribute);
return _Myattribute;
}

attribute *shaderprogram::create_attribute(const std::string &_Name,
	int *_Data,unsigned int _Components,unsigned int _Stride,
		bool _Normalized) {

attribute *_Myattribute=new attribute(_Name,_Data,_Components,
	_Stride,_Normalized);
_Myattributes.push_back(_Myattribute);
return _Myattribute;
}

attribute *shaderprogram::create_attribute(const std::string &_Name,
	float *_Data,unsigned int _Components,unsigned int _Stride,
		bool _Normalized) {

attribute *_Myattribute=new attribute(_Name,_Data,_Components,
	_Stride,_Normalized);
_Myattributes.push_back(_Myattribute);
return _Myattribute;
}

attribute *shaderprogram::create_attribute(const attribute &_Attribute) {

attribute *_Myattribute=new attribute(_Attribute);
_Myattributes.push_back(_Myattribute);
return _Myattribute;
}

sampler *shaderprogram::create_sampler(const std::string &_Name,
	unsigned int _Size) {

sampler *_Mysampler=new sampler(_Name,_Size);
_Mysamplers.push_back(_Mysampler);
return _Mysampler;
}

sampler *shaderprogram::create_sampler(const std::string &_Name,
	texture *_Texture,unsigned int _Size) {

sampler *_Mysampler=new sampler(_Name,_Texture,_Size);
_Mysamplers.push_back(_Mysampler);
return _Mysampler;
}

sampler *shaderprogram::create_sampler(const std::string &_Name,
	animation *_Animation,unsigned int _Size) {

sampler *_Mysampler=new sampler(_Name,_Animation,_Size);
_Mysamplers.push_back(_Mysampler);
return _Mysampler;
}

sampler *shaderprogram::create_sampler(const sampler &_Sampler) {

sampler *_Mysampler=new sampler(_Sampler);
_Mysamplers.push_back(_Mysampler);
return _Mysampler;
}

void shaderprogram::remove_uniform(uniform *&_Uniform) {

	for (_Uniforms::iterator _Iter=_Myuniforms.begin(),
		_End=_Myuniforms.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Uniform) {
		
		delete _Uniform;
		_Uniform=0; //Set referenced pointer to null
		
		_Myuniforms.erase(_Iter);
		return;
		}
	}
}

void shaderprogram::remove_attribute(attribute *&_Attribute) {

	for (_Attributes::iterator _Iter=_Myattributes.begin(),
		_End=_Myattributes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Attribute) {
		
		delete _Attribute;
		_Attribute=0; //Set referenced pointer to null
		
		_Myattributes.erase(_Iter);
		return;
		}
	}
}

void shaderprogram::remove_sampler(sampler *&_Sampler) {

	for (_Samplers::iterator _Iter=_Mysamplers.begin(),
		_End=_Mysamplers.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Sampler) {
		
		delete _Sampler;
		_Sampler=0; //Set referenced pointer to null
		
		_Mysamplers.erase(_Iter);
		return;
		}
	}
}

void shaderprogram::clear_uniforms() {

	for (_Uniforms::const_iterator _Iter=_Myuniforms.begin(),
		_End=_Myuniforms.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myuniforms.clear();
}

void shaderprogram::clear_attributes() {

	for (_Attributes::const_iterator _Iter=_Myattributes.begin(),
		_End=_Myattributes.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myattributes.clear();
}

void shaderprogram::clear_samplers() {

	for (_Samplers::const_iterator _Iter=_Mysamplers.begin(),
		_End=_Mysamplers.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Mysamplers.clear();
}

shaderprogram::uniforms_iter shaderprogram::uniforms_begin() const {
	return _Myuniforms.begin();
}

shaderprogram::uniforms_iter shaderprogram::uniforms_end() const {
	return _Myuniforms.end();
}

shaderprogram::attributes_iter shaderprogram::attributes_begin() const {
	return _Myattributes.begin();
}

shaderprogram::attributes_iter shaderprogram::attributes_end() const {
	return _Myattributes.end();
}

shaderprogram::samplers_iter shaderprogram::samplers_begin() const {
	return _Mysamplers.begin();
}

shaderprogram::samplers_iter shaderprogram::samplers_end() const {
	return _Mysamplers.end();
}

} //namespace lunar