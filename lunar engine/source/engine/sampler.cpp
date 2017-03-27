/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sampler.cpp
---------------------------------------------------
*/

#include "sampler.h"

//Dependencies
#include "texture.h"
#include "animation.h"

namespace lunar {

//Protected

sampler::sampler(const std::string &_Name,unsigned int _Size):
	uniform(_Name,1,_Size),_Unit(-1),_Texture(0),_Animation(0) {
	//Empty
}

sampler::sampler(const std::string &_Name,texture *_Texture,
	unsigned int _Size):uniform(_Name,1,_Size),_Unit(-1),
		_Texture(_Texture),_Animation(0) {
	attach_source();
}

sampler::sampler(const std::string &_Name,animation *_Animation,
	unsigned int _Size):uniform(_Name,1,_Size),_Unit(-1),
		_Texture(0),_Animation(_Animation) {
	attach_source();
}

sampler::sampler(const sampler &_Sampler):uniform(_Sampler) {

_Unit=-1; //Not copyable

copy(_Sampler);
attach_source();
}

sampler::~sampler() {
	detach_source();
}

void sampler::copy(const sampler &_Sampler) {

_Texture=_Sampler._Texture;
_Animation=_Sampler._Animation;
}

void sampler::unsubscribed(interface_base *_Interface) {

//Sampler source has been unsubscribed, notify sampler

	if (_Texture==_Interface || _Animation==_Interface) {
	
	_Texture=0; //Remove source
	_Animation=0; //Remove source
	}
}

void sampler::manageable_changed(manageable *_Manageable) {
	//Empty - override if needed
}

void sampler::manageable_statuschanged(manageable *_Manageable) {
	//Empty - override if needed
}

void sampler::manageable_removed(manageable *_Manageable) {

//Sampler source has been destroyed, notify sampler

sampler::unsubscribed(_Manageable); //Reuse functionality
}

void sampler::attach_source() {

//Can't add secure listeners because sampler doesn't own source
//Secure listeners will not be broken before source is out of scope
//If public link is boken the sampler will remove source

	//Add listener
	if (_Texture) _Texture->subscribe(this);
	else if (_Animation) _Animation->subscribe(this);
}

void sampler::detach_source() {

_Listening=false; //Ignore notifications

	//Remove listener
	if (_Texture) _Texture->unsubscribe(this); 
	else if (_Animation) _Animation->unsubscribe(this);

_Listening=true;
}

//Public

sampler &sampler::operator=(const sampler &_Sampler) {

	//Worth doing
	if (this!=&_Sampler) {
	
	uniform::operator=(_Sampler); //Invoke base operator=
	
	bool _Changed=(_Texture!=_Sampler._Texture ||
		_Animation!=_Sampler._Animation);
	
		if (_Changed) detach_source();
	
	copy(_Sampler);
	
		if (_Changed) attach_source();
	}

return *this;
}

void sampler::texture_data(texture *_Texture) {

	if (this->_Texture!=_Texture) {
	
	detach_source();
	this->_Texture=_Texture;
	_Animation=0; //Can't have both
	attach_source();
	}
}

texture *sampler::texture_data() const {
	return _Texture;
}

void sampler::animation_data(animation *_Animation) {

	if (this->_Animation!=_Animation) {
	
	detach_source();
	this->_Animation=_Animation;
	_Texture=0; //Can't have both
	attach_source();
	}
}

animation *sampler::animation_data() const {
	return _Animation;
}

} //namespace lunar