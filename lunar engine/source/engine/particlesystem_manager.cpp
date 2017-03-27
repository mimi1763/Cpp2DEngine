/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: particlesystem_manager.cpp
---------------------------------------------------
*/

#include "particlesystem_manager.h"

//Dependencies
#include "file_manager.h"
#include "texture_manager.h"
#include "animation_manager.h"

#include "script_parser.h"
#include "string_utility.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
particlesystem_manager *particlesystem_manager::_Instance=0;

//Protected

void particlesystem_manager::copy(const particlesystem_manager &_Particlesystem_manager) {

_Script_container=_Particlesystem_manager._Script_container;
_Texture_manager=_Particlesystem_manager._Texture_manager;
_Animation_manager=_Particlesystem_manager._Animation_manager;
}

//Script specific

void particlesystem_manager::init_particlescript(script_parser *_Script_parser) {

script_property *_Property;

//Particle system class
script_class *_Particlesystem_class=_Script_parser->create_class("particle_system");

_Property=_Particlesystem_class->create_property("transformation");
_Property->add_parameter("world,local");

//Emitter class
script_class *_Emitter_class=_Script_parser->create_class("emitter");

_Property=_Emitter_class->create_property("type");
_Property->add_parameter("point,box,ring");

_Property=_Emitter_class->create_property("position");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Emitter_class->create_property("direction");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Emitter_class->create_property("quota");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Emitter_class->create_property("size");
_Property->add_parameter(script_property::DT_UINT32);
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Emitter_class->create_property("inner_size");
_Property->add_parameter(script_property::DT_UINT32);
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Emitter_class->create_property("rate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("angle");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("duration");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Emitter_class->create_property("particle_size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("lifetime");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("velocity");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("mass");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Emitter_class->create_property("color_range");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_COLOR);

_Property=_Emitter_class->create_property("particle_texture");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Emitter_class->create_property("particle_animation");
_Property->add_parameter(script_property::DT_STRING);

//Affector class
script_class *_Affector_class=_Script_parser->create_class("affector");

_Property=_Affector_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

//Color fader class
script_class *_Colorfader_class=_Script_parser->create_class("color_fader");

_Property=_Colorfader_class->create_property("fade_color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_FLOAT);

//Direction randomizer class
script_class *_Directionrandomizer_class=_Script_parser->create_class("direction_randomizer");

_Property=_Directionrandomizer_class->create_property("randomness");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Directionrandomizer_class->create_property("scope");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Directionrandomizer_class->create_property("keep_velocity");
_Property->add_parameter(script_property::DT_FLOAT);

//Gravitation class
script_class *_Gravitation_class=_Script_parser->create_class("gravitation");

_Property=_Gravitation_class->create_property("position");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Gravitation_class->create_property("gravity");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Gravitation_class->create_property("mass");
_Property->add_parameter(script_property::DT_FLOAT);

//Linear force class
script_class *_Linearforce_class=_Script_parser->create_class("linear_force");

_Property=_Linearforce_class->create_property("type");
_Property->add_parameter("add,average");

_Property=_Linearforce_class->create_property("force");
_Property->add_parameter(script_property::DT_VECTOR2);

//Scaler class
script_class *_Scaler_class=_Script_parser->create_class("scaler");

_Property=_Scaler_class->create_property("scale_point");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

//Sine force class
script_class *_Sineforce_class=_Script_parser->create_class("sine_force");

_Property=_Sineforce_class->create_property("type");
_Property->add_parameter("add,average");

_Property=_Sineforce_class->create_property("force");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Sineforce_class->create_property("frequency");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

//Scopes
script_scope *_Particlesystem_scope=_Script_parser->create_scope("particle_system");
_Particlesystem_scope->register_class(_Particlesystem_class);

script_scope *_Emitter_scope=_Script_parser->create_scope("emitter");
_Emitter_scope->register_class(_Emitter_class);

script_scope *_Affector_scope=_Script_parser->create_scope("affector");
_Affector_scope->register_class(_Colorfader_class);
_Affector_scope->register_class(_Directionrandomizer_class);
_Affector_scope->register_class(_Gravitation_class);
_Affector_scope->register_class(_Linearforce_class);
_Affector_scope->register_class(_Scaler_class);
_Affector_scope->register_class(_Sineforce_class);

_Script_parser->register_scope(_Particlesystem_scope);
_Particlesystem_class->register_scope(_Emitter_scope);
_Particlesystem_class->register_scope(_Affector_scope);
_Emitter_class->register_scope(_Affector_scope);

//Inheritance
_Colorfader_class->register_inheritance(_Affector_class);
_Directionrandomizer_class->register_inheritance(_Affector_class);
_Gravitation_class->register_inheritance(_Affector_class);
_Linearforce_class->register_inheritance(_Affector_class);
_Scaler_class->register_inheritance(_Affector_class);
_Sineforce_class->register_inheritance(_Affector_class);
}

bool particlesystem_manager::init_particlesystem(particle_system *_Particle_system,
	file_data *_Script_data) {

std::string _Data;

//Load script data from data file
_Script_container->load_bytes(_Script_data,_Data);

	//Load script data from file (filename)
	if (_Data.empty() && !file_manager::load_binary(
		_Script_data->filename(),_Data)) return false;
	
	//Init script parser first time
	if (!_Script_parser) {
	
	_Script_parser=new script_parser();
	init_particlescript(_Script_parser);
	}

script_parser::result _Result(script_parser::result::LINEAR);

	if (!_Script_parser->parse_data(_Data,_Script_data->name(),_Result))
		return false;

emitter *_Emitter=0; //Current emitter
affector *_Affector=0; //Current affector

	for (script_class::object::objects_iterator _Iter=_Result.objects_begin(),
		_End=_Result.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
		//Emitter
		if (_Object=="emitter")
			_Emitter=_Particle_system->create_emitter();
		//Affector
		else if (_Object!="particle_system") {
		
		affector_manager *_Affector_manager=_Particle_system; //Create global affector
		
			//Create local affector
			if (string_utility::compare(_Object.parent(),"emitter"))
				_Affector_manager=_Emitter;
			
			if (_Object=="color_fader")
				_Affector=_Affector_manager->create_colorfader();
			else if (_Object=="direction_randomizer")
				_Affector=_Affector_manager->create_directionrandomizer();
			else if (_Object=="gravitation")
				_Affector=_Affector_manager->create_gravitation();
			else if (_Object=="linear_force")
				_Affector=_Affector_manager->create_linearforce();
			else if (_Object=="scaler")
				_Affector=_Affector_manager->create_scaler();
			else if (_Object=="sine_force")
				_Affector=_Affector_manager->create_sineforce();
		}
		
		//Set properties
		for (script_class::object::data_iterator _Diter=_Object.data_begin(),
			_Dend=_Object.data_end();_Diter!=_Dend;++_Diter) {
		
		const script_property::data &_Data=(*_Diter); //Increase speed
		
			//Particle system properties
			if (_Object=="particle_system") {
			
				if (_Data=="transformation") {
				
					//Incase sensitive type compare
					if (string_utility::compare(_Data.value(0),"world"))
						_Particle_system->transformation(particle_system::WORLD);
					else if (string_utility::compare(_Data.value(0),"local"))
						_Particle_system->transformation(particle_system::LOCAL);
				}
			}
			//Emitter properties
			else if (_Object=="emitter") {
			
				if (_Data=="type") {
				
					//Incase sensitive type compare
					if (string_utility::compare(_Data.value(0),"point"))
						_Emitter->type(emitter::POINT);
					else if (string_utility::compare(_Data.value(0),"box"))
						_Emitter->type(emitter::BOX);
					else if (string_utility::compare(_Data.value(0),"ring"))
						_Emitter->type(emitter::RING);
				}
				else if (_Data=="position")
					_Emitter->position(_Data.value<vector2>(0));
				else if (_Data=="direction")
					_Emitter->direction(_Data.value<vector2>(0));
				else if (_Data=="quota")
					_Emitter->quota(_Data.value<unsigned int>(0));
				else if (_Data=="size")
					_Emitter->size(_Data.value<unsigned int>(0),
						_Data.value<unsigned int>(1));
				else if (_Data=="inner_size")
					_Emitter->inner_size(_Data.value<unsigned int>(0),
						_Data.value<unsigned int>(1));
				else if (_Data=="rate")
					_Emitter->rate(_Data.value<float>(0));
				else if (_Data=="angle")
					_Emitter->angle(_Data.value<float>(0));
				else if (_Data=="duration")
					_Emitter->duration(_Data.value<float>(0));
				else if (_Data=="enabled") {
				
					if (_Data.value<bool>(0)) _Emitter->start();
					else _Emitter->stop();
				}
				else if (_Data=="particle_size")
					_Emitter->particle_size(_Data.value<float>(0),
						_Data.value<float>(1));
				else if (_Data=="lifetime")
					_Emitter->lifetime(_Data.value<float>(0),
						_Data.value<float>(1));
				else if (_Data=="velocity")
					_Emitter->velocity(_Data.value<float>(0),
						_Data.value<float>(1));
				else if (_Data=="mass")
					_Emitter->mass(_Data.value<float>(0),
						_Data.value<float>(1));
				else if (_Data=="color_range")
					_Emitter->color_range(_Data.value<color>(0),
						_Data.value<color>(1));
				else if (_Data=="particle_texture" || _Data=="particle_animation") {
				
				manager *_Manager=_Texture_manager; //Texture mode
				
					//Animation mode
					if (_Data!="particle_texture") _Manager=_Animation_manager;
				
				texturable *_Texturable=0;
				manageable *_Manageable;
				
					//Reuse texture/animation if at least one instance already exist
					for (manager::manageables_iter _Miter=_Manager->manageables_begin(),
						_Mend=_Manager->manageables_end();_Miter!=_Mend;++_Miter) {
					
					_Manageable=(*_Miter); //Increase speed
					
						if (_Manageable->data() && _Manageable->
							data()->name()==_Data.value(0)) {
						
						_Texturable=dynamic_cast<texturable*>(_Manageable);
						break;
						}
					}
					
					if (_Manager==_Texture_manager) {
					
						//Create texture
						if (!_Texturable) _Texturable=_Texture_manager->
							create_texture(_Data.value(0));
					
					_Emitter->particle_texture(static_cast<texture*>(_Texturable));
					}
					else {
					
						//Create animation
						if (!_Texturable) _Texturable=_Animation_manager->
							create_animation(_Data.value(0));
					
					_Emitter->particle_animation(static_cast<animation*>(_Texturable));
					}
				}
			}
			//Affector properties
			else {
			
				//Shared property
				if (_Data=="enabled") {
				
					if (_Data.value<bool>(0)) _Affector->enable();
					else _Affector->disable();
				
				continue;
				}
				
				if (_Object=="color_fader") {
				
				color_fader *_Color_fader=static_cast<color_fader*>(_Affector);
				
					if (_Data=="fade_color")
						_Color_fader->add_fadecolor(_Data.value<color>(0),
							_Data.value<float>(1));
				}
				else if (_Object=="direction_randomizer") {
				
				direction_randomizer *_Direction_randomizer=static_cast<
					direction_randomizer*>(_Affector);
				
					if (_Data=="randomness")
						_Direction_randomizer->randomness(_Data.value<float>(0));
					else if (_Data=="scope")
						_Direction_randomizer->scope(_Data.value<float>(0));
					else if (_Data=="keep_velocity")
						_Direction_randomizer->keep_velocity(_Data.value<bool>(0));
				}
				else if (_Object=="gravitation") {
				
				gravitation *_Gravitation=static_cast<gravitation*>(_Affector);
				
					if (_Data=="position")
						_Gravitation->position(_Data.value<vector2>(0));
					else if (_Data=="gravity")
						_Gravitation->gravity(_Data.value<float>(0));
					else if (_Data=="mass")
						_Gravitation->mass(_Data.value<float>(0));
				}
				else if (_Object=="linear_force") {
				
				linear_force *_Linear_force=static_cast<linear_force*>(_Affector);
				
					if (_Data=="type") {
					
						//Incase sensitive type compare
						if (string_utility::compare(_Data.value(0),"add"))
							_Linear_force->type(linear_force::ADD);
						else if (string_utility::compare(_Data.value(0),"average"))
							_Linear_force->type(linear_force::AVERAGE);
					}
					else if (_Data=="force")
						_Linear_force->force(_Data.value<vector2>(0));
				}
				else if (_Object=="scaler") {
				
				scaler *_Scaler=static_cast<scaler*>(_Affector);
				
					if (_Data=="scale_point")
						_Scaler->add_scalepoint(_Data.value<float>(0),
							_Data.value<float>(1));
				}
				else if (_Object=="sine_force") {
				
				sine_force *_Sine_force=static_cast<sine_force*>(_Affector);
				
					if (_Data=="type") {
					
						//Incase sensitive type compare
						if (string_utility::compare(_Data.value(0),"add"))
							_Sine_force->type(sine_force::ADD);
						else if (string_utility::compare(_Data.value(0),"average"))
							_Sine_force->type(sine_force::AVERAGE);
					}
					else if (_Data=="force")
						_Sine_force->force(_Data.value<vector2>(0));
					else if (_Data=="frequency")
						_Sine_force->frequency(_Data.value<float>(0),
							_Data.value<float>(1));
				}
			}
		}
	}

return true;
}

//Public

particlesystem_manager::particlesystem_manager():_Script_container(
	file_container::instance()),_Texture_manager(texture_manager::instance()),
		_Animation_manager(animation_manager::instance()),_Script_parser(0) {
	//Empty
}

particlesystem_manager::particlesystem_manager(texture_manager *_Texture_manager,
	animation_manager *_Animation_manager):_Script_container(
		file_container::instance()),_Texture_manager(_Texture_manager),
			_Animation_manager(_Animation_manager),_Script_parser(0) {
	//Empty
}

particlesystem_manager::particlesystem_manager(
	const particlesystem_manager &_Particlesystem_manager):
		manager(_Particlesystem_manager) {

_Script_parser=0; //Not copyable
copy(_Particlesystem_manager);
}

particlesystem_manager::~particlesystem_manager() {

	if (_Script_parser) delete _Script_parser;
}

particlesystem_manager &particlesystem_manager::operator=(
	const particlesystem_manager &_Particlesystem_manager) {

	//Worth doing
	if (this!=&_Particlesystem_manager) {
	
	manager::operator=(_Particlesystem_manager); //Invoke base operator=
	copy(_Particlesystem_manager);
	}

return *this;
}

particlesystem_manager *particlesystem_manager::instance() {
	return _Instance;
}

void particlesystem_manager::update(manageable *_Manageable) {

	//No need for update or no script to load
	if (_Manageable->status()!=manageable::NEED_UPDATE ||
		_Manageable->data()==0) return;

particle_system *_Particle_system=dynamic_cast<particle_system*>(
	_Manageable);

	//Invalid type for this manager
	if (!_Particle_system) return;

file_data *_Script_data=static_cast<file_data*>(
	_Manageable->data());

	//No data
	if (!_Script_data) {
	
	status(_Particle_system,manageable::FAILED);
	return;
	}

status(_Particle_system,init_particlesystem(_Particle_system,_Script_data)?
	manageable::OKAY:manageable::FAILED); //Pending
}

void particlesystem_manager::update() {
	manager::update(); //Must redirect because of name reuse
}

void particlesystem_manager::update(float _Time) {

	//No need for update
	if (_Mymanageables.empty()) return;

update(); //Make sure all particle_system scripts are loaded

	//Update all particle_systems
	for (_Manageables::const_iterator _Iter=_Mymanageables.begin(),
		_End=_Mymanageables.end();_Iter!=_End;++_Iter)
			static_cast<particle_system*>(*_Iter)->update(_Time);
}

void particlesystem_manager::script_source(file_container *_Script_container) {
	this->_Script_container=_Script_container;
}

file_container *particlesystem_manager::script_source() const {
	return _Script_container;
}

//Particle system functions

particle_system *particlesystem_manager::create_particlesystem(
	bool _Visible,bool _Secured) {

particle_system *_Myparticle_system=new particle_system(this,_Visible);
add(_Myparticle_system,_Secured);
return _Myparticle_system;
}

particle_system *particlesystem_manager::create_particlesystem(
	const std::string &_Script_name,bool _Visible,
		bool _Secured) {
	return create_particlesystem(static_cast<file_data*>(
		_Script_container->data(_Script_name)),_Visible,_Secured);
}

particle_system *particlesystem_manager::create_particlesystem(
	const file_data &_Script_data,bool _Visible,
		bool _Secured) {
	return create_particlesystem(_Script_container->add_data(
		_Script_data),_Visible,_Secured);
}

particle_system *particlesystem_manager::create_particlesystem(
	file_data *_Script_data,bool _Visible,
		bool _Secured) {

particle_system *_Myparticle_system=new particle_system(this,
	_Script_data,_Visible);
add(_Myparticle_system,_Secured);
return _Myparticle_system;
}

particle_system *particlesystem_manager::create_particlesystem(
	const particle_system &_Particle_system,bool _Secured) {

particle_system *_Myparticle_system=new particle_system(_Particle_system);
add(_Myparticle_system,_Secured);
return _Myparticle_system;
}

} //namespace lunar