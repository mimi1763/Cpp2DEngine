/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: framebuffer_manager.cpp
---------------------------------------------------
*/

#include "framebuffer_manager.h"

//Dependencies
#include "system.h"

#include "texture_manager.h"
#include "support.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
framebuffer_manager *framebuffer_manager::_Instance=0;

//Protected

void framebuffer_manager::copy(const framebuffer_manager &_Framebuffer_manager) {
	_Texture_manager=_Framebuffer_manager._Texture_manager;
}

void framebuffer_manager::removed(manageable *_Manageable) {

	//Must be supported
	if (support::fbo()!=support::OPENGL) return;

framebuffer *_Framebuffer=static_cast<framebuffer*>(_Manageable);

	//Invalid GL framebuffer, skip
	if (_Framebuffer->_Glframebuffer==0) return;

glDeleteRenderbuffersEXT(1,&_Framebuffer->_Glrenderbuffer);
_Framebuffer->_Glrenderbuffer=0;

glDeleteFramebuffersEXT(1,&_Framebuffer->_Glframebuffer);
_Framebuffer->_Glframebuffer=0;
}

//GL framebuffer specific

bool framebuffer_manager::init_framebuffer(framebuffer *_Framebuffer) {

	//Must be supported
	if (support::fbo()!=support::OPENGL) return false;

removed(_Framebuffer); //Make sure previous GL framebuffer is deleted

//Create frame buffer object
unsigned int _Glframebuffer=0;
glGenFramebuffersEXT(1,&_Glframebuffer);

	//Invalid frame buffer object
	if (_Glframebuffer==0) return false;

glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,_Glframebuffer);

//Create render buffer
unsigned int _Glrenderbuffer=0;
glGenRenderbuffersEXT(1,&_Glrenderbuffer);

	//Invalid render buffer
	if (_Glrenderbuffer==0) {
	
	//Failed, clean up created frame buffer
	glDeleteFramebuffersEXT(1,&_Glframebuffer);
	return false;
	}

glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,_Glrenderbuffer);
glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,
	_Framebuffer->_Width,_Framebuffer->_Height); //Allocate buffer size

glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,
	GL_RENDERBUFFER_EXT,_Glrenderbuffer); //Attach render buffer to frame buffer object

glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,
	GL_TEXTURE_2D,_Framebuffer->_Texture->gltexture(),0); //Attach texture to frame buffer object

	//Finally check frame buffer status
	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=
		GL_FRAMEBUFFER_COMPLETE_EXT) {
	
	removed(_Framebuffer); //Status is not okay, clean up
	return false;
	}

_Framebuffer->_Glframebuffer=_Glframebuffer;
_Framebuffer->_Glrenderbuffer=_Glrenderbuffer;
return true;
}

//Public

framebuffer_manager::framebuffer_manager():
	_Texture_manager(texture_manager::instance()) {
	//Empty
}

framebuffer_manager::framebuffer_manager(texture_manager *_Texture_manager):
	_Texture_manager(_Texture_manager) {
	//Empty
}

framebuffer_manager::framebuffer_manager(const framebuffer_manager &_Framebuffer_manager):
	manager(_Framebuffer_manager) {
	copy(_Framebuffer_manager);
}

framebuffer_manager::~framebuffer_manager() {

tidy(); //Call before clearing cache
clear_cache();
}

framebuffer_manager &framebuffer_manager::operator=(
	const framebuffer_manager &_Framebuffer_manager) {

	//Worth doing
	if (this!=&_Framebuffer_manager) {
	
	manager::operator=(_Framebuffer_manager); //Invoke base operator=
	copy(_Framebuffer_manager);
	}

return *this;
}

framebuffer_manager *framebuffer_manager::instance() {
	return _Instance;
}

void framebuffer_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

framebuffer *_Framebuffer=dynamic_cast<framebuffer*>(_Manageable);

	//Invalid type for this manager
	if (!_Framebuffer) return;
	
	//No texture
	if (!_Framebuffer->_Texture) {
	
	status(_Framebuffer,manageable::FAILED);
	return;
	}

_Texture_manager->update(_Framebuffer->_Texture); //Update texture

	//Texture is invalid
	if (_Framebuffer->_Texture->status()!=manageable::OKAY) {
	
	status(_Framebuffer,manageable::FAILED);
	return;
	}

status(_Framebuffer,init_framebuffer(_Framebuffer)?manageable::OKAY:
	manageable::FAILED);
}

void framebuffer_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

//Framebuffer functions

framebuffer *framebuffer_manager::create_framebuffer(unsigned int _Width,
	unsigned int _Height,bool _Visible,bool _Secured) {

unsigned int _Framebuffer_width=2;
unsigned int _Framebuffer_height=2;

	//Make sure framebuffer size is power of 2
	while (_Framebuffer_width<_Width) _Framebuffer_width*=2;
	while (_Framebuffer_height<_Height) _Framebuffer_height*=2;

framebuffer *_Myframebuffer=new framebuffer(this,_Framebuffer_width,
	_Framebuffer_height,_Texture_manager->create_texture(
		_Width,_Height,_Visible,_Secured));
add(_Myframebuffer,_Secured);
return _Myframebuffer;
}

framebuffer *framebuffer_manager::create_framebuffer(const framebuffer &_Framebuffer,
	bool _Secured) {

framebuffer *_Myframebuffer=new framebuffer(this,_Framebuffer,
	_Framebuffer._Texture?_Texture_manager->create_texture(
		*_Framebuffer._Texture,_Secured):0);
add(_Myframebuffer,_Secured);
return _Myframebuffer;
}

void framebuffer_manager::clear_cache() {

manageable *_Manageable;

	//Delete GL framebuffers and update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) {
	
	_Manageable=(*_Iter); //Increase speed
	
	removed(_Manageable);
	status(_Manageable,manageable::NEED_UPDATE);
	}
}

} //namespace lunar