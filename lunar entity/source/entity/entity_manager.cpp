/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: entity_manager.cpp
---------------------------------------------------
*/

#include "entity_manager.h"

//Dependencies (engine)
#include "timer_manager.h"
#include "sound_manager.h"

#include "text_manager.h"
#include "texture_manager.h"
#include "animation_manager.h"
#include "particlesystem_manager.h"

#include "shader_manager.h"
#include "shaderprogram_manager.h"
#include "framebuffer_manager.h"
#include "scene_manager.h"

#include "render_node.h"
#include "math_utility.h"
#include "string_utility.h"

//Dependencies (entity)
#include "node_manager.h"
#include "lighting_manager.h"

//Public

//Environment
//Private

void entity_manager::environment::copy(const environment &_Environment) {

_Timer_manager=_Environment._Timer_manager;
_Sound_manager=_Environment._Sound_manager;

_Font_manager=_Environment._Font_manager;
_Text_manager=_Environment._Text_manager;
_Texture_manager=_Environment._Texture_manager;
_Animation_manager=_Environment._Animation_manager;
_Particlesystem_manager=_Environment._Particlesystem_manager;

_Shader_manager=_Environment._Shader_manager;
_Shaderprogram_manager=_Environment._Shaderprogram_manager;
_Framebuffer_manager=_Environment._Framebuffer_manager;
_Scene_manager=_Environment._Scene_manager;
}

//Public

entity_manager::environment::environment() {
	managers(); //Use the manager instances as default managers
}

entity_manager::environment::environment(const environment &_Environment) {
	copy(_Environment);
}

entity_manager::environment::~environment() {
	//Empty
}

entity_manager::environment &entity_manager::environment::operator=(
	const environment &_Environment) {

	//Worth doing
	if (this!=&_Environment) copy(_Environment);

return *this;
}

void entity_manager::environment::managers(timer_manager *_Timer_manager,
										   sound_manager *_Sound_manager,
										   font_manager *_Font_manager,
										   text_manager *_Text_manager,
										   texture_manager *_Texture_manager,
										   animation_manager *_Animation_manager,
										   particlesystem_manager *_Particlesystem_manager,
										   shader_manager *_Shader_manager,
										   shaderprogram_manager *_Shaderprogram_manager,
										   framebuffer_manager *_Framebuffer_manager,
										   scene_manager *_Scene_manager) {

	if (_Timer_manager) this->_Timer_manager=_Timer_manager;
	else this->_Timer_manager=timer_manager::instance();
	
	if (_Sound_manager) this->_Sound_manager=_Sound_manager;
	else this->_Sound_manager=sound_manager::instance();
	
	if (_Font_manager) this->_Font_manager=_Font_manager;
	else this->_Font_manager=font_manager::instance();
	
	if (_Text_manager) this->_Text_manager=_Text_manager;
	else this->_Text_manager=text_manager::instance();
	
	if (_Texture_manager) this->_Texture_manager=_Texture_manager;
	else this->_Texture_manager=texture_manager::instance();
	
	if (_Animation_manager) this->_Animation_manager=_Animation_manager;
	else this->_Animation_manager=animation_manager::instance();
	
	if (_Particlesystem_manager) this->_Particlesystem_manager=_Particlesystem_manager;
	else this->_Particlesystem_manager=particlesystem_manager::instance();
	
	if (_Shader_manager) this->_Shader_manager=_Shader_manager;
	else this->_Shader_manager=shader_manager::instance();
	
	if (_Shaderprogram_manager) this->_Shaderprogram_manager=_Shaderprogram_manager;
	else this->_Shaderprogram_manager=shaderprogram_manager::instance();
	
	if (_Framebuffer_manager) this->_Framebuffer_manager=_Framebuffer_manager;
	else this->_Framebuffer_manager=framebuffer_manager::instance();
	
	if (_Scene_manager) this->_Scene_manager=_Scene_manager;
	else this->_Scene_manager=scene_manager::instance();
}

timer_manager *entity_manager::environment::timer_manager_ptr() const {
	return _Timer_manager;
}

sound_manager *entity_manager::environment::sound_manager_ptr() const {
	return _Sound_manager;
}

font_manager *entity_manager::environment::font_manager_ptr() const {
	return _Font_manager;
}

text_manager *entity_manager::environment::text_manager_ptr() const {
	return _Text_manager;
}

texture_manager *entity_manager::environment::texture_manager_ptr() const {
	return _Texture_manager;
}

animation_manager *entity_manager::environment::animation_manager_ptr() const {
	return _Animation_manager;
}

particlesystem_manager *entity_manager::environment::particlesystem_manager_ptr() const {
	return _Particlesystem_manager;
}

shader_manager *entity_manager::environment::shader_manager_ptr() const {
	return _Shader_manager;
}

shaderprogram_manager *entity_manager::environment::shaderprogram_manager_ptr() const {
	return _Shaderprogram_manager;
}

framebuffer_manager *entity_manager::environment::framebuffer_manager_ptr() const {
	return _Framebuffer_manager;
}

scene_manager *entity_manager::environment::scene_manager_ptr() const {
	return _Scene_manager;
}

//Entity manager
//Protected

void entity_manager::copy(const entity_manager &_Entity_manager) {

*_Node_manager=*_Entity_manager._Node_manager;
*_Lighting_manager=*_Entity_manager._Lighting_manager;
_Environment=_Entity_manager._Environment;
}

//Script specific

int entity_manager::read_draworder(const script_class::object &_Object) {

	//Search for draw order property
	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter) {
	
	const script_property::data &_Data=(*_Iter); //Increase speed
	
		if (_Data=="draw_order")
			return _Data.value<int>(0);
	}

return 1;
}

object *entity_manager::read_parent(entity *_Entity,const script_class::object &_Object) {
	
	//Search for parent property
	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter) {
				
	const script_property::data &_Data=(*_Iter); //Increase speed
				
		if (_Data=="parent")
			return _Entity->object_pointer(_Data.value(0));
	}

return 0;
}

bool entity_manager::read_paused(const script_class::object &_Object) {

	//Search for paused property
	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter) {
				
	const script_property::data &_Data=(*_Iter); //Increase speed
				
		if (_Data=="paused")		
			return _Data.value<bool>(0);
	}

return false;
}

attachable *entity_manager::read_attachable(component *_Component,const script_class::object &_Object) {

attachable *_Attachable = 0;

	if (_Component && !_Object.name().empty()) {
	
		//Text
		if (_Object=="text")
			_Attachable=_Component->data(font_container::
				instance()->data(_Object.name()));
		//Texture
		else if (_Object=="texture")
			_Attachable=_Component->data(texture_container::
				instance()->data(_Object.name()));
		//Animation
		else if (_Object=="animation")
			_Attachable=_Component->data(animation_container::
				instance()->data(_Object.name()));
		//Particle system
		else if (_Object=="particle_system")
			_Attachable=_Component->data(file_container::
				instance()->data(_Object.name()));
	}

	//Skip
	if (!_Attachable) return 0;

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			attachable_properties(_Attachable,_Environment,_Object,*_Iter);

return _Attachable;
}

component *entity_manager::read_component(object *_Entity_object,const script_class::object &_Object) {

component *_Component = 0;

	if (_Entity_object && !_Object.name().empty()) {
	
		if (_Object=="light_component") {
				
		light *_Light=static_cast<light*>(_Entity_object);
		_Component=_Light->create_lightcomponent(_Object.name());
		}
		else _Component=_Entity_object->create_component(_Object.name());
	}

	//Skip
	if (!_Component) return 0;

	for (script_class::object::objects_iterator _Iter=_Object.objects_begin(),
		_End=_Object.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed

		if (_Object=="text" || _Object=="texture" ||
			_Object=="animation" || _Object=="particle_system")
				read_attachable(_Component,_Object);
	}

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			component_properties(_Component,*_Iter);

return _Component;
}

render_node *entity_manager::read_node(entity *_Entity, object *_Entity_object,const script_class::object &_Object) {

render_node *_Node=(string_utility::compare(_Object.parent(),"object") ||
	string_utility::compare(_Object.parent(),"light")?
		_Entity_object->node():_Entity->node());

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			node_properties(_Node,*_Iter);

return _Node;
}

sound *entity_manager::read_sound(entity::sound_group *_Sound_group,const script_class::object &_Object) {

sound *_Sound = 0;

	if (_Sound_group && !_Object.name().empty()) {
	
	bool _Paused=read_paused(_Object);
	
	_Sound=_Sound_group->create_sound(sound_container::
		instance()->data(_Object.name()),_Paused);
	}

	//Skip
	if (!_Sound) return 0;

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			sound_properties(_Sound,*_Iter);

return _Sound;
}

audible::sound_group *entity_manager::read_soundgroup(entity *_Entity,object *_Entity_object,
	const script_class::object &_Object) {

audible::sound_group *_Sound_group = 0;

	if (!_Object.name().empty()) {
	
		if (_Entity && !string_utility::compare(_Object.parent(),"object") &&
			!string_utility::compare(_Object.parent(),"light"))
				_Sound_group=_Entity->create_soundgroup(_Object.name());
		else if (_Entity_object)
			_Sound_group=_Entity_object->create_soundgroup(_Object.name());
	}

	//Skip
	if (!_Sound_group) return 0;

	for (script_class::object::objects_iterator _Iter=_Object.objects_begin(),
		_End=_Object.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed

		if (_Object=="sound")
			read_sound(_Sound_group,_Object);
	}

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			soundgroup_properties(_Sound_group,*_Iter);

return _Sound_group;
}

object *entity_manager::read_entityobject(entity *_Entity,const script_class::object &_Object) {

object *_Entity_object = 0;

	if (_Entity && !_Object.name().empty()) {
			
	object *_Parent_object=read_parent(_Entity, _Object);
				
		if (_Object=="light") {
				
			if (_Parent_object)
				_Entity_object=_Entity->create_light(_Object.name(),
					_Parent_object,read_draworder(_Object));
			else _Entity_object=_Entity->create_light(_Object.name(),
				read_draworder(_Object));
		}
		else {
				
			if (_Parent_object)
				_Entity_object=_Entity->create_object(_Object.name(),
					_Parent_object,read_draworder(_Object));
			else _Entity_object=_Entity->create_object(_Object.name(),
				read_draworder(_Object));
		}
	}

	//Skip
	if (!_Entity_object) return 0;

	for (script_class::object::objects_iterator _Iter=_Object.objects_begin(),
		_End=_Object.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed

		if (_Object=="component" || _Object=="light_component")
			read_component(_Entity_object,_Object);
		else if (_Object=="node")
			read_node(_Entity,_Entity_object,_Object);
		else if (_Object=="sound_group")
			read_soundgroup(_Entity,_Entity_object,_Object);
	}

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			object_properties(_Entity_object,_Object,*_Iter);

return _Entity_object;
}

entity *entity_manager::read_entity(entity *_Entity, const script_class::object &_Object) {

	//Skip
	if (!_Entity) return 0;

	for (script_class::object::objects_iterator _Iter=_Object.objects_begin(),
		_End=_Object.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed

		if (_Object == "object" || _Object == "light")
			read_entityobject(_Entity,_Object);
		else if (_Object == "sound_group")
			read_soundgroup(_Entity,0,_Object);
		else if (_Object == "action_group")
			read_actiongroup(_Entity,_Object);
		else if (_Object == "node")
			read_node(_Entity,0,_Object);
	}

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			entity_properties(_Entity,*_Iter);

return _Entity;
}

entity::action *entity_manager::read_action(entity::action_group *_Action_group,const script_class::object &_Object) {

entity::action *_Action = 0;

	if (_Action_group) {
	
		if (_Object=="motion")
			_Action=_Action_group->create_motion();
		else _Action=_Action_group->create_action();
	}

	//Skip
	if (!_Action) return 0;

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			action_properties(_Action,_Object,*_Iter);

return _Action;
}

entity::action_group *entity_manager::read_actiongroup(entity *_Entity,const script_class::object &_Object) {

entity::action_group *_Action_group = 0;
		
	if (_Entity && !_Object.name().empty())
		_Action_group=_Entity->create_actiongroup(_Object.name());

	//Skip
	if (!_Action_group) return 0;

	for (script_class::object::objects_iterator _Iter=_Object.objects_begin(),
		_End=_Object.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed

		if (_Object == "action" || _Object == "motion")
			read_action(_Action_group,_Object);
	}

	for (script_class::object::data_iterator _Iter=_Object.data_begin(),
		_End=_Object.data_end();_Iter!=_End;++_Iter)
			actiongroup_properties(_Action_group,*_Iter);

return _Action_group;
}

void entity_manager::attachable_properties(attachable *_Attachable,const environment &_Environment,
	const script_class::object &_Object,const script_property::data &_Data) {
	
	if (!_Attachable) return;

	//Shared properties
	if (_Data=="blending") {
				
		if (string_utility::compare(_Data.value(0),"default_blend"))
			_Attachable->blending(render_manager::DEFAULT_BLEND);
		else if (string_utility::compare(_Data.value(0),"default_alpha_blend"))
			_Attachable->blending(render_manager::DEFAULT_ALPHA_BLEND);
		else if (string_utility::compare(_Data.value(0),"additive_blend"))
			_Attachable->blending(render_manager::ADDITIVE_BLEND);
		else if (string_utility::compare(_Data.value(0),"additive_alpha_blend"))
			_Attachable->blending(render_manager::ADDITIVE_ALPHA_BLEND);
		else if (string_utility::compare(_Data.value(0),"light_blend"))
			_Attachable->blending(render_manager::LIGHT_BLEND);
	}
	else if (_Data=="passes")
		_Attachable->passes(_Data.value<unsigned int>(0));
	else if (_Data=="visible")
		_Attachable->visible(_Data.value<bool>(0));
	else if (_Data=="bound_offset")
		_Attachable->bound_offset(_Data.value<vector2>(0),_Data.value<vector2>(1));
	else if (_Data=="show_bounds")
		_Attachable->show_bounds(_Data.value<bool>(0));
	else if (_Data=="shaderprogram") {
				
	unsigned int _Size=_Data.values();
				
		for (unsigned int i=0;i<_Size;++i) {
					
		file_data *_Script_data=dynamic_cast<file_data*>(
			file_container::instance()->data(_Data.value(0)));
					
			if (_Script_data)
				_Attachable->add_shaderprogram(_Environment.
					shaderprogram_manager_ptr()->create_shaderprogram(_Script_data));
		}
	}
	else if (_Data=="shaderprograms_enabled")
		_Attachable->shaderprograms_enabled(_Data.value<bool>(0));
				
	//Text properties
	else if (_Object=="text") {
				
	text *_Text=static_cast<text*>(_Attachable);
				
		if (_Data=="data")
			_Text->text_data(_Data.value(0));
		else if (_Data=="alignment") {
					
			if (string_utility::compare(_Data.value(0),"left"))
				_Text->alignment(text::LEFT);
			else if (string_utility::compare(_Data.value(0),"center"))
				_Text->alignment(text::CENTER);
			else if (string_utility::compare(_Data.value(0),"right"))
				_Text->alignment(text::RIGHT);
		}
		else if (_Data=="color")
			_Text->color_value(_Data.value<color>(0));
		else if (_Data=="wrap_width")
			_Text->wrap_width(_Data.value<unsigned int>(0));
		else if (_Data=="styles_enabled")
			_Text->styles_enabled(_Data.value<bool>(0));
		else if (_Data=="line_space")
			_Text->line_space(_Data.value<int>(0));
		else if (_Data=="max_lines")
			_Text->max_lines(_Data.value<unsigned int>(0));
		else if (_Data=="skip_lines")
			_Text->skip_lines(_Data.value<unsigned int>(0));
	}
	//Particle system properties
	else if (_Object=="particle_system") {
		//Empty
	}
	else {
				
	texturable *_Texturable=static_cast<texturable*>(_Attachable);
				
		//Shared properties
		//Renderable
		if (_Data=="color")
			_Texturable->color_value(_Data.value<color>(0));
		else if (_Data=="point_rendering")
			_Texturable->point_rendering(_Data.value<bool>(0));
		else if (_Data=="point_size")
			_Texturable->point_size(_Data.value<float>(0));
		//Rectangle
		else if (_Data=="size")
			_Texturable->size(_Data.value<float>(0),_Data.value<float>(1));
		else if (_Data=="origin") {
					
			if (string_utility::compare(_Data.value(0),"top_left"))
				_Texturable->origin(rectangle::TOP_LEFT);
			else if (string_utility::compare(_Data.value(0),"top_center"))
				_Texturable->origin(rectangle::TOP_CENTER);
			else if (string_utility::compare(_Data.value(0),"top_right"))
				_Texturable->origin(rectangle::TOP_RIGHT);
			else if (string_utility::compare(_Data.value(0),"center_left"))
				_Texturable->origin(rectangle::CENTER_LEFT);
			else if (string_utility::compare(_Data.value(0),"center"))
				_Texturable->origin(rectangle::CENTER);
			else if (string_utility::compare(_Data.value(0),"center_right"))
				_Texturable->origin(rectangle::CENTER_RIGHT);
			else if (string_utility::compare(_Data.value(0),"bottom_left"))
				_Texturable->origin(rectangle::BOTTOM_LEFT);
			else if (string_utility::compare(_Data.value(0),"bottom_center"))
				_Texturable->origin(rectangle::BOTTOM_CENTER);
			else if (string_utility::compare(_Data.value(0),"bottom_right"))
				_Texturable->origin(rectangle::BOTTOM_RIGHT);
		}
		//Texturable
		else if (_Data=="auto_size")
			_Texturable->auto_size(_Data.value<bool>(0));
		else if (_Data=="stretch")
			_Texturable->stretch(_Data.value<bool>(0),_Data.value<bool>(1));
		else if (_Data=="flip")
			_Texturable->flip(_Data.value<bool>(0),_Data.value<bool>(1));
		else if (_Data=="rotation") {
					
			if (string_utility::compare(_Data.value(0),"none"))
				_Texturable->rotation(texturable::ROTATE_NONE);
			else if (string_utility::compare(_Data.value(0),"180"))
				_Texturable->rotation(texturable::ROTATE_180);
			else if (string_utility::compare(_Data.value(0),"90cw"))
				_Texturable->rotation(texturable::ROTATE_90CW);
			else if (string_utility::compare(_Data.value(0),"90ccw"))
				_Texturable->rotation(texturable::ROTATE_90CCW);
		}
		else if (_Data=="ratio_mode") {
		
			if (string_utility::compare(_Data.value(0),"keep_width"))
				_Texturable->ratio_mode(texturable::RATIO_KEEP_WIDTH);
			else if (string_utility::compare(_Data.value(0),"keep_height"))
				_Texturable->ratio_mode(texturable::RATIO_KEEP_HEIGHT);
			else if (string_utility::compare(_Data.value(0),"fit_inside"))
				_Texturable->ratio_mode(texturable::RATIO_FIT_INSIDE);
			else if (string_utility::compare(_Data.value(0),"fully_cover"))
				_Texturable->ratio_mode(texturable::RATIO_FULLY_COVER);
		}
		else if (_Data=="keep_ratio")
			_Texturable->keep_ratio(_Data.value<bool>(0));
		else if (_Data=="cut_mode") {
					
			if (string_utility::compare(_Data.value(0),"top_left"))
				_Texturable->cut_mode(texturable::CUT_TOP_LEFT);
			else if (string_utility::compare(_Data.value(0),"top_right"))
				_Texturable->cut_mode(texturable::CUT_TOP_RIGHT);
			else if (string_utility::compare(_Data.value(0),"bottom_left"))
				_Texturable->cut_mode(texturable::CUT_BOTTOM_LEFT);
			else if (string_utility::compare(_Data.value(0),"bottom_right"))
				_Texturable->cut_mode(texturable::CUT_BOTTOM_RIGHT);
		}
		else if (_Data=="crop_size")
			_Texturable->crop_size(_Data.value<float>(0),_Data.value<float>(1));
		else if (_Data=="crop_position")
			_Texturable->crop_position(_Data.value<vector2>(0));
		else if (_Data=="auto_crop")
			_Texturable->auto_crop(_Data.value<bool>(0));
					
		//Texture properties
		else if (_Object=="texture") {
			//Empty
		}
		//Animation properties
		else if (_Object=="animation") {
					
		animation *_Animation=static_cast<animation*>(_Texturable);
		
			if (_Data=="enabled") {	

				if (_Data.value<bool>(0))
					_Animation->start();
				else
					_Animation->stop();
			}
			else if (_Data=="framerate") {
						
			_Animation->update(); //Required
			_Animation->framerate(_Data.value<float>(0));
			}
			else if (_Data=="duration") {
						
			_Animation->update(); //Required
			_Animation->duration(_Data.value<float>(0));
			}
			else if (_Data=="loop")
				_Animation->loop(_Data.value<bool>(0));
			else if (_Data=="reverse")
				_Animation->reverse(_Data.value<bool>(0));
			else if (_Data=="frame") {
						
			_Animation->update(); //Required
			_Animation->frame(_Data.value<unsigned int>(0));
			}
		}
	}
}

void entity_manager::component_properties(component *_Component,const script_property::data &_Data) {

	if (!_Component) return;

	if (_Data=="text")
		_Component->data(font_container::instance()->data(_Data.value(0)));
	else if (_Data=="texture")
		_Component->data(texture_container::instance()->data(_Data.value(0)));
	else if (_Data=="animation")
		_Component->data(animation_container::instance()->data(_Data.value(0)));
	else if (_Data=="particle_system")
		_Component->data(file_container::instance()->data(_Data.value(0)));
				
	else if (_Data=="color")
		_Component->color_channel(_Data.value<color>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="opacity")
		_Component->opacity(_Data.value<float>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="visible")
		_Component->visible(_Data.value<bool>(0));
}

void entity_manager::node_properties(render_node *_Node,const script_property::data &_Data) {

	if (!_Node) return;
			
	if (_Data=="draw_order")
		_Node->draw_order(_Data.value<int>(0),_Data.values()>1?
			_Data.value<bool>(1):true);
	else if (_Data=="visible")
		_Node->visible(_Data.value<bool>(0),_Data.values()>1?
			_Data.value<bool>(1):true);
	else if (_Data=="lookat")
		_Node->lookat(_Data.value<vector2>(0));
	else if (_Data=="position")
		_Node->position(_Data.value<vector2>(0));
	else if (_Data=="direction")
		_Node->direction(_Data.value<vector2>(0));
	else if (_Data=="rotation")
		_Node->rotation(math::degree(_Data.value<float>(0)));
	else if (_Data=="scaling")
		_Node->scaling(_Data.value<vector2>(0));
	else if (_Data=="rotation_origin") {
				
		if (string_utility::compare(_Data.value(0),"parent"))
			_Node->rotation_origin(render_node::PARENT);
		else if (string_utility::compare(_Data.value(0),"local"))
			_Node->rotation_origin(render_node::LOCAL);
	}
	else if (_Data=="inherit_rotation")
		_Node->inherit_rotation(_Data.value<bool>(0));
	else if (_Data=="inherit_scaling")
		_Node->inherit_scaling(_Data.value<bool>(0));
}

void entity_manager::sound_properties(sound *_Sound,const script_property::data &_Data) {

	if (!_Sound) return;

	if (_Data=="volume")
		_Sound->volume(_Data.value<float>(0));
	else if (_Data=="mute")
		_Sound->mute(_Data.value<bool>(0));
	else if (_Data=="attributes")
		_Sound->attributes(_Data.value<vector2>(0),_Data.values()>1?
			_Data.value<vector2>(1):vector2::ZERO);
	else if (_Data=="distance")
		_Sound->distance(_Data.value<float>(0),_Data.values()>1?
			_Data.value<float>(1):1000000000.0f);
}

void entity_manager::soundgroup_properties(audible::sound_group *_Sound_group,const script_property::data &_Data) {

	if (!_Sound_group) return;

	if (_Data=="volume")
		_Sound_group->volume(_Data.value<float>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="sound")
		_Sound_group->create_sound(sound_container::
			instance()->data(_Data.value(0)),_Data.values()>1?
				_Data.value<bool>(1):true);
}

void entity_manager::object_properties(object *_Entity_object,const script_class::object &_Object,
	const script_property::data &_Data) {

	if (!_Entity_object) return;

	//Shared properties
	if (_Data=="node") {
				
	_Entity_object->node()->position(_Data.value<vector2>(0));
				
		if (_Data.values()>1) _Entity_object->node()->rotation(
			math::degree(_Data.value<float>(1)));
	}
	else if (_Data=="elevation")
		_Entity_object->elevation(_Data.value<float>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="color")
		_Entity_object->color_channel(_Data.value<color>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="opacity")
		_Entity_object->opacity(_Data.value<float>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="visible")
		_Entity_object->visible(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
	else if (_Data=="cast_shadow")
		_Entity_object->cast_shadow(_Data.value<bool>(0));
	else if (_Data=="shadow_type") {
				
		if (string_utility::compare(_Data.value(0),"global"))
			_Entity_object->shadow_type(object::GLOBAL);
		else if (string_utility::compare(_Data.value(0),"local"))
			_Entity_object->shadow_type(object::LOCAL);
	}
	else if (_Data=="component" || _Data=="light_component") {
				
	resource_data *_Resource_data=0;
	bool _Visible=(_Data.values()>3?_Data.value<bool>(3):true);
				
		if (_Data.values()>2) {
					
			if (string_utility::compare(_Data.value(1),"text"))
				_Resource_data=font_container::instance()->data(_Data.value(2));
			else if (string_utility::compare(_Data.value(1),"texture"))
				_Resource_data=texture_container::instance()->data(_Data.value(2));
			else if (string_utility::compare(_Data.value(1),"animation"))
				_Resource_data=animation_container::instance()->data(_Data.value(2));
			else if (string_utility::compare(_Data.value(1),"particle_system"))
				_Resource_data=file_container::instance()->data(_Data.value(2));
		}
					
		if (_Data=="light_component") {
					
		light *_Light=static_cast<light*>(_Entity_object);
		_Light->create_lightcomponent(_Data.value(0),_Resource_data,_Visible);
		}
		else _Entity_object->create_component(_Data.value(0),_Resource_data,_Visible);
	}
	else if (_Data=="volume")
		_Entity_object->volume(_Data.value<float>(0),
			_Data.values()>1?_Data.value<bool>(1):true);
				
	//Light properties
	else if (_Object=="light") {
				
	light *_Light=static_cast<light*>(_Entity_object);
				
		if (_Data=="enabled")
			_Light->enabled(_Data.value<bool>(0));
		else if (_Data=="brightness")
			_Light->brightness(_Data.value<float>(0),
				_Data.values()>1?_Data.value<bool>(1):true);
	}
}

void entity_manager::entity_properties(entity *_Entity,
	const script_property::data &_Data) {

	if (!_Entity) return;
	
	if (_Data=="node") {
	
	_Entity->node()->position(_Data.value<vector2>(0));
	
		if (_Data.values()>1) _Entity->node()->rotation(
			math::degree(_Data.value<float>(1)));
	}
	else if (_Data=="volume")
		_Entity->volume(_Data.value<float>(0),_Data.values()>1?
			_Data.value<bool>(1):true);
	
	//Common functionality
	else {
	
	object *_Parent=(_Data.values()>1?_Entity->object_pointer(_Data.value(1)):0);
	
		if (_Data=="object" || _Data=="light") {
		
		int _Draw_order=(_Data.values()>2?_Data.value<int>(2):1);
		bool _Visible=(_Data.values()>3?_Data.value<bool>(3):true);
		
			if (_Data=="light")
				_Entity->create_light(_Data.value(0),_Parent,_Draw_order,_Visible);
			else _Entity->create_object(_Data.value(0),_Parent,_Draw_order,_Visible);
		}
		else {
		
		bool _Forced=(_Data.values()>2?_Data.value<bool>(2):false);
		
			if (_Data=="elevation") {
			
				if (!_Parent) _Entity->elevation(_Data.value<float>(0));
				else _Entity->elevation(_Parent,_Data.value<float>(0),_Forced);
			}
			else if (_Data=="color") {
			
				if (!_Parent) _Entity->color_channel(_Data.value<color>(0));
				else _Entity->color_channel(_Parent,_Data.value<color>(0),_Forced);
			}
			else if (_Data=="opacity") {
			
				if (!_Parent) _Entity->opacity(_Data.value<float>(0));
				else _Entity->opacity(_Parent,_Data.value<float>(0),_Forced);
			}
			else if (_Data=="visible") {
			
				if (!_Parent) _Entity->visible(_Data.value<bool>(0));
				else _Entity->visible(_Parent,_Data.value<bool>(0),_Forced);
			}
			else if (_Data=="sound_enabled") {
			
				if (!_Parent) _Entity->sound_enabled(_Data.value<bool>(0));
				else _Entity->sound_enabled(_Parent,_Data.value<bool>(0),_Forced);
			}
			else if (_Data=="sound_volume") {
			
				if (!_Parent) _Entity->sound_volume(_Data.value<float>(0));
				else _Entity->sound_volume(_Parent,_Data.value<float>(0),_Forced);
			}
			else if (_Data=="light_enabled") {
			
				if (!_Parent) _Entity->light_enabled(_Data.value<bool>(0));
				else _Entity->light_enabled(_Parent,_Data.value<bool>(0));
			}
			else if (_Data=="light_brightness") {
			
				if (!_Parent) _Entity->light_brightness(_Data.value<float>(0));
				else _Entity->light_brightness(_Parent,_Data.value<float>(0),_Forced);
			}
		}
	}
}

void entity_manager::action_properties(entity::action *_Action,const script_class::object &_Object,
	const script_property::data &_Data) {

	if (!_Action) return;

entity::motion *_Motion=dynamic_cast<entity::motion*>(_Action);
			
	//Shared properties
	if (_Data=="scope") {
				
		if (string_utility::compare(_Data.value(0),"tree"))
			_Action->scope(entity::action::TREE,
				_Data.values()>1?_Data.value(1):"");
		else if (string_utility::compare(_Data.value(0),"branch"))
			_Action->scope(entity::action::BRANCH,
				_Data.values()>1?_Data.value(1):"");
		else if (string_utility::compare(_Data.value(0),"local"))
			_Action->scope(entity::action::LOCAL,
				_Data.values()>1?_Data.value(1):"");
	}
	else if (_Data=="start")
		_Action->start(_Data.value<float>(0));
	else if (_Data=="enabled")
		_Action->enabled(_Data.value<bool>(0));
	else if (_Data=="translate")
		_Action->translate(_Data.value<vector2>(0));
	else if (_Data=="rotate")
		_Action->rotate(math::degree(_Data.value<float>(0)));
	else if (_Data=="scale")
		_Action->scale(_Data.value<vector2>(0));
				
	//Dual properties
	else if (_Data=="position") {
				
		if (_Motion) _Motion->position(_Data.value<vector2>(0),
			_Data.values()>1?_Data.value<vector2>(1):_Data.value<vector2>(0));
		else _Action->position(_Data.value<vector2>(0));
	}
	else if (_Data=="rotation") {
				
		if (_Motion) _Motion->rotation(math::degree(_Data.value<float>(0)),
			_Data.values()>1?math::degree(_Data.value<float>(1)):
				math::degree(_Data.value<float>(0)));
		else _Action->rotation(math::degree(_Data.value<float>(0)));
	}
	else if (_Data=="scaling") {
				
		if (_Motion) _Motion->scaling(_Data.value<vector2>(0),
			_Data.values()>1?_Data.value<vector2>(1):_Data.value<vector2>(0));
		else _Action->scaling(_Data.value<vector2>(0));
	}
	else if (_Data=="color") {
				
		if (_Motion) _Motion->color_channel(_Data.value<color>(0),
			_Data.values()>1?_Data.value<color>(1):_Data.value<color>(0));
		else _Action->color_channel(_Data.value<color>(0));
	}
	else if (_Data=="opacity") {
				
		if (_Motion) _Motion->opacity(_Data.value<float>(0),
			_Data.values()>1?_Data.value<float>(1):_Data.value<float>(0));
		else _Action->opacity(_Data.value<float>(0));
	}
	else if (_Data=="visible") {
				
		if (_Motion) _Motion->visible(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):_Data.value<bool>(0));
		else _Action->visible(_Data.value<bool>(0));
	}
	else if (_Data=="sound_enabled") {
				
		if (_Motion) _Motion->sound_enabled(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):_Data.value<bool>(0));
		else _Action->sound_enabled(_Data.value<bool>(0));
	}
	else if (_Data=="light_enabled") {
				
		if (_Motion) _Motion->light_enabled(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):_Data.value<bool>(0));
		else _Action->light_enabled(_Data.value<bool>(0));
	}
	else if (_Data=="light_brightness") {
				
		if (_Motion) _Motion->light_brightness(_Data.value<float>(0),
			_Data.values()>1?_Data.value<float>(1):_Data.value<float>(0));
		else _Action->light_brightness(_Data.value<float>(0));
	}
	else if (_Data=="animation_enabled") {
				
		if (_Motion) _Motion->animation_enabled(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):_Data.value<bool>(0));
		else _Action->animation_enabled(_Data.value<bool>(0));
	}
	else if (_Data=="particlesystem_enabled") {
				
		if (_Motion) _Motion->particlesystem_enabled(_Data.value<bool>(0),
			_Data.values()>1?_Data.value<bool>(1):_Data.value<bool>(0));
		else _Action->particlesystem_enabled(_Data.value<bool>(0));
	}
	else if (_Data=="custom") {
				
		if (_Motion) _Motion->custom(_Data.value<float>(0),
			_Data.values()>1?_Data.value<float>(1):
				_Data.value<float>(0));
		else _Action->custom(_Data.value<float>(0));
	}
				
	//Motion properties
	else if (_Object=="motion") {
				
		if (_Data=="technique") {
					
			if (string_utility::compare(_Data.value(0),"lerp"))
				_Motion->technique(entity::motion::LERP);
			else if (string_utility::compare(_Data.value(0),"slerp"))
				_Motion->technique(entity::motion::SLERP);
		}
		else if (_Data=="duration")
			_Motion->duration(_Data.value<float>(0));
	}
}

void entity_manager::actiongroup_properties(entity::action_group *_Action_group,const script_property::data &_Data) {

	if (!_Action_group) return;

	if (_Data=="start")
		_Action_group->start(_Data.value<float>(0));
	else if (_Data=="enabled")
		_Action_group->enabled(_Data.value<bool>(0));
	else if (_Data=="automatic")
		_Action_group->automatic(_Data.value<bool>(0));
	else if (_Data=="loop")
		_Action_group->loop(_Data.value<unsigned int>(0));
}

void entity_manager::init_entityscript(script_parser *_Script_parser) {

script_property *_Property;

//Attachable class

script_class *_Attachable_class=_Script_parser->create_class("attachable");

_Property=_Attachable_class->create_property("blending");
_Property->add_parameter("default_blend,default_alpha_blend,additive_blend,"
	"additive_alpha_blend,light_blend");

_Property=_Attachable_class->create_property("passes");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Attachable_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Attachable_class->create_property("bound_offset");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Attachable_class->create_property("show_bounds");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Attachable_class->create_property("shaderprogram");
_Property->add_parameter(script_property::DT_STRING);
_Property->variable_parameters(true);

_Property=_Attachable_class->create_property("shaderprograms_enabled");
_Property->add_parameter(script_property::DT_BOOL);

//Renderable class

script_class *_Renderable_class=_Script_parser->create_class("renderable");

_Property=_Renderable_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);

_Property=_Renderable_class->create_property("point_rendering");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Renderable_class->create_property("point_size");
_Property->add_parameter(script_property::DT_FLOAT);

//Rectangle class

script_class *_Rectangle_class=_Script_parser->create_class("rectangle");

_Property=_Rectangle_class->create_property("size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Rectangle_class->create_property("origin");
_Property->add_parameter("top_left,top_center,top_right,center_left,"
	"center,center_right,bottom_left,bottom_center,bottom_right");

//Texturable class

script_class *_Texturable_class=_Script_parser->create_class("texturable");

_Property=_Texturable_class->create_property("auto_size");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Texturable_class->create_property("stretch");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Texturable_class->create_property("flip");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Texturable_class->create_property("rotation");
_Property->add_parameter("none,180,90cw,90ccw");

_Property=_Texturable_class->create_property("ratio_mode");
_Property->add_parameter("keep_width,keep_height,fit_inside,fully_cover");

_Property=_Texturable_class->create_property("keep_ratio");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Texturable_class->create_property("cut_mode");
_Property->add_parameter("top_left,top_right,bottom_left,bottom_right");

_Property=_Texturable_class->create_property("crop_size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Texturable_class->create_property("crop_position");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Texturable_class->create_property("auto_crop");
_Property->add_parameter(script_property::DT_BOOL);

//Text class

script_class *_Text_class=_Script_parser->create_class("text");

_Property=_Text_class->create_property("data");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Text_class->create_property("alignment");
_Property->add_parameter("left,center,right");

_Property=_Text_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);

_Property=_Text_class->create_property("wrap_width");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Text_class->create_property("styles_enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Text_class->create_property("line_space");
_Property->add_parameter(script_property::DT_INT32);

_Property=_Text_class->create_property("max_lines");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Text_class->create_property("skip_lines");
_Property->add_parameter(script_property::DT_UINT32);

//Texture class

script_class *_Texture_class=_Script_parser->create_class("texture");

//Animation class

script_class *_Animation_class=_Script_parser->create_class("animation");

_Property=_Animation_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Animation_class->create_property("framerate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Animation_class->create_property("duration");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Animation_class->create_property("loop");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Animation_class->create_property("reverse");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Animation_class->create_property("frame");
_Property->add_parameter(script_property::DT_UINT32);

//Particle system class

script_class *_Particlesystem_class=_Script_parser->create_class("particle_system");

//Component class

script_class *_Component_class=_Script_parser->create_class("component");

_Property=_Component_class->create_property("text");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Component_class->create_property("texture");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Component_class->create_property("animation");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Component_class->create_property("particle_system");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Component_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Component_class->create_property("opacity");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Component_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);

//Light component class

script_class *_Lightcomponent_class=_Script_parser->create_class("light_component");

//Node class

script_class *_Node_class=_Script_parser->create_class("node");

_Property=_Node_class->create_property("draw_order");
_Property->add_parameter(script_property::DT_INT32);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Node_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Node_class->create_property("lookat");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Node_class->create_property("position");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Node_class->create_property("direction");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Node_class->create_property("rotation");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Node_class->create_property("scaling");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Node_class->create_property("rotation_origin");
_Property->add_parameter("parent,local");

_Property=_Node_class->create_property("inherit_rotation");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Node_class->create_property("inherit_scaling");
_Property->add_parameter(script_property::DT_BOOL);

//Sound class

script_class *_Sound_class=_Script_parser->create_class("sound");

_Property=_Sound_class->create_property("paused");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Sound_class->create_property("volume");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Sound_class->create_property("mute");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Sound_class->create_property("attributes");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->required_parameters(1);

_Property=_Sound_class->create_property("distance");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

//Sound group class

script_class *_Soundgroup_class=_Script_parser->create_class("sound_group");

_Property=_Soundgroup_class->create_property("sound");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Soundgroup_class->create_property("volume");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

//Audible class

script_class *_Audible_class=_Script_parser->create_class("audible");

_Property=_Audible_class->create_property("volume");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

//Object class

script_class *_Object_class=_Script_parser->create_class("object");

_Property=_Object_class->create_property("parent");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Object_class->create_property("draw_order");
_Property->add_parameter(script_property::DT_INT32);

_Property=_Object_class->create_property("node");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Object_class->create_property("elevation");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Object_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Object_class->create_property("opacity");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Object_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Object_class->create_property("cast_shadow");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Object_class->create_property("shadow_type");
_Property->add_parameter("global,local");

_Property=_Object_class->create_property("component");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter("text,texture,animation,particle_system");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

//Light class

script_class *_Light_class=_Script_parser->create_class("light");

_Property=_Light_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Light_class->create_property("brightness");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Light_class->create_property("light_component");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter("text,texture,animation,particle_system");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

//Static entity class

script_class *_Staticentity_class=_Script_parser->create_class("static_entity");

_Property=_Staticentity_class->create_property("draw_order");
_Property->add_parameter(script_property::DT_INT32);

_Property=_Staticentity_class->create_property("node");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("elevation");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("opacity");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("sound_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("sound_volume");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("light_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("light_brightness");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("object");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_INT32);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Staticentity_class->create_property("light");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_INT32);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

//Entity class

script_class *_Entity_class=_Script_parser->create_class("entity");

//Action class

script_class *_Action_class=_Script_parser->create_class("action");

_Property=_Action_class->create_property("scope");
_Property->add_parameter("tree,branch,local");
_Property->add_parameter(script_property::DT_STRING);
_Property->required_parameters(1);

_Property=_Action_class->create_property("start");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Action_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Action_class->create_property("translate");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Action_class->create_property("rotate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Action_class->create_property("scale");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Action_class->create_property("position");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->required_parameters(1);

_Property=_Action_class->create_property("rotation");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Action_class->create_property("scaling");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->required_parameters(1);

_Property=_Action_class->create_property("color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter(script_property::DT_COLOR);
_Property->required_parameters(1);

_Property=_Action_class->create_property("opacity");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Action_class->create_property("visible");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Action_class->create_property("sound_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Action_class->create_property("sound_volume");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Action_class->create_property("light_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Action_class->create_property("light_brightness");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

_Property=_Action_class->create_property("animation_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Action_class->create_property("particlesystem_enabled");
_Property->add_parameter(script_property::DT_BOOL);
_Property->add_parameter(script_property::DT_BOOL);
_Property->required_parameters(1);

_Property=_Action_class->create_property("custom");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);
_Property->required_parameters(1);

//Motion class

script_class *_Motion_class=_Script_parser->create_class("motion");

_Property=_Motion_class->create_property("technique");
_Property->add_parameter("lerp,slerp");

_Property=_Motion_class->create_property("duration");
_Property->add_parameter(script_property::DT_FLOAT);

//Action group class

script_class *_Actiongroup_class=_Script_parser->create_class("action_group");

_Property=_Actiongroup_class->create_property("start");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Actiongroup_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Actiongroup_class->create_property("automatic");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Actiongroup_class->create_property("loop");
_Property->add_parameter(script_property::DT_UINT32);

//Scopes

script_scope *_Data_scope=_Script_parser->create_scope("data");
_Data_scope->register_class(_Text_class);
_Data_scope->register_class(_Texture_class);
_Data_scope->register_class(_Animation_class);
_Data_scope->register_class(_Particlesystem_class);

script_scope *_Component_scope=_Script_parser->create_scope("component");
_Component_scope->register_class(_Component_class);
_Component_scope->register_class(_Node_class);
_Component_scope->register_class(_Soundgroup_class);

script_scope *_Lightcomponent_scope=_Script_parser->create_scope("light_component");
_Lightcomponent_scope->register_class(_Lightcomponent_class);

script_scope *_Sound_scope=_Script_parser->create_scope("sound");
_Sound_scope->register_class(_Sound_class);

script_scope *_Object_scope=_Script_parser->create_scope("object");
_Object_scope->register_class(_Object_class);
_Object_scope->register_class(_Light_class);
_Object_scope->register_class(_Node_class);
_Object_scope->register_class(_Soundgroup_class);
_Object_scope->register_class(_Actiongroup_class);

script_scope *_Entity_scope=_Script_parser->create_scope("entity");
_Entity_scope->register_class(_Entity_class);

script_scope *_Action_scope=_Script_parser->create_scope("action");
_Action_scope->register_class(_Action_class);
_Action_scope->register_class(_Motion_class);

_Script_parser->register_scope(_Entity_scope);
_Entity_class->register_scope(_Object_scope);
_Object_class->register_scope(_Component_scope);
_Light_class->register_scope(_Lightcomponent_scope);
_Soundgroup_class->register_scope(_Sound_scope);
_Component_class->register_scope(_Data_scope);
_Actiongroup_class->register_scope(_Action_scope);

//Inheritance

_Renderable_class->register_inheritance(_Attachable_class);
_Rectangle_class->register_inheritance(_Renderable_class);
_Texturable_class->register_inheritance(_Rectangle_class);
_Text_class->register_inheritance(_Attachable_class);
_Texture_class->register_inheritance(_Texturable_class);
_Animation_class->register_inheritance(_Texturable_class);
_Particlesystem_class->register_inheritance(_Attachable_class);
_Lightcomponent_class->register_inheritance(_Component_class);
_Object_class->register_inheritance(_Audible_class);
_Light_class->register_inheritance(_Object_class);
_Staticentity_class->register_inheritance(_Audible_class);
_Entity_class->register_inheritance(_Staticentity_class);
_Motion_class->register_inheritance(_Action_class);
}

bool entity_manager::read_object(entity *&_Entity,const script_class::object &_Object,
	const interval &_Draw_order,bool _Visible) {
	return false;
}

entity *entity_manager::init_entity(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible,script_parser::result::TYPE _Result_type,
		bool _Merge) {

	//Init script parser first time
	if (!_Script_parser) {
	
	_Script_parser=new script_parser();
	init_entityscript(_Script_parser);
	}

script_parser::result _Result(_Result_type,_Merge);

	if (!_Script_parser->parse_script(_Script_data->name(),_Result))
		return 0;

return (_Result_type == script_parser::result::LINEAR ?
	init_entity_linear(_Result, _Draw_order, _Visible) :
	init_entity_tree(_Result, _Draw_order, _Visible));
}

entity *entity_manager::init_entity_linear(const script_parser::result &_Result,
	const interval &_Draw_order,bool _Visible) {

entity *_Entity=0; //Current entity
entity::action_group *_Action_group=0; //Current action group
entity::action *_Action=0; //Current action

object *_Entity_object=0; //Current object
audible::sound_group *_Sound_group=0; //Current sound group
sound *_Sound=0; //Current sound

component *_Component=0; //Current component
attachable *_Attachable=0; //Current attachable

	for (script_class::object::objects_iterator _Iter=_Result.objects_begin(),
		_End=_Result.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
		//Read derived objects
		if (read_object(_Entity,_Object,_Draw_order,_Visible))
			continue;
		
		//Entity
		if (_Object=="entity")
			_Entity=create_entity(_Draw_order+read_draworder(_Object),_Visible);
		//Action group
		else if (_Object=="action_group") {
		
		_Action_group=0;
		
			if (_Entity && !_Object.name().empty())
				_Action_group=_Entity->create_actiongroup(_Object.name());
			
			//Skip object
			if (_Action_group==0) continue;
		}
		//Action
		else if (_Object=="action" || _Object=="motion") {
		
		_Action=0;
		
			if (_Action_group) {
			
				if (_Object=="motion")
					_Action=_Action_group->create_motion();
				else _Action=_Action_group->create_action();
			}
			
			//Skip object
			if (_Action==0) continue;
		}
		//Object
		else if (_Object=="object" || _Object=="light") {
		
		_Entity_object=0;
		
			if (_Entity && !_Object.name().empty()) {
			
			object *_Parent_object=read_parent(_Entity, _Object);
				
				if (_Object=="light") {
				
					if (_Parent_object)
						_Entity_object=_Entity->create_light(_Object.name(),
							_Parent_object,read_draworder(_Object));
					else _Entity_object=_Entity->create_light(_Object.name(),
						read_draworder(_Object));
				}
				else {
				
					if (_Parent_object)
						_Entity_object=_Entity->create_object(_Object.name(),
							_Parent_object,read_draworder(_Object));
					else _Entity_object=_Entity->create_object(_Object.name(),
						read_draworder(_Object));
				}
			}
			
			//Skip object
			if (_Entity_object==0) continue;
		}
		//Sound group
		else if (_Object=="sound_group") {
		
		_Sound_group=0;
		
			if (!_Object.name().empty()) {
			
				if (_Entity && !string_utility::compare(_Object.parent(),"object") &&
					!string_utility::compare(_Object.parent(),"light"))
						_Sound_group=_Entity->create_soundgroup(_Object.name());
				else if (_Entity_object)
					_Sound_group=_Entity_object->create_soundgroup(_Object.name());
			}
			
			//Skip object
			if (_Sound_group==0) continue;
		}
		//Sound
		else if (_Object=="sound") {
		
		_Sound=0;
		
			if (_Sound_group && !_Object.name().empty()) {
			
			bool _Paused=read_paused(_Object);
			
			_Sound=_Sound_group->create_sound(sound_container::
				instance()->data(_Object.name()),_Paused);
			}
			
			//Skip object
			if (_Sound==0) continue;
		}
		//Node
		else if (_Object=="node") {
		
			//Skip object
			if (((string_utility::compare(_Object.parent(),"object") ||
				string_utility::compare(_Object.parent(),"light")) &&
					!_Entity_object) || !_Entity) continue;
		}
		//Component
		else if (_Object=="component" || _Object=="light_component") {
		
		_Component=0;
		
			if (_Entity_object && !_Object.name().empty()) {
			
				if (_Object=="light_component") {
				
				light *_Light=static_cast<light*>(_Entity_object);
				_Component=_Light->create_lightcomponent(_Object.name());
				}
				else _Component=_Entity_object->create_component(_Object.name());
			}
			
			//Skip object
			if (_Component==0) continue;
		}
		//Attachable
		else if (_Object=="text" || _Object=="texture" ||
			_Object=="animation" || _Object=="particle_system") {
		
		_Attachable=0;
		
			if (_Component && !_Object.name().empty()) {
			
				//Text
				if (_Object=="text")
					_Attachable=_Component->data(font_container::
						instance()->data(_Object.name()));
				//Texture
				else if (_Object=="texture")
					_Attachable=_Component->data(texture_container::
						instance()->data(_Object.name()));
				//Animation
				else if (_Object=="animation")
					_Attachable=_Component->data(animation_container::
						instance()->data(_Object.name()));
				//Particle system
				else if (_Object=="particle_system")
					_Attachable=_Component->data(file_container::
						instance()->data(_Object.name()));
			}
			
			//Skip object
			if (_Attachable==0) continue;
		}
		
		//Set properties
		for (script_class::object::data_iterator _Diter=_Object.data_begin(),
			_Dend=_Object.data_end();_Diter!=_Dend;++_Diter) {
		
		const script_property::data &_Data=(*_Diter); //Increase speed
		
			//Entity properties
			if (_Object=="entity")
				entity_properties(_Entity,_Data);
			//Action group properties
			else if (_Object=="action_group")
				actiongroup_properties(_Action_group,_Data);
			//Action properties
			else if (_Object=="action" || _Object=="motion")
				action_properties(_Action,_Object,_Data);
			//Object properties
			else if (_Object=="object" || _Object=="light")
				object_properties(_Entity_object,_Object,_Data);
			//Sound group properties
			else if (_Object=="sound_group")
				soundgroup_properties(_Sound_group,_Data);
			//Sound properties
			else if (_Object=="sound")
				sound_properties(_Sound,_Data);
			//Node properties
			else if (_Object=="node") {

			render_node *_Node=(string_utility::compare(_Object.parent(),"object") ||
				string_utility::compare(_Object.parent(),"light")?
					_Entity_object->node():_Entity->node());
			node_properties(_Node,_Data);
			}
			//Component properties
			else if (_Object=="component" || _Object=="light_component")
				component_properties(_Component,_Data);
			//Attachable properties
			else if (_Object=="text" || _Object=="texture" ||
				_Object=="animation" || _Object=="particle_system")
					attachable_properties(_Attachable, _Environment, _Object, _Data);
		}
	}

return _Entity;
}

entity *entity_manager::init_entity_tree(const script_parser::result &_Result,
	const interval &_Draw_order,bool _Visible) {

entity *_Entity = 0;

	for (script_class::object::objects_iterator _Iter=_Result.objects_begin(),
		_End=_Result.objects_end();_Iter!=_End;++_Iter) {
	
	const script_class::object &_Object=(*_Iter); //Increase speed
	
		//Read derived objects
		if (read_object(_Entity,_Object,_Draw_order,_Visible))
			continue;
		
		//Entity
		if (_Object=="entity")
			_Entity = create_entity(_Draw_order+read_draworder(_Object),_Visible);
		
		read_entity(_Entity, _Object);
	}

return _Entity;
}

//Public

entity_manager::entity_manager(render_node *_Parent_node,int _Draw_order):
	_Script_parser(0) {

_Node=_Parent_node->create_childnode(_Draw_order);

_Node_manager=new node_manager(this,_Node);
_Lighting_manager=new lighting_manager(this,_Node,
	1000,interval(-3,-1)); //Default
}

entity_manager::entity_manager(const entity_manager &_Entity_manager) {

_Node=_Entity_manager._Node->parent_node()->create_childnode(
	_Entity_manager._Node->draw_order());

_Node_manager=new node_manager(this,_Node); //Correct owner
*_Node_manager=*_Entity_manager._Node_manager; //Copy properties

_Lighting_manager=new lighting_manager(this,_Node, //Correct owner
	1000,interval(-3,-1)); //Default
*_Lighting_manager=*_Entity_manager._Lighting_manager; //Copy properties

_Script_parser=0; //Not copyable
copy(_Entity_manager);
}

entity_manager::~entity_manager() {

	if (_Script_parser) delete _Script_parser;

clear_entities();
delete _Lighting_manager;
delete _Node_manager;
}

entity_manager &entity_manager::operator=(const entity_manager &_Entity_manager) {

	//Worth doing
	if (this!=&_Entity_manager) copy(_Entity_manager);

return *this;
}

void entity_manager::update(float _Time) {

	if (!_Myentities.empty()) {
	
		for (_Entities::const_iterator _Iter=_Myentities.begin(),
			_End=_Myentities.end();_Iter!=_End;++_Iter)
				(*_Iter)->update(_Time);
	}
	
	//Lighting changes applied to all entities, reset
	if (_Lighting_manager->_Lighting_changed)
		_Lighting_manager->_Lighting_changed=false;
}

node_manager *entity_manager::node_manager_ptr() const {
	return _Node_manager;
}

lighting_manager *entity_manager::lighting_manager_ptr() const {
	return _Lighting_manager;
}

entity_manager::environment &entity_manager::external() {
	return _Environment;
}

render_node *entity_manager::node() const {
	return _Node;
}

//Entities

entity *entity_manager::create_entity(const interval &_Draw_order,
	bool _Visible) {

entity *_Myentity=new entity(this,_Node,_Draw_order,_Visible);
_Myentities.push_back(_Myentity);
return _Myentity;
}

entity *entity_manager::create_entity(const interval &_Draw_order,
	const entity &_Entity) {

entity *_Myentity=new entity(this,_Node,_Draw_order,_Entity);
_Myentities.push_back(_Myentity);
return _Myentity;
}

entity *entity_manager::create_entity(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {

	if (!_Script_data) return 0;

return init_entity(_Script_data,_Draw_order,_Visible);
}

void entity_manager::create_entities(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {
	create_entity(_Script_data,_Draw_order,_Visible);
}

void entity_manager::remove_entity(entity *&_Entity) {

	for (_Entities::iterator _Iter=_Myentities.begin(),
		_End=_Myentities.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Entity) {
		
		delete _Entity;
		_Entity=0; //Set referenced pointer to null
		
		_Myentities.erase(_Iter);
		return;
		}
	}
}

void entity_manager::clear_entities() {

	for (_Entities::const_iterator _Iter=_Myentities.begin(),
		_End=_Myentities.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myentities.clear();
}

entity_manager::entities_iter entity_manager::entities_begin() const {
	return _Myentities.begin();
}

entity_manager::entities_iter entity_manager::entities_end() const {
	return _Myentities.end();
}