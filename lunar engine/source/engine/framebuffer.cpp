/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: framebuffer.cpp
---------------------------------------------------
*/

#include "framebuffer.h"

//Dependencies
#include "framebuffer_manager.h"
#include "texture_manager.h"

namespace lunar {

//Protected

framebuffer::framebuffer(framebuffer_manager *_Owner,unsigned int _Width,
	unsigned int _Height,texture *_Texture):manageable(_Owner,0),
		_Glframebuffer(0),_Width(_Width),_Height(_Height),
			_Glrenderbuffer(0),_Texture(_Texture) {
	_Texture->subscribe(this,true); //Add secure listener
}

framebuffer::framebuffer(const framebuffer &_Framebuffer):
	manageable(_Framebuffer) {

_Glframebuffer=0; //Not copyable
_Glrenderbuffer=0; //Not copyable
_Texture=0; //No texture

copy(_Framebuffer);
}

framebuffer::framebuffer(framebuffer_manager *_Owner,
	const framebuffer &_Framebuffer,texture *_Texture):
		manageable(_Owner,_Framebuffer),_Texture(_Texture) {

_Glframebuffer=0; //Not copyable
_Glrenderbuffer=0; //Not copyable
_Texture->subscribe(this,true); //Add secure listener

copy(_Framebuffer);
}

framebuffer::~framebuffer() {

_Listening=false; //Ignore notifications

	if (_Texture) _Texture->owner()->remove(_Texture);
}

void framebuffer::copy(const framebuffer &_Framebuffer) {

_Width=_Framebuffer._Width;
_Height=_Framebuffer._Height;
}

void framebuffer::manageable_changed(manageable *_Manageable) {

//Texture has been changed, notify framebuffer

	 //Update explicit (not changed)
	if (_Texture==_Manageable) status(NEED_UPDATE);
}

void framebuffer::manageable_statuschanged(manageable *_Manageable) {

//Texture status has been changed, notify framebuffer

	//Update explicit (not changed)
	if (_Manageable->status()==NEED_UPDATE && _Texture==_Manageable)
		status(NEED_UPDATE);
}

void framebuffer::manageable_removed(manageable *_Manageable) {

//Texture has been destroyed, notify framebuffer

	if (_Texture==_Manageable) {
	
	_Texture=0;
	status(FAILED); //Invalidate framebuffer
	}
}

//Public

framebuffer &framebuffer::operator=(const framebuffer &_Framebuffer) {

	//Worth doing
	if (this!=&_Framebuffer) {
	
	manageable::operator=(_Framebuffer); //Invoke base operator=
	
	bool _Changed=(_Width!=_Framebuffer._Width ||
		_Height!=_Framebuffer._Height);
	copy(_Framebuffer);
	
		//Update explicit
		if (_Changed) changed();
	}

return *this;
}

bool framebuffer::operator==(const framebuffer &_Framebuffer) const {
	return (_Width==_Framebuffer._Width && _Height==_Framebuffer._Height); //Redefine equality
}

bool framebuffer::operator!=(const framebuffer &_Framebuffer) const {
	return !(*this==_Framebuffer); //Redefine inequality
}

unsigned int framebuffer::glframebuffer() const {
	return _Glframebuffer;
}

unsigned int framebuffer::width() const {
	return _Width;
}

unsigned int framebuffer::height() const {
	return _Height;
}

texture *framebuffer::framebuffer_texture() const {
	return _Texture;
}

} //namespace lunar