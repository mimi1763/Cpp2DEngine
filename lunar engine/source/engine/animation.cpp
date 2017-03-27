/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation.cpp
---------------------------------------------------
*/

#include "animation.h"

//Dependencies
#include "animation_manager.h"
#include "timer.h"
#include "math_utility.h"

namespace lunar {

//Protected

animation::animation(animation_manager *_Owner,animation_data *_Animation_data,
	timer *_Timer,bool _Visible):manageable(_Owner,_Animation_data),
		texturable(_Visible),_Framerate(0.0f),_Loop(true),_Reverse(false),
			_Frame(0),_Frames(0),_Gltexture(0),_Timer(_Timer),_Oncomplete(0),
				_Onframecomplete(0) {
	_Timer->subscribe(this,true); //Add secure listener
}

animation::animation(const animation &_Animation):manageable(_Animation),
	texturable(_Animation) {

_Timer=0; //No timer
_Oncomplete=0; //No callback
_Onframecomplete=0; //No callback
copy(_Animation);
}

animation::animation(animation_manager *_Owner,const animation &_Animation,
	timer *_Timer):manageable(_Owner,_Animation),texturable(_Animation),
		_Timer(_Timer) {

_Timer->subscribe(this,true); //Add secure listener
_Oncomplete=0; //No callback
_Onframecomplete=0; //No callback
copy(_Animation);
}

animation::~animation() {

_Listening=false; //Ignore notifications

	if (_Timer) _Timer->owner()->remove(_Timer);

release_oncomplete();
release_onframecomplete();
}

void animation::copy(const animation &_Animation) {

_Framerate=_Animation._Framerate;
_Loop=_Animation._Loop;
_Reverse=_Animation._Reverse;

_Frame=_Animation._Frame;
_Frames=_Animation._Frames;
_Gltexture=_Animation._Gltexture;

status(_Animation._Status); //Inherit status
}

void animation::manageable_removed(manageable *_Manageable) {

//Timer has been destroyed, notify animation

	if (_Timer==_Manageable) {
	
	_Timer=0;
	status(FAILED); //Invalidate animation
	}
	//Use base functionality
	else texturable::manageable_removed(_Manageable);
}

void animation::pre_draw() {

	//Use base functionality
	if (_Status==OKAY) texturable::pre_draw();
	//Skip texturable functionality
	else rectangle::pre_draw();
}

void animation::post_draw() {

	//Use base functionality
	if (_Status==OKAY) texturable::post_draw();
	//Skip texturable functionality
	else rectangle::post_draw();
}

//Public

animation &animation::operator=(const animation &_Animation) {

	//Worth doing
	if (this!=&_Animation) {
	
	manageable::operator=(_Animation); //Invoke base operator=
	texturable::operator=(_Animation); //Invoke base operator=
	copy(_Animation);
	
		//Copy animation timer
		if (_Timer && _Animation._Timer)
			(*_Timer)=(*_Animation._Timer);
		
		//No need for manager update
		//Data is equal and timers are okay
		if (_Status==OKAY && _Timer && _Animation._Timer) {
		
			if (_Animation._Timer->enabled()) start();
			else stop(false);
		}
	}

return *this;
}

bool animation::prepare() {

update(); //Make sure this is up to date

	//Use base functionality
	if (_Status==OKAY) return texturable::prepare();

//Fallback on rectangle if update failed
//Skip texturable functionality
return rectangle::prepare();
}

void animation::data(animation_data *_Animation_data) {
	manageable::data(_Animation_data); //Data contains the correct derived data type
}

animation_data *animation::data() const {
	return static_cast<animation_data*>(manageable::data());
}

unsigned int animation::gltexture() const {
	return (_Status==OKAY && _Frame<_Frames?_Gltexture[_Frame]:0);
}

void animation::start() {

	if (_Timer) _Timer->start();
}

void animation::stop(bool _Reset) {

	if (_Reset) _Frame=(_Reverse?(_Frames>0?_Frames-1:0):0);
	if (_Timer) _Timer->stop(_Reset);
}

float animation::elapsed() {
	return (duration()*progress());
}

void animation::next_frame() {
	frame(_Reverse?(_Frame>0?_Frame-1:_Frame):_Frame+1);
}

void animation::previous_frame() {
	frame(_Reverse?_Frame+1:(_Frame>0?_Frame-1:_Frame));
}

float animation::progress() const {

float _Progress=(_Gltexture && _Timer?(_Frame<_Frames?
	math::lerp((float)_Frame/_Frames,(float)(_Frame+1)/_Frames,
		_Timer->time()/_Timer->interval()):1.0f):0.0f);
return (_Reverse?1.0f-_Progress:_Progress);
}

float animation::frame_progress() const {

float _Progress=(_Gltexture?(_Frame<_Frames && _Frames>1?
	(float)_Frame/(_Frames-1):1.0f):0.0f);
return (_Reverse?1.0f-_Progress:_Progress);
}

void animation::framerate(float _Framerate) {

this->_Framerate=_Framerate;

	//Update timer interval
	if (_Timer) _Timer->interval(duration()/_Frames);
}

float animation::framerate() const {
	return _Framerate;
}

void animation::duration(float _Duration) {
	framerate(_Frames/_Duration);
}

float animation::duration() const {
	return (_Frames/_Framerate);
}

void animation::loop(bool _Loop) {
	this->_Loop=_Loop;
}

bool animation::loop() const {
	return _Loop;
}

void animation::reverse(bool _Reverse) {
	this->_Reverse=_Reverse;
}

bool animation::reverse() const {
	return _Reverse;
}

void animation::frame(unsigned int _Frame) {

	if (_Timer) {
	
	bool _Enabled=_Timer->enabled();
	_Timer->stop(); //Reset timer
	
		if (_Enabled) _Timer->start();
	}

this->_Frame=(_Frame<_Frames?_Frame:_Frames-1);
}

unsigned int animation::frame() const {
	return _Frame;
}

unsigned int animation::frames() const {
	return _Frames;
}

timer *animation::animation_timer() const {
	return _Timer;
}

//Callbacks

void animation::oncomplete(static_callback<animation*>::function _Function) {

	if (_Oncomplete) release_oncomplete();

_Oncomplete=new static_callback<animation*>(_Function);
}

void animation::onframecomplete(static_callback<animation*>::function _Function) {

	if (_Onframecomplete) release_onframecomplete();

_Onframecomplete=new static_callback<animation*>(_Function);
}

void animation::release_oncomplete() {

	if (_Oncomplete) {
	
	delete _Oncomplete;
	_Oncomplete=0;
	}
}

void animation::release_onframecomplete() {

	if (_Onframecomplete) {
	
	delete _Onframecomplete;
	_Onframecomplete=0;
	}
}

} //namespace lunar