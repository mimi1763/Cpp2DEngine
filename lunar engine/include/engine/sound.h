/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound.h
---------------------------------------------------
*/

#ifndef _SOUND_
#define _SOUND_

#include "fmod.hpp" //C++ API
#include "manageable.h"
#include "sound_container.h"

#include "vector2.h"

namespace lunar {

//Forward declarations
class sound_manager;

class sound:public manageable {

	friend sound_manager;

protected:

	bool _Paused;
	float _Volume;
	bool _Mute;
	
	vector2 _Position;
	vector2 _Velocity;
	float _Min_distance;
	float _Max_distance;
	
	FMOD::Sound *_Fmodsound;
	FMOD::Channel *_Channel;
	
	//Protected constructor
	sound(sound_manager *_Owner,sound_data *_Sound_data,
		bool _Paused=true);
	
	//Protected copy constructor
	sound(const sound &_Sound);
	
	//Protected alternative copy constructor
	sound(sound_manager *_Owner,const sound &_Sound);
	
	//Protected virtual destructor
	virtual ~sound();
	
	void copy(const sound &_Sound);
	
	virtual void status_changed();

public:

	sound &operator=(const sound &_Sound);
	
	void data(sound_data *_Sound_data);
	sound_data *data() const;
	
	void play();
	void stop(bool _Reset=true);
	bool playing() const;
	
	void volume(float _Volume);
	float volume() const;
	
	void mute(bool _Mute);
	bool mute() const;
	
	void attributes(const vector2 &_Position,const vector2 &_Velocity=vector2::ZERO);
	const vector2 &position() const;
	const vector2 &velocity() const;
	
	void distance(float _Min=100.0f,float _Max=1000000000.0f);
	float min_distance() const;
	float max_distance() const;
	
	bool spectrum(float *_Bands,int _Size);
};

} //namespace lunar

#endif