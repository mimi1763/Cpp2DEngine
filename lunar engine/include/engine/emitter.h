/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: emitter.h
---------------------------------------------------
*/

#ifndef _EMITTER_
#define _EMITTER_

#include "affector_manager.h"
#include "listener.h"

namespace lunar {

//Forward declarations
class particle_system;
class particle;
class texture;
class animation;

class emitter:public affector_manager,public manageable_listener {

	friend particle_system;

public:

	enum TYPE { POINT, BOX, RING };

protected:

	//Emitter data
	
	TYPE _Type;
	vector2 _Position;
	vector2 _Direction;
	unsigned int _Quota;
	
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _Inner_width;
	unsigned int _Inner_height;
	
	float _Rate;
	float _Angle;
	float _Duration;
	bool _Enabled;
	
	//Particle data
	
	float _Particlesize_min;
	float _Particlesize_max;
	
	float _Lifetime_min;
	float _Lifetime_max;
	float _Velocity_min;
	float _Velocity_max;
	float _Mass_min;
	float _Mass_max;
	color _Color_begin;
	color _Color_end;
	
	texture *_Particle_texture;
	animation *_Particle_animation;
	
	//Other data
	
	particle *_Particle_begin; //Begin of particle sequence
	particle *_Particle_end; //End of particle sequence
	particle *_Particle_last; //First inactive particle
	
	float _Emission_amount;
	float _Total_duration;
	
	//Protected constructor
	emitter();
	
	//Protected copy constructor
	emitter(const emitter &_Emitter);
	
	//Protected virtual destructor
	virtual ~emitter();
	
	void copy(const emitter &_Emitter);
	
	virtual void unsubscribed(interface_base *_Interface);
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);
	
	void attach_texturable();
	void detach_texturable();
	
	void emit(unsigned int _Amount,const vector2 &_World_position);

public:

	emitter &operator=(const emitter &_Emitter);
	
	void update(float _Time);
	void update(float _Time,const vector2 &_World_position);
	
	//Emitter functions
	
	void start();
	void stop();
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void position(const vector2 &_Position);
	const vector2 &position() const;
	
	void direction(const vector2 &_Direction);
	const vector2 &direction() const;
	
	void quota(unsigned int _Quota);
	unsigned int quota() const;
	
	void rate(float _Rate);
	float rate() const;
	
	void angle(float _Angle);
	float angle() const;
	
	void duration(float _Duration);
	float duration() const;
	
	bool enabled() const;
	
	void size(unsigned int _Width,unsigned int _Height);
	
	void width(unsigned int _Width);
	unsigned int width() const;
	
	void height(unsigned int _Height);
	unsigned int height() const;
	
	void inner_size(unsigned int _Width,unsigned int _Height);
	
	void inner_width(unsigned int _Inner_width);
	unsigned int inner_width() const;
	
	void inner_height(unsigned int _Inner_height);
	unsigned int inner_height() const;
	
	//Particle functions
	
	void particle_size(float _Particlesize_min,
		float _Particlesize_max);
	float particlesize_min() const;
	float particlesize_max() const;
	
	void lifetime(float _Lifetime_min,float _Lifetime_max);
	float lifetime_min() const;
	float lifetime_max() const;
	
	void velocity(float _Velocity_min,float _Velocity_max);
	float velocity_min() const;
	float velocity_max() const;
	
	void mass(float _Mass_min,float _Mass_max);
	float mass_min() const;
	float mass_max() const;
	
	void color_range(const color &_Color_begin,const color &_Color_end);
	const color &color_begin() const;
	const color &color_end() const;
	
	void particle_texture(texture *_Particle_texture);
	texture *particle_texture() const;
	
	void particle_animation(animation *_Particle_animation);
	animation *particle_animation() const;
	
	//Other functions
	
	particle *particles_begin() const;
	particle *particles_end() const;
	particle *particles_last() const;
	
	void clear_particles();
};

} //namespace lunar

#endif