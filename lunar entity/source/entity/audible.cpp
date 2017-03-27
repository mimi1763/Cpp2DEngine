/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: audible.cpp
---------------------------------------------------
*/

#include "audible.h"

//Dependencies (engine)
#include "sound_manager.h"
#include "render_node.h"

//Dependencies (entity)
#include "entity_manager.h"

//Public

//Sound group
//Private

audible::sound_group::sound_group(audible *_Owner,const std::string &_Name):
	_Owner(_Owner),_Name(_Name),_Volume(1.0f),_Playing(true) {
	//Empty
}

audible::sound_group::sound_group(const sound_group &_Sound_group):
	_Owner(_Sound_group._Owner),_Name(_Sound_group._Name) {
	copy(_Sound_group);
}

audible::sound_group::sound_group(audible *_Owner,const std::string &_Name,
	const sound_group &_Sound_group):_Owner(_Owner),_Name(_Name) {
	copy(_Sound_group);
}

audible::sound_group::~sound_group() {
	clear_sounds();
}

void audible::sound_group::copy(const sound_group &_Sound_group) {

_Volume=_Sound_group._Volume;
_Playing=_Sound_group._Playing;

	//Copy sounds
	if (!_Sound_group._Mysounds.empty()) {
	
		for (_Sounds::const_iterator _Iter=_Sound_group._Mysounds.begin(),
			_End=_Sound_group._Mysounds.end();_Iter!=_End;++_Iter)
				create_sound(**_Iter);
	}
}

//Sound group
//Public

audible::sound_group &audible::sound_group::operator=(const sound_group &_Sound_group) {

	//Worth doing
	if (this!=&_Sound_group) {
	
	clear_sounds();
	copy(_Sound_group);
	}

return *this;
}

audible *audible::sound_group::owner() const {
	return _Owner;
}

const std::string &audible::sound_group::name() const {
	return _Name;
}

void audible::sound_group::play(bool _Base) {

	if (_Base) _Playing=true;
	
	if (_Playing) {
	
	sound *_Sound;
	
		for (_Sounds::const_iterator _Iter=_Mysounds.begin(),
			_End=_Mysounds.end();_Iter!=_End;++_Iter) {
		
		_Sound=(*_Iter); //Increase speed
		
			if (_Base || _Sound->playing()) _Sound->play();
		}
	}
}

void audible::sound_group::stop(bool _Reset,bool _Base) {

	if (_Base) _Playing=false;
	
	for (_Sounds::const_iterator _Iter=_Mysounds.begin(),
		_End=_Mysounds.end();_Iter!=_End;++_Iter)
			(*_Iter)->stop(_Reset);
}

void audible::sound_group::volume(float _Volume,bool _Base) {

	if (!_Base || this->_Volume!=_Volume) {
	
		if (_Base) this->_Volume=_Volume;
	
	float _New_volume=(_Base?_Volume:this->_Volume-(1.0f-_Volume));
	
		//Clamp
		if (_New_volume<0.0f) _New_volume=0.0f;
		else if (_New_volume>1.0f) _New_volume=1.0f;
		
		for (_Sounds::const_iterator _Iter=_Mysounds.begin(),
			_End=_Mysounds.end();_Iter!=_End;++_Iter)
				(*_Iter)->volume(_New_volume);
	}
}

float audible::sound_group::volume() const {
	return _Volume;
}

bool audible::sound_group::playing() const {
	return _Playing;
}

//Sounds

sound *audible::sound_group::create_sound(resource_data *_Resource_data,
	bool _Paused) {

	//Nothing to do
	if (!_Resource_data) return 0;

sound_data *_Sound_data=dynamic_cast<sound_data*>(_Resource_data);

	//Invalid type
	if (!_Sound_data) return 0;

sound_manager *_Sound_manager=_Owner->sound_owner();

	//No sound manager found
	if (!_Sound_manager) return 0;

sound *_Mysound=_Sound_manager->create_sound(_Sound_data,_Paused);
_Mysounds.push_back(_Mysound);

	if (!_Paused) _Playing=true;

_Mysound->volume(_Volume); //Set default volume
return _Mysound;
}

sound *audible::sound_group::create_sound(const sound &_Sound) {

sound_manager *_Sound_manager=_Owner->sound_owner();

	//No sound manager found
	if (!_Sound_manager) return 0;

sound *_Mysound=_Sound_manager->create_sound(_Sound);
_Mysounds.push_back(_Mysound);

	if (_Mysound->playing()) _Playing=true;

return _Mysound;
}

void audible::sound_group::remove_sound(sound *&_Sound) {

	for (_Sounds::iterator _Iter=_Mysounds.begin(),
		_End=_Mysounds.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Sound) {
		
		_Sound->owner()->remove(_Sound);
		_Mysounds.erase(_Iter);
		return;
		}
	}
}

void audible::sound_group::clear_sounds() {

sound *_Sound;

	for (_Sounds::const_iterator _Iter=_Mysounds.begin(),
		_End=_Mysounds.end();_Iter!=_End;++_Iter) {
	
	_Sound=(*_Iter); //Increase speed
	_Sound->owner()->remove(_Sound);
	}

_Mysounds.clear();
}

audible::sound_group::sounds_iter audible::sound_group::sounds_begin() const {
	return _Mysounds.begin();
}

audible::sound_group::sounds_iter audible::sound_group::sounds_end() const {
	return _Mysounds.end();
}

//Audible
//Protected

audible::audible():_Volume(1.0f),_Playing(true) {
	//Empty
}

audible::audible(const audible &_Audible) {
	copy(_Audible);
}

audible::~audible() {
	clear_soundgroups();
}

void audible::copy(const audible &_Audible) {

_Volume=_Audible._Volume;
_Playing=_Audible._Playing;

	//Copy sounds
	if (!_Audible._Mysound_groups.empty()) {
	
	sound_group *_Sound_group;
	
		for (_Sound_groups::const_iterator _Iter=_Audible._Mysound_groups.begin(),
			_End=_Audible._Mysound_groups.end();_Iter!=_End;++_Iter) {
		
		_Sound_group=(*_Iter); //Increase speed
		create_soundgroup(_Sound_group->_Name,*_Sound_group);
		}
	}
}

render_node *audible::sound_node() const {
	return 0;
}

sound_manager *audible::sound_owner() const {
	return 0;
}

//Public

audible &audible::operator=(const audible &_Audible) {

	//Worth doing
	if (this!=&_Audible) {
	
	clear_soundgroups();
	copy(_Audible);
	}

return *this;
}

void audible::update(float _Time) {

	if (_Mysound_groups.empty()) return;

render_node *_Node=sound_node();

	if (_Node) {
	
	const vector2 &_Position=_Node->world_position();
	
	sound_group *_Sound_group;
	sound *_Sound;
	
		//Update sound position and velocity
		for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
			_End=_Mysound_groups.end();_Iter!=_End;++_Iter) {
		
		_Sound_group=(*_Iter); //Increase speed
		
			for (sound_group::_Sounds::const_iterator _Siter=_Sound_group->_Mysounds.begin(),
				_Send=_Sound_group->_Mysounds.end();_Siter!=_Send;++_Siter) {
			
			_Sound=(*_Siter); //Increase speed
			_Sound->attributes(_Position,(_Position-_Sound->position())/_Time);
			}
		}
	}
}

void audible::play(bool _Base) {

	if (_Base) _Playing=true;
	
	for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter)
			(*_Iter)->play(_Base);
}

void audible::stop(bool _Reset,bool _Base) {

	if (_Base) _Playing=false;
	
	for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter)
			(*_Iter)->stop(_Reset,_Base);
}

void audible::volume(float _Volume,bool _Base) {

	if (_Base) this->_Volume=_Volume;
	
	for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter)
			(*_Iter)->volume(_Volume,_Base);
}

float audible::volume() const {
	return _Volume;
}

bool audible::playing() const {
	return _Playing;
}

//Sound groups

audible::sound_group *audible::create_soundgroup(const std::string &_Name) {

	//Sound group already exist
	if (soundgroup_pointer(_Name)) return 0;

sound_group *_Mysound_group=new sound_group(this,_Name);
_Mysound_groups.push_back(_Mysound_group);

_Mysound_group->volume(_Volume); //Set default volume
return _Mysound_group;
}

audible::sound_group *audible::create_soundgroup(const std::string &_Name,
	const sound_group &_Sound_group) {

	//Sound group already exist
	if (soundgroup_pointer(_Name)) return 0;

sound_group *_Mysound_group=new sound_group(this,_Name,_Sound_group);
_Mysound_groups.push_back(_Mysound_group);

_Mysound_group->volume(_Volume); //Set default volume
return _Mysound_group;
}

void audible::remove_soundgroup(sound_group *&_Sound_group) {

	for (_Sound_groups::iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Sound_group) {
		
		delete _Sound_group;
		_Sound_group=0; //Set referenced pointer to null
		
		_Mysound_groups.erase(_Iter);
		return;
		}
	}
}

void audible::clear_soundgroups() {

	for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Mysound_groups.clear();
}

audible::soundgroups_iter audible::soundgroups_begin() const {
	return _Mysound_groups.begin();
}

audible::soundgroups_iter audible::soundgroups_end() const {
	return _Mysound_groups.end();
}

audible::sound_group *audible::soundgroup_pointer(const std::string &_Name) const {

	for (_Sound_groups::const_iterator _Iter=_Mysound_groups.begin(),
		_End=_Mysound_groups.end();_Iter!=_End;++_Iter) {
	
		//Sound group exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}