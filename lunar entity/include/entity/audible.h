/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: audible.h
---------------------------------------------------
*/

#ifndef _AUDIBLE_
#define _AUDIBLE_

#include <list>

//Dependencies (engine)
#include "sound.h"

//Forward declarations (engine)
namespace lunar {
	class render_node;
} //namespace lunar

//Forward declarations (entity)
class entity_manager;

using namespace lunar;

class audible {

public:

	struct sound_group {
	
		friend audible;
	
	private:
	
		typedef std::list<sound*> _Sounds;
		
		audible *_Owner;
		std::string _Name;
		float _Volume;
		bool _Playing;
		
		_Sounds _Mysounds;
		
		//Private constructor
		sound_group(audible *_Owner,const std::string &_Name);
		
		//Private copy constructor
		sound_group(const sound_group &_Sound_group);
		
		//Private alternative copy constructor
		sound_group(audible *_Owner,const std::string &_Name,
			const sound_group &_Sound_group);
		
		//Private destructor
		~sound_group();
		
		void copy(const sound_group &_Sound_group);
	
	public:
	
		typedef _Sounds::const_iterator sounds_iter; //Keep data safe
		
		sound_group &operator=(const sound_group &_Sound_group);
		
		audible *owner() const;
		const std::string &name() const;
		
		void play(bool _Base=true);
		void stop(bool _Reset=true,bool _Base=true);
		
		void volume(float _Volume,bool _Base=true);
		float volume() const;
		
		bool playing() const;
		
		//Sounds
		
		sound *create_sound(resource_data *_Resource_data,bool _Paused=true);
		sound *create_sound(const sound &_Sound);
		
		void remove_sound(sound *&_Sound);
		void clear_sounds();
		
		sounds_iter sounds_begin() const;
		sounds_iter sounds_end() const;
	};

protected:

	typedef std::list<sound_group*> _Sound_groups;
	
	float _Volume;
	bool _Playing;
	
	_Sound_groups _Mysound_groups;
	
	//Protected constructor
	audible();
	
	//Protected copy constructor
	audible(const audible &_Audible);
	
	//Protected virtual destructor
	virtual ~audible();
	
	void copy(const audible &_Audible);
	
	virtual sound_manager *sound_owner() const;
	virtual render_node *sound_node() const;

public:

	typedef _Sound_groups::const_iterator soundgroups_iter; //Keep data safe
	
	audible &operator=(const audible &_Audible);
	
	virtual void update(float _Time);
	
	void play(bool _Base=true);
	void stop(bool _Reset=true,bool _Base=true);
	
	void volume(float _Volume,bool _Base=true);
	float volume() const;
	
	bool playing() const;
	
	//Sound groups
	
	sound_group *create_soundgroup(const std::string &_Name);
	sound_group *create_soundgroup(const std::string &_Name,
		const sound_group &_Sound_group);
	
	void remove_soundgroup(sound_group *&_Sound_group);
	void clear_soundgroups();
	
	soundgroups_iter soundgroups_begin() const;
	soundgroups_iter soundgroups_end() const;
	
	sound_group *soundgroup_pointer(const std::string &_Name) const;
};

#endif