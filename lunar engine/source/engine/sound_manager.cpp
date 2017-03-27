/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_manager.cpp
---------------------------------------------------
*/

#include "sound_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
sound_manager *sound_manager::_Instance=0;

//Protected

void sound_manager::copy(const sound_manager &_Sound_manager) {

_Sound_container=_Sound_manager._Sound_container;

_Listener_position=_Sound_manager._Listener_position;
_Listener_velocity=_Sound_manager._Listener_velocity;
}

//Fmod specific

bool sound_manager::init_fmod() {

_System=0;

	if (FMOD::System_Create(&_System)!=FMOD_OK) return false;

_System->set3DSettings(1.0f,1.0f,0.5f);
return (_System->init(MAX_CHANNELS,FMOD_INIT_NORMAL,0)==FMOD_OK);
}

bool sound_manager::create_fmodsound(sound_data *_Sound_data) {

const char *_Data=_Sound_data->filename().c_str(); //Use filename as default data

FMOD_CREATESOUNDEXINFO _Info={0};
_Info.cbsize=sizeof(FMOD_CREATESOUNDEXINFO); //Required

FMOD_MODE _Mode=((_Sound_data->software()?FMOD_SOFTWARE:FMOD_HARDWARE) |
	(_Sound_data->positional()?FMOD_3D:FMOD_2D) | (_Sound_data->loop()?
		FMOD_LOOP_NORMAL:FMOD_LOOP_OFF));

std::string _Bytes;
char *_Stream_data=0;

	//Load data from data file (memory)
	if (_Sound_container->load_bytes(_Sound_data,_Bytes)) {
	
	_Info.length=(unsigned int)_Bytes.size(); //Required
	_Mode|=FMOD_OPENMEMORY;
	
	//For sound streams, FMOD uses the memory as is
	//else it duplicates the memory into its own buffers
	
		if (_Sound_data->stream()) {
		
		_Stream_data=new char[_Bytes.size()];
		memcpy(_Stream_data,_Bytes.c_str(),_Bytes.size());
		}
	
	_Data=(_Stream_data?_Stream_data:_Bytes.c_str());
	}

FMOD::Sound *_Fmodsound=0;
FMOD_RESULT _Result=(_Sound_data->stream()?_System->createStream(
	_Data,_Mode,&_Info,&_Fmodsound):_System->createSound(
		_Data,_Mode,&_Info,&_Fmodsound));

	if (_Result!=FMOD_OK) {
	
		//Deallocate stream data if used
		if (_Stream_data) delete[] _Stream_data;
	
	return false;
	}

_Created_sound _Mycreated_sound;
_Mycreated_sound._Sound_data=_Sound_data;
_Mycreated_sound._Fmodsound=_Fmodsound;
_Mycreated_sound._Stream_data=_Stream_data;
_Mycreated_sounds.push_front(_Mycreated_sound); //Faster access to newly added elements
return true;
}

void sound_manager::init_sound(sound *_Sound,const _Created_sound &_Mycreated_sound) const {
	_Sound->_Fmodsound=_Mycreated_sound._Fmodsound;
}

//Public

const int sound_manager::MAX_CHANNELS=100;

sound_manager::sound_manager():_Sound_container(
	sound_container::instance()) {
	_Supported=init_fmod();
}

sound_manager::sound_manager(const sound_manager &_Sound_manager):
	manager(_Sound_manager) {

_Supported=init_fmod();
copy(_Sound_manager);
}

sound_manager::~sound_manager() {

tidy(); //Call before clearing cache
clear_cache();

	if (_Supported) _System->release();
}

sound_manager &sound_manager::operator=(const sound_manager &_Sound_manager) {

	//Worth doing
	if (this!=&_Sound_manager) {
	
	manager::operator=(_Sound_manager); //Invoke base operator=
	copy(_Sound_manager);
	
	listener_attributes(_Listener_position,_Listener_velocity); //Update listener attributes
	}

return *this;
}

sound_manager *sound_manager::instance() {
	return _Instance;
}

void sound_manager::update(manageable *_Manageable) {

	//No need for update
	if (_Manageable->status()!=manageable::NEED_UPDATE) return;

sound *_Sound=dynamic_cast<sound*>(_Manageable);

	//Invalid type for this manager
	if (!_Sound) return;
	
	//Not supported or max channels reached
	if (!_Supported || _Mymanageables.size()>=MAX_CHANNELS) {
	
	status(_Sound,manageable::FAILED);
	return;
	}

sound_data *_Sound_data=static_cast<sound_data*>(
	_Manageable->data());

	//No data
	if (!_Sound_data) {
	
	status(_Sound,manageable::FAILED);
	return;
	}

FMOD::Sound *_Fmodsound=0;

	//Reuse fmodsound for samples
	if (!_Sound_data->stream()) {
	
		for (_Created_sounds::const_iterator _Iter=_Mycreated_sounds.begin(),
			_End=_Mycreated_sounds.end();_Iter!=_End;++_Iter) {
		
			if ((*_Iter)._Sound_data==_Sound_data) {
			
			_Fmodsound=(*_Iter)._Fmodsound;
			break;
			}
		}
	}
	
	//Could not create fmod sound
	if (!_Fmodsound && !create_fmodsound(_Sound_data)) {
	
	status(_Sound,manageable::FAILED);
	return;
	}

init_sound(_Sound,_Mycreated_sounds.front());
status(_Sound,manageable::OKAY);

	//Start sound automatically (requires status OKAY)
	if (!_Sound->_Paused) _Sound->play();
}

void sound_manager::update() {

manager::update(); //Use base functionality

	//Update fmod
	if (_Supported) _System->update();
}

void sound_manager::sound_source(sound_container *_Sound_container) {
	this->_Sound_container=_Sound_container;
}

sound_container *sound_manager::sound_source() const {
	return _Sound_container;
}

void sound_manager::volume(float _Volume) {

	if (_Supported) {
	
	FMOD::ChannelGroup *_Channelgroup;
	_System->getMasterChannelGroup(&_Channelgroup);
	_Channelgroup->setVolume(_Volume);
	}
}

void sound_manager::listener_attributes(const vector2 &_Listener_position,
	const vector2 &_Listener_velocity) {

	if (_Supported) {
	
	//FMOD Ex is left handed (+x is right, +y is up and +z is forward)
	FMOD_VECTOR _Position={_Listener_position.x,0.0f,_Listener_position.y};
	FMOD_VECTOR _Velocity={_Listener_velocity.x,0.0f,_Listener_velocity.y};
	FMOD_VECTOR _Forward={0.0f,0.0f,1.0f};
	FMOD_VECTOR _Up={0.0f,1.0f,0.0f};
	_System->set3DListenerAttributes(0,&_Position,&_Velocity,&_Forward,&_Up);
	}

this->_Listener_position=_Listener_position;
this->_Listener_velocity=_Listener_velocity;
}

const vector2 &sound_manager::listener_position() const {
	return _Listener_position;
}

const vector2 &sound_manager::listener_velocity() const {
	return _Listener_velocity;
}

//Sound functions

sound *sound_manager::create_sound(const std::string &_Name,bool _Paused,
	bool _Secured) {
	return create_sound(static_cast<sound_data*>(
		_Sound_container->data(_Name)),_Paused,_Secured);
}

sound *sound_manager::create_sound(const sound_data &_Sound_data,bool _Paused,
	bool _Secured) {
	return create_sound(_Sound_container->add_data(
		_Sound_data),_Paused,_Secured);
}

sound *sound_manager::create_sound(sound_data *_Sound_data,bool _Paused,
	bool _Secured) {

sound *_Mysound=new sound(this,_Sound_data,_Paused);
add(_Mysound,_Secured);
return _Mysound;
}

sound *sound_manager::create_sound(const sound &_Sound,bool _Secured) {

sound *_Mysound=new sound(this,_Sound);
add(_Mysound,_Secured);
return _Mysound;
}

void sound_manager::clear_cache() {

	for (_Created_sounds::const_iterator _Iter=_Mycreated_sounds.begin(),
		_End=_Mycreated_sounds.end();_Iter!=_End;++_Iter) {
	
	const _Created_sound &_Mycreated_sound=(*_Iter); //Increase speed
	
	_Mycreated_sound._Fmodsound->release();
	
		//Deallocate stream data if used
		if (_Mycreated_sound._Stream_data) delete[] _Mycreated_sound._Stream_data;
	}

_Mycreated_sounds.clear();

	//Update all manageables
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter) status(
			*_Iter,manageable::NEED_UPDATE);
}

} //namespace lunar