/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particle_system.cpp
---------------------------------------------------
*/

#include "particle_system.h"

//Dependencies
#include "system.h"

#include "particlesystem_manager.h"
#include "particle.h"

#include "shaderprogram_manager.h"
#include "texture.h"
#include "animation.h"

#include "render_node.h"
#include "support.h"

namespace lunar {

//Protected

//Initialize these when used first time
shaderprogram *particle_system::_Particle_program=0;
uniform *particle_system::_Particle_usetexture=0;
sampler *particle_system::_Particle_texture=0;
attribute *particle_system::_Particle_size=0;
bool particle_system::_Default_program=false;

particle_system::particle_system(particlesystem_manager *_Owner,
	bool _Visible):manageable(_Owner,0),attachable(_Visible),
		_Transformation(WORLD) {
	//Empty
}

particle_system::particle_system(particlesystem_manager *_Owner,
	file_data *_Script_data,bool _Visible):
		manageable(_Owner,_Script_data),attachable(_Visible),
			_Transformation(WORLD) {
	//Empty
}

particle_system::particle_system(const particle_system &_Particle_system):
	manageable(_Particle_system),attachable(_Particle_system),
		affector_manager(_Particle_system) {
	copy(_Particle_system);
}

particle_system::particle_system(particlesystem_manager *_Owner,
	const particle_system &_Particle_system):manageable(_Owner,
		_Particle_system),attachable(_Particle_system),
			affector_manager(_Particle_system) {
	copy(_Particle_system);
}

particle_system::~particle_system() {
	clear_emitters();
}

void particle_system::copy(const particle_system &_Particle_system) {
	_Transformation=_Particle_system._Transformation;
}

void particle_system::update_bounds() {
	//Empty - override if needed
}

void particle_system::pre_draw() {

	//Cancel relative node matrix, use world coordinates
	if (_Transformation==WORLD) glPopMatrix();

_Default_program=false;

	//Use default program if no user programs is enabled
	if (!_Shaderprograms_enabled && support::shader()!=support::NONE) {
	
		if (!_Particle_program) create_defaultprogram();
	
	_Particle_program->update();
	
		if (_Particle_program->status()==manageable::OKAY) {
		
			if (support::shader()==support::OPENGL) {
			
			glUseProgram(_Particle_program->glprogram()); //Use default program
			_Default_program=true;
			}
			else if (support::shader()==support::ARB) {
			
			glUseProgramObjectARB(_Particle_program->glprogram()); //Use default program
			_Default_program=true;
			}
		}
	}
	
	if (support::point_sprite()==support::OPENGL) {
	
	glEnable(GL_POINT_SPRITE); //Enable point sprite
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); //Enable varying point size
	glTexEnvf(GL_POINT_SPRITE,GL_COORD_REPLACE,GL_TRUE); //Enable sprite texcoords
	}
	else if (support::point_sprite()==support::ARB) {
	
	glEnable(GL_POINT_SPRITE_ARB); //Enable point sprite
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB); //Enable varying point size
	glTexEnvf(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE_ARB,GL_TRUE); //Enable sprite texcoords
	}

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);
}

void particle_system::draw() {

glPushMatrix();
emitter *_Emitter;

	//Draw all emitters in this particle system
	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) {
	
	_Emitter=(*_Iter); //Increase speed
	
	particle *_Particle=_Emitter->particles_begin(); //First particle to draw
	size_t _Count=_Emitter->particles_last()-_Particle; //Number of particles
	
		//Nothing to draw, continue to next emitter
		if (_Count==0) continue; 
	
	texture *_Texture=0;
	animation *_Animation=0;
	
		//Only check for textures if point sprite is supported
		if (support::point_sprite()!=support::NONE) {
		
		_Texture=_Emitter->particle_texture();
		_Animation=_Emitter->particle_animation();
		}
		
		//Set default program variable values
		if (_Default_program) {
		
		int _Use_texture=1;
		
			if (_Texture) _Particle_texture->texture_data(_Texture);
			else if (_Animation) _Particle_texture->animation_data(_Animation);
			else _Use_texture=0; //Don't use texture
		
		_Particle_usetexture->data(&_Use_texture); //Set uniform data
		_Particle_size->data(&_Particle->_Size); //Set attribute data
		
		shaderprogram_manager::instance()->update_shadervariables(_Particle_program);
		
		//No need to disable GL_TEXTURE_2D later
		_Texture=0;
		_Animation=0;
		}
		else {
		
		manageable *_Manageable=0;
		
			if (_Texture) _Manageable=_Texture;
			else if (_Animation) _Manageable=_Animation;
			
			//Draw particles with texture/animation
			if (_Manageable) {
			
			_Manageable->update();
			
				if (_Manageable->status()==manageable::OKAY) {
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,_Texture?_Texture->gltexture():
					_Animation->gltexture());
				}
				else {
				
				//No need to disable GL_TEXTURE_2D later
				_Texture=0;
				_Animation=0;
				}
			}
		}
	
	glPointSize((_Emitter->particlesize_min()+_Emitter->
		particlesize_max())*0.5f); //Default particle size if not overridden by shader
	
	glVertexPointer(2,GL_FLOAT,sizeof(particle),_Particle->_Position.ptr);
	glColorPointer(4,GL_FLOAT,sizeof(particle),_Particle->_Color.ptr);
	
	glDrawArrays(GL_POINTS,0,(GLsizei)_Count); //Draw particles
	
		if (_Texture || _Animation) glDisable(GL_TEXTURE_2D);
	}

glPopMatrix();
}

void particle_system::post_draw() {

glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);

	if (support::point_sprite()==support::OPENGL) {
	
	glTexEnvf(GL_POINT_SPRITE,GL_COORD_REPLACE,GL_FALSE); //Disable sprite texcoords
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE); //Disable varying point size
	glDisable(GL_POINT_SPRITE); //Disable point sprite
	}
	else if (support::point_sprite()==support::ARB) {
	
	glTexEnvf(GL_POINT_SPRITE_ARB,GL_COORD_REPLACE_ARB,GL_FALSE); //Disable sprite texcoords
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB); //Disable varying point size
	glDisable(GL_POINT_SPRITE_ARB); //Disable point sprite
	}
	
	//Default program used, disable states
	if (_Default_program) {
	
		if (support::shader()==support::OPENGL) {
		
		glDisableVertexAttribArray(1);
		glUseProgram(0);
		}
		else if (support::shader()==support::ARB) {
		
		glDisableVertexAttribArrayARB(1);
		glUseProgramObjectARB(0);
		}
	}
	
	//Restore relative matrix for next attachable
	if (_Transformation==WORLD && _Parent_node &&
		this!=*(--_Parent_node->attachables_end())) {
	
	glPushMatrix();
	glLoadMatrixf(_Parent_node->glmatrix());
	}
}

void particle_system::create_defaultprogram() {

_Particle_program=shaderprogram_manager::instance()->create_shaderprogram(
	"default_particle",true);

//Create uniform (draw with texture)
_Particle_usetexture=_Particle_program->create_uniform("useTexture");
_Particle_usetexture->auto_update(true); //Shared

//Create sampler (the texture to draw)
_Particle_texture=_Particle_program->create_sampler("texture");
_Particle_texture->auto_update(true); //Shared

//Create attribute (varying particle size)
_Particle_size=_Particle_program->create_attribute("pointSize",
	1,sizeof(particle));
_Particle_size->auto_update(true); //Shared
}

//Public

particle_system &particle_system::operator=(const particle_system &_Particle_system) {

	//Worth doing
	if (this!=&_Particle_system) {
	
	manageable::operator=(_Particle_system); //Invoke base operator=
	attachable::operator=(_Particle_system); //Invoke base operator=
	affector_manager::operator=(_Particle_system); //Invoke base operator=
	copy(_Particle_system);
	}

return *this;
}

bool particle_system::prepare() {
	//Check if there is something to draw
	return (visible_particles()?attachable::prepare():false);
}

attachable::geometry_stats particle_system::statistics() const {

geometry_stats _Geometry_stats={0};

emitter *_Emitter;

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) {
	
	_Emitter=(*_Iter); //Increase speed
	
	_Geometry_stats.vertices+=(unsigned int)
		(_Emitter->_Particle_last-_Emitter->_Particle_begin);
	}

return _Geometry_stats;
}

void particle_system::update() {
	manageable::update(); //Must redirect because of name reuse
}

void particle_system::update(float _Time) {

	//No position to update from
	if (!_Parent_node) return;

const vector2 &_Position=(_Transformation==WORLD?
	_Parent_node->world_position():vector2::ZERO);

emitter *_Emitter;

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) {
	
	_Emitter=(*_Iter); //Increase speed
	_Emitter->update(_Time,_Position);
	
		//Global affectors
		for (_Affectors::const_iterator _Aiter=_Myaffectors.begin(),
			_Aend=_Myaffectors.end();_Aiter!=_Aend;++_Aiter)
				(*_Aiter)->affect(_Emitter->_Particle_begin,
					_Emitter->_Particle_last,_Time);
	}
}

void particle_system::start() {

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter)
			(*_Iter)->start();
}

void particle_system::stop() {

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter)
			(*_Iter)->stop();
}

void particle_system::data(file_data *_Script_data) {
	manageable::data(_Script_data); //Data contains the correct derived data type
}

file_data *particle_system::data() const {
	return static_cast<file_data*>(manageable::data());
}

bool particle_system::visible_particles() const {

emitter *_Emitter;

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) {
	
	_Emitter=(*_Iter); //Increase speed
	
		if (_Emitter->_Particle_last-_Emitter->_Particle_begin>0) return true;
	}

return false;
}

void particle_system::transformation(const TRANSFORMATION &_Transformation) {
	this->_Transformation=_Transformation;
}

particle_system::TRANSFORMATION particle_system::transformation() const {
	return _Transformation;
}

//Emitter

emitter *particle_system::create_emitter() {

emitter *_Myemitter=new emitter();
_Myemitters.push_back(_Myemitter);
return _Myemitter;
}

emitter *particle_system::create_emitter(const emitter &_Emitter) {

emitter *_Myemitter=new emitter(_Emitter);
_Myemitters.push_back(_Myemitter);
return _Myemitter;
}

void particle_system::remove_emitter(emitter *&_Emitter) {

	for (_Emitters::iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Emitter) {
		
		delete _Emitter;
		_Emitter=0; //Set referenced pointer to null
		
		_Myemitters.erase(_Iter);
		return;
		}
	}
}

void particle_system::clear_emitters() {

	for (_Emitters::const_iterator _Iter=_Myemitters.begin(),
		_End=_Myemitters.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myemitters.clear();
}

particle_system::emitters_iter particle_system::emitters_begin() const {
	return _Myemitters.begin();
}

particle_system::emitters_iter particle_system::emitters_end() const {
	return _Myemitters.end();
}

} //namespace lunar