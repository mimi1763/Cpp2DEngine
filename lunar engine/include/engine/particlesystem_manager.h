/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particlesystem_manager.h
---------------------------------------------------
*/

#ifndef _PARTICLESYSTEM_MANAGER_
#define _PARTICLESYSTEM_MANAGER_

#include "manager.h"
#include "particle_system.h"

namespace lunar {

//Forward declarations
class core;
class texture_manager;
class animation_manager;
class script_parser;

class particlesystem_manager:public manager {

	friend core;

private:

	static particlesystem_manager *_Instance; //Grant core exclusive access

protected:

	file_container *_Script_container;
	texture_manager *_Texture_manager;
	animation_manager *_Animation_manager;
	
	script_parser *_Script_parser;
	
	void copy(const particlesystem_manager &_Particlesystem_manager);
	
	//Script specific
	static void init_particlescript(script_parser *_Script_parser);
	
	bool init_particlesystem(particle_system *_Particle_system,
		file_data *_Script_data);

public:

	//Constructor
	particlesystem_manager();
	
	//Alternative constructor
	particlesystem_manager(texture_manager *_Texture_manager,
		animation_manager *_Animation_manager);
	
	//Copy constructor
	particlesystem_manager(const particlesystem_manager &_Particlesystem_manager);
	
	//Virtual destructor
	virtual ~particlesystem_manager();
	
	particlesystem_manager &operator=(const particlesystem_manager &_Particlesystem_manager);
	
	static particlesystem_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void update(float _Time);
	
	void script_source(file_container *_Script_container);
	file_container *script_source() const;
	
	//Particle system functions
	
	particle_system *create_particlesystem(bool _Visible=true,
		bool _Secured=false);
	
	particle_system *create_particlesystem(const std::string &_Script_name,
		bool _Visible=true,bool _Secured=false);
	
	particle_system *create_particlesystem(const file_data &_Script_data,
		bool _Visible=true,bool _Secured=false);
	
	particle_system *create_particlesystem(file_data *_Script_data,
		bool _Visible=true,bool _Secured=false);
	
	particle_system *create_particlesystem(const particle_system &_Particle_system,
		bool _Secured=false);
};

} //namespace

#endif