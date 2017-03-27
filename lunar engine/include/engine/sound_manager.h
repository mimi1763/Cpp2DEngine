/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sound_manager.h
---------------------------------------------------
*/

#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include "manager.h"
#include "sound.h"

namespace lunar {

//Forward declarations
class core;

class sound_manager:public manager {

	friend core;

private:

	static sound_manager *_Instance; //Grant core exclusive access

protected:

	struct _Created_sound {
	
	sound_data *_Sound_data;
	FMOD::Sound *_Fmodsound;
	char *_Stream_data;
	};
	
	typedef std::list<_Created_sound> _Created_sounds;
	
	_Created_sounds _Mycreated_sounds;
	sound_container *_Sound_container;
	
	vector2 _Listener_position;
	vector2 _Listener_velocity;
	
	FMOD::System *_System;
	bool _Supported;
	
	void copy(const sound_manager &_Sound_manager);
	
	//Fmod specific
	bool init_fmod();
	
	bool create_fmodsound(sound_data *_Sound_data);
	void init_sound(sound *_Sound,const _Created_sound &_Mycreated_sound) const;

public:

	static const int MAX_CHANNELS;
	
	//Constructor
	sound_manager();
	
	//Copy constructor
	sound_manager(const sound_manager &_Sound_manager);
	
	//Virtual destructor
	virtual ~sound_manager();
	
	sound_manager &operator=(const sound_manager &_Sound_manager);
	
	static sound_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void sound_source(sound_container *_Sound_container);
	sound_container *sound_source() const;
	
	void volume(float _Volume);
	
	void listener_attributes(const vector2 &_Listener_position,
		const vector2 &_Listener_velocity=vector2::ZERO);
	const vector2 &listener_position() const;
	const vector2 &listener_velocity() const;
	
	//Sound functions
	
	sound *create_sound(const std::string &_Name,bool _Paused=true,
		bool _Secured=false);
	
	sound *create_sound(const sound_data &_Sound_data,bool _Paused=true,
		bool _Secured=false);
	
	sound *create_sound(sound_data *_Sound_data,bool _Paused=true,
		bool _Secured=false);
	
	sound *create_sound(const sound &_Sound,bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif