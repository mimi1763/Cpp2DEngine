/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: shader.cpp
---------------------------------------------------
*/

#include "shader.h"

//Dependencies
#include "shader_manager.h"

namespace lunar {

//Protected

shader::shader(shader_manager *_Owner,shader_data *_Shader_data):
	manageable(_Owner,_Shader_data),_Glshader(0) {
	//Empty
}

shader::shader(const shader &_Shader):manageable(_Shader) {
	copy(_Shader);
}

shader::shader(shader_manager *_Owner,const shader &_Shader):
	manageable(_Owner,_Shader) {
	copy(_Shader);
}

shader::~shader() {
	//Empty
}

void shader::copy(const shader &_Shader) {

_Glshader=_Shader._Glshader;
status(_Shader._Status); //Inherit status
}

//Public

shader &shader::operator=(const shader &_Shader) {

	//Worth doing
	if (this!=&_Shader) {
	
	manageable::operator=(_Shader); //Invoke base operator=
	copy(_Shader);
	}

return *this;
}

void shader::data(shader_data *_Shader_data) {
	manageable::data(_Shader_data); //Data contains the correct derived data type
}

shader_data *shader::data() const {
	return static_cast<shader_data*>(manageable::data());
}

unsigned int shader::glshader() {
	return _Glshader;
}

} //namespace lunar