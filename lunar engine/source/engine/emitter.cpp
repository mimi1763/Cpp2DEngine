/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: emitter.cpp
---------------------------------------------------
*/

#include "emitter.h"

//Dependencies
#include <cstring> //memcpy
#include <cmath>

#include "particle.h"
#include "texture.h"
#include "animation.h"
#include "math_utility.h"

namespace lunar {

//Protected

emitter::emitter():_Type(POINT),_Direction(vector2::UNIT_Y),_Quota(100),
	_Width(100),_Height(100),_Inner_width(0),_Inner_height(0),_Rate(10.0f),
		_Angle(0.0f),_Duration(0.0f),_Enabled(false),_Particlesize_min(2.0f),
			_Particlesize_max(2.0f),_Lifetime_min(1.0f),_Lifetime_max(2.0f),
				_Velocity_min(50.0f),_Velocity_max(100.0f),_Mass_min(1.0f),
					_Mass_max(1.0f),_Particle_texture(0),_Particle_animation(0),
						_Particle_begin(0),_Particle_end(0),_Particle_last(0),
							_Emission_amount(0),_Total_duration(0) {
	//Empty
}

emitter::emitter(const emitter &_Emitter):
	affector_manager(_Emitter) {

//Not copyable
_Particle_begin=0;
_Particle_end=0;
_Particle_last=0;

copy(_Emitter);
attach_texturable();
}

emitter::~emitter() {

detach_texturable();
clear_particles();
}

void emitter::copy(const emitter &_Emitter) {

//Emitter data

_Type=_Emitter._Type;
_Position=_Emitter._Position;
_Direction=_Emitter._Direction;
_Quota=_Emitter._Quota;

_Width=_Emitter._Width;
_Height=_Emitter._Height;
_Inner_width=_Emitter._Inner_width;
_Inner_height=_Emitter._Inner_height;

_Rate=_Emitter._Rate;
_Angle=_Emitter._Angle;
_Duration=_Emitter._Duration;
_Enabled=_Emitter._Enabled;

//Particle data

_Particlesize_min=_Emitter._Particlesize_min;
_Particlesize_max=_Emitter._Particlesize_max;

_Lifetime_min=_Emitter._Lifetime_min;
_Lifetime_max=_Emitter._Lifetime_max;
_Velocity_min=_Emitter._Velocity_min;
_Velocity_max=_Emitter._Velocity_max;
_Mass_min=_Emitter._Mass_min;
_Mass_max=_Emitter._Mass_max;
_Color_begin=_Emitter._Color_begin;
_Color_end=_Emitter._Color_end;

_Particle_texture=_Emitter._Particle_texture;
_Particle_animation=_Emitter._Particle_animation;

//Other data

_Emission_amount=_Emitter._Emission_amount;
_Total_duration=_Emitter._Total_duration;
}

void emitter::unsubscribed(interface_base *_Interface) {

//Particle texture/animation has been unsubscribed, notify emitter

	if (_Particle_texture==_Interface || _Particle_animation==_Interface) {
	
	_Particle_texture=0; //Remove particle texture
	_Particle_animation=0; //Remove particle animation
	}
}

void emitter::manageable_changed(manageable *_Manageable) {
	//Empty - override if needed
}

void emitter::manageable_statuschanged(manageable *_Manageable) {
	//Empty - override if needed
}

void emitter::manageable_removed(manageable *_Manageable) {

//Particle texture/animation has been destroyed, notify emitter

emitter::unsubscribed(_Manageable); //Reuse functionality
}

void emitter::attach_texturable() {

//Can't add secure listeners because emitter doesn't own texturable
//Secure listeners will not be broken before texturable is out of scope
//If public link is boken the emitter will remove texturable

	//Add listener
	if (_Particle_texture) _Particle_texture->subscribe(this); 
	else if (_Particle_animation) _Particle_animation->subscribe(this);
}

void emitter::detach_texturable() {

_Listening=false; //Ignore notifications

	//Remove listener
	if (_Particle_texture) _Particle_texture->unsubscribe(this); 
	else if (_Particle_animation) _Particle_animation->unsubscribe(this);

_Listening=true;
}

void emitter::emit(unsigned int _Amount,const vector2 &_World_position) {

	//Need allocation?
	if (!_Particle_begin) {
	
	_Particle_last=_Particle_begin=new particle[_Quota]; //Allocate
	_Particle_end=_Particle_begin+_Quota;
	}

particle *_Particle;

	for (unsigned int i=0;i<_Amount;++i) {
	
		if (_Particle_last==_Particle_end) {
		
		_Emission_amount=0; //Nothing more to emit, reset emission amount
		return;
		}
	
	_Particle=_Particle_last++; //Reuse particle
	_Particle->_Position=_World_position+_Position;
	
		if (_Type!=POINT) {
		
		float _Half_width=(float)_Width*0.5f;
		float _Half_height=(float)_Height*0.5f;
		float _Half_innerwidth=(float)_Inner_width*0.5f;
		float _Half_innerheight=(float)_Inner_height*0.5f;
		
			if (_Type==BOX) {
			
			float _Xrand=math::random(-1.0f,1.0f);
			float _Yrand=math::random(-1.0f,1.0f);
			
				//50/50 for uniform distribution
				if (math::random(0,1)) {
				
				_Particle->_Position.x+=_Half_width*_Xrand; //Place on x-axis
				
					if (_Particle->_Position.x>-_Half_innerwidth && _Particle->
						_Position.x<_Half_innerwidth) _Particle->_Position.y+=(
							_Half_height-_Half_innerheight)*_Yrand+_Half_innerheight*(
								_Yrand>=0?1:-1); //Place on bounded y-axis
					else _Particle->_Position.y+=_Half_height*_Yrand;
				}
				else {
				
				_Particle->_Position.y+=_Half_height*_Yrand; //Place on y-axis
				
					if (_Particle->_Position.y>-_Half_innerheight && _Particle->
						_Position.y<_Half_innerheight) _Particle->_Position.x+=(
							_Half_width-_Half_innerwidth)*_Xrand+_Half_innerwidth*(
								_Xrand>=0?1:-1); //Place on bounded x-axis
					else _Particle->_Position.x+=_Half_width*_Xrand;
				}
			}
			else if (_Type==RING) {
			
			float _Theta=math::TWO_PI*math::random(); //Random [0,1]
			float _Rand=sqrt(math::random()); //Random [0,1], sqrt() is used for uniform distribution
			float _Xradius=(_Half_width-_Half_innerwidth)*_Rand+_Half_innerwidth;
			float _Yradius=(_Half_height-_Half_innerheight)*_Rand+_Half_innerheight;
			
			_Particle->_Position.x+=_Xradius*math::cosine(_Theta);
			_Particle->_Position.y+=_Yradius*math::sine(_Theta);
			}
		}
	
	_Particle->_Direction=(_Angle!=0.0f?_Direction.deviant(
		math::random(-_Angle,_Angle)):_Direction);
	_Particle->_Direction*=math::random(_Velocity_min,_Velocity_max); //Increase vector with velocity
	_Particle->_Size=math::random(_Particlesize_min,_Particlesize_max);
	
	_Particle->_Lifetime=math::random(_Lifetime_min,_Lifetime_max);
	_Particle->_Mass=math::random(_Mass_min,_Mass_max);
	_Particle->_Color=_Color_begin.mix(_Color_end,math::random()); //Random [0,1]
	
	_Particle->_Total_lifetime=_Particle->_Lifetime; //Set total as initial lifetime
	}
}

//Public

emitter &emitter::operator=(const emitter &_Emitter) {

	//Worth doing
	if (this!=&_Emitter) {
	
	affector_manager::operator=(_Emitter); //Invoke base operator=
	
	bool _Changed=(_Particle_texture!=_Emitter._Particle_texture ||
		_Particle_animation!=_Emitter._Particle_animation);
	
	//Critical functionality
	//Must be called before copy (need both old and new quota)
	quota(_Emitter._Quota);
	
		if (_Changed) detach_texturable();
	
	copy(_Emitter);
	
		if (_Changed) attach_texturable();
	}

return *this;
}

void emitter::update(float _Time) {
	update(_Time,vector2::ZERO);
}

void emitter::update(float _Time,const vector2 &_World_position) {

	if (_Enabled) {
	
	_Total_duration+=_Time;
	
		if (_Duration>0.0f && _Total_duration>=_Duration) stop();
		else {
		
		_Emission_amount+=_Rate*_Time;
		
			if (_Emission_amount>=1.0f) {
			
			emit((unsigned int)_Emission_amount,_World_position); //Emit # of particles in current emission amount
			_Emission_amount-=(int)_Emission_amount; //Only the fractional part is stored for the next frame
			}
		}
	}
	
	//All particles must evolve even if the emitter is not enabled
	for (particle *_Particle=_Particle_begin;_Particle!=_Particle_last;
		++_Particle) {
	
	_Particle->_Lifetime-=_Time;
	
		//Evolve particle?
		if (_Particle->_Lifetime>0) _Particle->_Position+=_Particle->
			_Direction*_Time;
		else {
		
		(*_Particle)=*(--_Particle_last); //Make copy from last active particle
		
			if (_Particle==_Particle_last) break;
		}
	}
	
	if (!_Enabled && _Particle_last && _Particle_last==_Particle_begin) 
		clear_particles(); //Clean up
	
	//Local affectors
	for (_Affectors::const_iterator _Iter=_Myaffectors.begin(),
		_End=_Myaffectors.end();_Iter!=_End;++_Iter) (*_Iter)->affect(
			_Particle_begin,_Particle_last,_Time);
}

void emitter::start() {
	_Enabled=true;
}

void emitter::stop() {

_Enabled=false;
_Emission_amount=0; //Reset emission amount
_Total_duration=0; //Reset total duration
}

void emitter::type(const TYPE &_Type) {
	this->_Type=_Type;
}

emitter::TYPE emitter::type() const {
	return _Type;
}

void emitter::position(const vector2 &_Position) {
	this->_Position=_Position;
}

const vector2 &emitter::position() const {
	return _Position;
}

void emitter::direction(const vector2 &_Direction) {
	this->_Direction=_Direction;
}

const vector2 &emitter::direction() const {
	return _Direction;
}

void emitter::quota(unsigned int _Quota) {

	//Reallocation required?
	if (_Particle_begin && this->_Quota!=_Quota) {
	
	particle *_Source_begin=_Particle_begin;
	size_t _Difference=_Particle_last-_Particle_begin;
	
	_Particle_last=_Particle_begin=new particle[_Quota]; //Reallocate
	_Particle_end=_Particle_begin+_Quota;
	
	memcpy(_Particle_begin,_Source_begin,(this->_Quota<=_Quota?
		this->_Quota*sizeof(particle):_Quota*sizeof(particle)));
	delete[] _Source_begin;
	
	_Particle_last+=(_Quota<=_Difference?_Quota:_Difference);
	}

this->_Quota=_Quota;
}

unsigned int emitter::quota() const {
	return _Quota;
}

void emitter::rate(float _Rate) {
	this->_Rate=_Rate;
}

float emitter::rate() const {
	return _Rate;
}

void emitter::angle(float _Angle) {
	this->_Angle=_Angle;
}

float emitter::angle() const {
	return _Angle;
}

void emitter::duration(float _Duration) {
	this->_Duration=_Duration;
}

float emitter::duration() const {
	return _Duration;
}

bool emitter::enabled() const {
	return _Enabled;
}

void emitter::size(unsigned int _Width,
	unsigned int _Height) {

width(_Width);
height(_Height);
}

void emitter::width(unsigned int _Width) {

this->_Width=_Width;

	if (_Width<_Inner_width) _Inner_width=_Width;
}

unsigned int emitter::width() const {
	return _Width;
}

void emitter::height(unsigned int _Height) {

this->_Height=_Height;

	if (_Height<_Inner_height) _Inner_height=_Height;
}

unsigned int emitter::height() const {
	return _Height;
}

void emitter::inner_size(unsigned int _Inner_width,
	unsigned int _Inner_height) {

inner_width(_Inner_width);
inner_height(_Inner_height);
}

void emitter::inner_width(unsigned int _Inner_width) {

this->_Inner_width=_Inner_width;

	if (_Inner_width>_Width) _Width=_Inner_width;
}

unsigned int emitter::inner_width() const {
	return _Inner_width;
}

void emitter::inner_height(unsigned int _Inner_height) {

this->_Inner_height=_Inner_height;

	if (_Inner_height>_Height) _Height=_Inner_height;
}

unsigned int emitter::inner_height() const {
	return _Inner_height;
}

void emitter::particle_size(float _Particlesize_min,
	float _Particlesize_max) {

this->_Particlesize_min=_Particlesize_min;
this->_Particlesize_max=(_Particlesize_max<_Particlesize_min?
	_Particlesize_min:_Particlesize_max);
}

float emitter::particlesize_min() const {
	return _Particlesize_min;
}

float emitter::particlesize_max() const {
	return _Particlesize_max;
}

void emitter::lifetime(float _Lifetime_min,
	float _Lifetime_max) {

this->_Lifetime_min=_Lifetime_min;
this->_Lifetime_max=(_Lifetime_max<_Lifetime_min?
	_Lifetime_min:_Lifetime_max);
}

float emitter::lifetime_min() const {
	return _Lifetime_min;
}

float emitter::lifetime_max() const {
	return _Lifetime_max;
}

void emitter::velocity(float _Velocity_min,
	float _Velocity_max) {

this->_Velocity_min=_Velocity_min;
this->_Velocity_max=(_Velocity_max<_Velocity_min?
	_Velocity_min:_Velocity_max);
}

float emitter::velocity_min() const {
	return _Velocity_min;
}

float emitter::velocity_max() const {
	return _Velocity_max;
}

void emitter::mass(float _Mass_min,float _Mass_max) {

this->_Mass_min=_Mass_min;
this->_Mass_max=(_Mass_max<_Mass_min?_Mass_min:_Mass_max);
}

float emitter::mass_min() const {
	return _Mass_min;
}

float emitter::mass_max() const {
	return _Mass_max;
}

void emitter::color_range(const color &_Color_begin,
	const color &_Color_end) {

this->_Color_begin=_Color_begin;
this->_Color_end=_Color_end;
}

const color &emitter::color_begin() const {
	return _Color_begin;
}

const color &emitter::color_end() const {
	return _Color_end;
}

void emitter::particle_texture(texture *_Particle_texture) {

	if (this->_Particle_texture!=_Particle_texture) {
	
	detach_texturable();
	this->_Particle_texture=_Particle_texture;
	_Particle_animation=0; //Can't have both
	attach_texturable();
	}
}

texture *emitter::particle_texture() const {
	return _Particle_texture;
}

void emitter::particle_animation(animation *_Particle_animation) {

	if (this->_Particle_animation!=_Particle_animation) {
	
	detach_texturable();
	this->_Particle_animation=_Particle_animation;
	_Particle_texture=0; //Can't have both
	attach_texturable();
	}
}

animation *emitter::particle_animation() const {
	return _Particle_animation;
}

particle *emitter::particles_begin() const {
	return _Particle_begin;
}

particle *emitter::particles_end() const {
	return _Particle_end; //Return end of sequence
}

particle *emitter::particles_last() const {
	return _Particle_last; //Return end of active sequence
}

void emitter::clear_particles() {

	if (_Particle_begin) delete[] _Particle_begin; //Deallocate

_Particle_begin=_Particle_end=_Particle_last=0;
}

} //namespace lunar