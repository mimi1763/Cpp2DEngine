/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: support.cpp
---------------------------------------------------
*/

#include "support.h"

//Dependencies
#include "system.h"

namespace lunar {

//Private

support::TYPE support::_Blend_separate=NONE;
support::TYPE support::_Fbo=NONE;
support::TYPE support::_Multi_texture=NONE;
support::TYPE support::_Point_sprite=NONE;
support::TYPE support::_Shader=NONE;
support::TYPE support::_Vbo=NONE;

int support::_Texture_units=1;

support::support() {

	//OpenGL 2.0 supported?
	if (glewIsSupported("GL_VERSION_2_0")) {
	
	_Multi_texture=OPENGL;
	_Point_sprite=OPENGL;
	_Shader=OPENGL;
	_Vbo=OPENGL;
	
	glGetIntegerv(GL_MAX_TEXTURE_UNITS,&_Texture_units);
	}
	//Check for ARB extensions
	else {
	
		if (GLEW_ARB_multitexture) {
		
		_Multi_texture=ARB;
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&_Texture_units);
		}
		
		if (GLEW_ARB_point_sprite) _Point_sprite=ARB;
		if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) _Shader=ARB;
		if (GLEW_ARB_vertex_buffer_object) _Vbo=ARB;
	}
	
	//Check for other extensions
	if (GLEW_EXT_blend_func_separate) _Blend_separate=OPENGL;
	if (GLEW_EXT_framebuffer_object) _Fbo=OPENGL;
}

support::~support() {
	//Empty
}

//Public

support::TYPE support::blend_separate() {
	return _Blend_separate;
}

support::TYPE support::fbo() {
	return _Fbo;
}

support::TYPE support::multi_texture() {
	return _Multi_texture;
}

support::TYPE support::point_sprite() {
	return _Point_sprite;
}

support::TYPE support::shader() {
	return _Shader;
}

support::TYPE support::vbo() {
	return _Vbo;
}

int support::texture_units() {
	return _Texture_units;
}

} //namespace lunar