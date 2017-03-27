/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound.cpp
---------------------------------------------------
*/

#include "sound.h"

//Dependencies
#include "sound_manager.h"

namespace lunar {

//Protected

sound::sound(sound_manager *_Owner,sound_data *_Sound_data,
	bool _Paused):manageable(_Owner,_Sound_data),_Paused(_Paused),
		_Volume(1.0f),_Mute(false),_Min_distance(100.0f),
			_Max_distance(1000000000.0f),_Fmodsound(0),_Channel(0) {
	//Empty
}

sound::sound(const sound &_Sound):manageable(_Sound) {

_Fmodsound=0; //Request fmodsound
_Channel=0; //Request channel
copy(_Sound);
}

sound::sound(sound_manager *_Owner,const sound &_Sound):
	manageable(_Owner,_Sound) {

_Fmodsound=0; //Request fmodsound
_Channel=0; //Request channel
copy(_Sound);
}

sound::~sound() {

	if (_Channel) _Channel->stop();
}

void sound::copy(const sound &_Sound) {

_Paused=_Sound._Paused;
_Volume=_Sound._Volume;
_Mute=_Sound._Mute;

_Position=_Sound._Position;
_Velocity=_Sound._Velocity;
_Min_distance=_Sound._Min_distance;
_Max_distance=_Sound._Max_distance;
}

void sound::status_changed() {

manageable::status_changed(); //Use base functionality

	if (_Status==FAILED && _Channel) _Channel->stop();
}

//Public

sound &sound::operator=(const sound &_Sound) {

	//Worth doing
	if (this!=&_Sound) {
	
	manageable::operator=(_Sound); //Invoke base operator=
	copy(_Sound);
	
		//No need for manager update
		//Data is equal and channel is okay
		if (_Status==OKAY) {
		
			if (_Paused) stop(false);
			else play();
		
		//Update volume, mute, channel attributes and min/max distance
		volume(_Volume);
		mute(_Mute);
		attributes(_Position,_Velocity);
		distance(_Min_distance,_Max_distance);
		}
	}

return *this;
}

void sound::data(sound_data *_Sound_data) {
	manageable::data(_Sound_data); //Data contains the correct derived data type
}

sound_data *sound::data() const {
	return static_cast<sound_data*>(manageable::data());
}

void sound::play() {

	if (_Status==OKAY) {
	
	bool _Playing=false;
	
		//Check if channel is active
		if (_Channel) _Channel->isPlaying(&_Playing);
		
		//Resume
		if (_Playing) _Channel->setPaused(false);
		//Start
		else {
		
		FMOD::System *_System=0;
		_Fmodsound->getSystemObject(&_System);
		FMOD_RESULT _Result=_System->playSound(_Channel?
			FMOD_CHANNEL_REUSE:FMOD_CHANNEL_FREE,_Fmodsound,
				false,&_Channel);
		
			if (_Result==FMOD_OK) {
			
			//Update volume, mute, channel attributes and min/max distance
			volume(_Volume);
			mute(_Mute);
			attributes(_Position,_Velocity);
			distance(_Min_distance,_Max_distance);
			}
			else status(FAILED);
		}
	}

_Paused=false;
}

void sound::stop(bool _Reset) {

	if (_Status==OKAY && _Channel) {
	
	_Channel->setPaused(true);
	
		//Reset
		if (_Reset) _Channel->setPosition(0,FMOD_TIMEUNIT_MS);
	}

_Paused=true;
}

bool sound::playing() const {

bool _Playing=false;

	if (_Status==OKAY && _Channel) {
	
	_Channel->getPaused(&_Playing);
	
		//Check if channel is active
		if (!_Playing) _Channel->isPlaying(&_Playing);
	}

return _Playing;
}

void sound::volume(float _Volume) {

	if (_Volume>1.0f) this->_Volume=1.0f;
	else if (_Volume<0.0f) this->_Volume=0.0f;
	else this->_Volume=_Volume;

	if (_Status==OKAY && _Channel)
		_Channel->setVolume(this->_Volume);
}

float sound::volume() const {
	return _Volume;
}

void sound::mute(bool _Mute) {

	if (_Status==OKAY && _Channel)
		_Channel->setMute(_Mute);

this->_Mute=_Mute;
}

bool sound::mute() const {
	return _Mute;
}

void sound::attributes(const vector2 &_Position,const vector2 &_Velocity) {

	if (_Status==OKAY && _Channel) {
	
	//FMOD Ex is left handed (+x is right, +y is up and +z is forward)
	FMOD_VECTOR _Pos={_Position.x,0.0f,_Position.y};
	FMOD_VECTOR _Vel={_Velocity.x,0.0f,_Velocity.y};
	_Channel->set3DAttributes(&_Pos,&_Vel);
	}

this->_Position=_Position;
this->_Velocity=_Velocity;
}

const vector2 &sound::position() const {
	return _Position;
}

const vector2 &sound::velocity() const {
	return _Velocity;
}

void sound::distance(float _Min,float _Max) {

	if (_Status==OKAY && _Channel)
		_Channel->set3DMinMaxDistance(_Min,_Max);

_Min_distance=_Min;
_Max_distance=_Max;
}

float sound::min_distance() const {
	return _Min_distance;
}

float sound::max_distance() const {
	return _Max_distance;
}

bool sound::spectrum(float *_Bands,int _Size) {
	return (_Status==OKAY && _Channel?_Channel->getSpectrum(
		_Bands,_Size,0,FMOD_DSP_FFT_WINDOW_RECT)==FMOD_OK:false);
}

} //namespace lunar