/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader_manager.cpp
---------------------------------------------------
*/

#include "shader_manager.h"

//Dependencies
#include "system.h"

#include "support.h"
#include "file_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
shader_manager *shader_manager::_Instance=0;

//Protected

void shader_manager::copy(const shader_manager &_Shader_manager) {
	_Shader_container=_Shader_manager._Shader_container;
}

//GL shader specific

bool shader_manager::create_glshader(shader_data *_Shader_data) {

unsigned int _Glshader=0;
std::string _Shadersource;

//Retrieve shader source from data file
_Shader_container->load_bytes(_Shader_data,_Shadersource);

	//Retrieve shader source from file (filename)
	if (_Shadersource.empty() && !file_manager::load_binary(
		_Shader_data->filename(),_Shadersource)) return false;

const char *_Shadersource_ptr=_Shadersource.c_str();
int _Compile=0;

	//Compile shader
	if (support::shader()==support::OPENGL) {
	
		if (_Shader_data->type()==shader_data::VERTEX)
			_Glshader=GL_VERTEX_SHADER;
		else if (_Shader_data->type()==shader_data::FRAGMENT)
			_Glshader=GL_FRAGMENT_SHADER;
	
	_Glshader=glCreateShader(_Glshader);
	glShaderSource(_Glshader,1,&_Shadersource_ptr,0); //Set GLSL code
	
	glCompileShader(_Glshader); //Compile
	glGetShaderiv(_Glshader,GL_COMPILE_STATUS,&_Compile);
	}
	else if (support::shader()==support::ARB) {
	
		if (_Shader_data->type()==shader_data::VERTEX)
			_Glshader=GL_VERTEX_SHADER_ARB;
		else if (_Shader_data->type()==shader_data::FRAGMENT)
			_Glshader=GL_FRAGMENT_SHADER_ARB;
	
	_Glshader=glCreateShaderObjectARB(_Glshader);
	glShaderSourceARB(_Glshader,1,&_Shadersource_ptr,0); //Set GLSL code
	
	glCompileShaderARB(_Glshader); //Compile
	glGetObjectParameterivARB(_Glshader,GL_OBJECT_COMPILE_STATUS_ARB,&_Compile);
	}
	
	//Shader did not compile properly
	if (_Compile==0) {
	
	file_manager::save("shader_log.txt",_Shader_data->filename(),true);
	save_shaderlog(_Glshader);
	
		//Clean up shader object
		if (support::shader()==support::OPENGL) glDeleteShader(_Glshader);
		else if (support::shader()==support::ARB) glDeleteObjectARB(_Glshader);
	
	return false;
	}

_Created_glshader _Mycreated_glshader;
_Mycreated_glshader._Shader_data=_Shader_data;
_Mycreated_glshader._Glshader=_Glshader;
_Mycreated_glshaders.push_front(_Mycreated_glshader); //Faster access to newly added elements
return true;
}

void shader_manager::init_shader(shader *_Shader,const _Created_glshader &_Mycreated_glshader) {
	_Shader->_Glshader=_Mycreated_glshader._Glshader;
}

void shader_manager::save_shaderlog(unsigned int _Glshader) {

int _Length=0;

	//Retrieve log length from shader object
	if (support::shader()==support::OPENGL) glGetShaderiv(
		_Glshader,GL_INFO_LOG_LENGTH,&_Length);
	else if (support::shader()==support::ARB) glGetObjectParameterivARB(
		_Glshader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&_Length);
	
	if (_Length>0) {
	
	char *_Log=new char[_Length];
	int _Written=0;
	
		//Retrieve log from shader object
		if (support::shader()==support::OPENGL) glGetShaderInfoLog(
			_Glshader,_Length,&_Written,_Log);
		else if (support::shader()==support::ARB) glGetInfoLogARB(
			_Glshader,_Length,&_Written,_Log);
	
	std::string _Data(_Log);
	delete[] _Log;
	
	file_manager::save("shader_log.txt",_Data,true); //Save shader log
	}
}

//Public

shader_manager::shader_manager():_Shader_container(
	shader_container::instance()) {
	//Empty
}

shader_manager::shader_manager(const shader_manager &_Shader_manager):
	manager(_Shader_manager) {
	copy(_Shader_manager);
}

shader_manager::~shader_manager() {

tidy(); //Call before clearing cache
clear_cache();
}

shader_manager &shader_manager::operator=(const shader_manager &_Shader_manager) {

	//Worth doing
	if (this!=&_Shader_manager) {
	
	manager::operator=(_Shader_manager); //Invoke base operator=
	copy(_Shader_manager);
	}

return *this;
}

shader_manager *shader_manager::instance() {
	return _Instance;
}

void shader_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

shader *_Shader=dynamic_cast<shader*>(_Manageable);

	//Invalid type for this manager
	if (!_Shader) return;

shader_data *_Shader_data=static_cast<shader_data*>(
	_Manageable->data());

	//No data
	if (!_Shader_data) {
	
	status(_Shader,manageable::FAILED);
	return;
	}
	
	for (_Created_glshaders::const_iterator _Iter=_Mycreated_glshaders.begin(),
		_End=_Mycreated_glshaders.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)._Shader_data==_Shader_data) {
		
		init_shader(_Shader,*_Iter);
		status(_Shader,manageable::OKAY); //No need for update anymore
		return;
		}
	}
	
	//Could not create glshader
	if (!create_glshader(_Shader_data)) {
	
	status(_Shader,manageable::FAILED);
	return;
	}

init_shader(_Shader,_Mycreated_glshaders.front());
status(_Shader,manageable::OKAY); //No need for update anymore
}

void shader_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void shader_manager::shader_source(shader_container *_Shader_container) {
	this->_Shader_container=_Shader_container;
}

shader_container *shader_manager::shader_source() const {
	return _Shader_container;
}

//Shader functions

shader *shader_manager::create_shader(const std::string &_Name,bool _Secured) {
	return create_shader(static_cast<shader_data*>(
		_Shader_container->data(_Name)),_Secured);
}

shader *shader_manager::create_shader(const shader_data &_Shader_data,bool _Secured) {
	return create_shader(_Shader_container->add_data(
		_Shader_data),_Secured);
}

shader *shader_manager::create_shader(shader_data *_Shader_data,bool _Secured) {

shader *_Myshader=new shader(this,_Shader_data);
add(_Myshader,_Secured);
return _Myshader;
}

shader *shader_manager::create_shader(const shader &_Shader,bool _Secured) {

shader *_Myshader=new shader(this,_Shader);
add(_Myshader,_Secured);
return _Myshader;
}

void shader_manager::clear_cache() {

	for (_Created_glshaders::const_iterator _Iter=_Mycreated_glshaders.begin(),
		_End=_Mycreated_glshaders.end();_Iter!=_End;++_Iter) {
	
		if (support::shader()==support::OPENGL) glDeleteShader(
			(*_Iter)._Glshader);
		else if (support::shader()==support::ARB) glDeleteObjectARB(
			(*_Iter)._Glshader);
	}

_Mycreated_glshaders.clear();

	//Update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) status(
			*_Iter,manageable::NEED_UPDATE);
}

} //namespace lunar