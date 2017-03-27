/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particle_system.h
---------------------------------------------------
*/

#ifndef _PARTICLE_SYSTEM_
#define _PARTICLE_SYSTEM_

#include "manageable.h"
#include "file_container.h"

#include "attachable.h"
#include "affector_manager.h"
#include "emitter.h"

namespace lunar {

//Forward declarations
class particlesystem_manager;

class shaderprogram;
class uniform;
class attribute;
class sampler;

class particle_system:public manageable,public attachable,
	public affector_manager {

	friend particlesystem_manager;

public:

	enum TRANSFORMATION { WORLD, LOCAL };

protected:

	typedef std::list<emitter*> _Emitters;
	
	TRANSFORMATION _Transformation;
	_Emitters _Myemitters;
	
	//Default particle system shader program and variables
	//This shader program is shared by all particle systems
	
	static shaderprogram *_Particle_program;
	static uniform *_Particle_usetexture;
	static sampler *_Particle_texture;
	static attribute *_Particle_size;
	static bool _Default_program;
	
	//Protected constructor
	explicit particle_system(particlesystem_manager *_Owner,
		bool _Visible=true);
	
	//Protected alternative constructor
	particle_system(particlesystem_manager *_Owner,
		file_data *_Script_data,bool _Visible=true);
	
	//Protected copy constructor
	particle_system(const particle_system &_Particle_system);
	
	//Protected alternative copy constructor
	particle_system(particlesystem_manager *_Owner,
		const particle_system &_Particle_system);
	
	//Protected virtual destructor
	virtual ~particle_system();
	
	void copy(const particle_system &_Particle_system);
	
	virtual void update_bounds();
	
	virtual void pre_draw();
	virtual void draw();
	virtual void post_draw();
	
	static void create_defaultprogram();

public:

	typedef _Emitters::const_iterator emitters_iter; //Keep data safe
	
	particle_system &operator=(const particle_system &_Particle_system);
	
	virtual bool prepare();
	virtual geometry_stats statistics() const;
	
	virtual void update();
	virtual void update(float _Time);
	
	void start();
	void stop();
	
	void data(file_data *_Script_data);
	file_data *data() const;
	
	bool visible_particles() const;
	
	void transformation(const TRANSFORMATION &_Transformation);
	TRANSFORMATION transformation() const;
	
	//Emitter
	
	emitter *create_emitter();
	emitter *create_emitter(const emitter &_Emitter);
	
	void remove_emitter(emitter *&_Emitter);
	void clear_emitters();
	
	emitters_iter emitters_begin() const;
	emitters_iter emitters_end() const;
};

} //namespace

#endif