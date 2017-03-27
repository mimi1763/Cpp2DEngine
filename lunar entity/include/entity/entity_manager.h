/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: entity_manager.h
---------------------------------------------------
*/

#ifndef _ENTITY_MANAGER_
#define _ENTITY_MANAGER_

#include <list>

//Dependencies (engine)
#include "script_parser.h"

//Dependencies (entity)
#include "entity.h"

//Forward declarations (engine)
namespace lunar {

class timer_manager;
class sound_manager;

class font_manager;
class text_manager;
class texture_manager;
class animation_manager;
class particlesystem_manager;

class shader_manager;
class shaderprogram_manager;
class framebuffer_manager;
class scene_manager;

class render_node;
} //namespace lunar

//Forward declarations (entity)
class node_manager;
class lighting_manager;

using namespace lunar;

class entity_manager {

public:

	struct environment {
	
	private:
	
		timer_manager *_Timer_manager;
		sound_manager *_Sound_manager;
		
		font_manager *_Font_manager;
		text_manager *_Text_manager;
		texture_manager *_Texture_manager;
		animation_manager *_Animation_manager;
		particlesystem_manager *_Particlesystem_manager;
		
		shader_manager *_Shader_manager;
		shaderprogram_manager *_Shaderprogram_manager;
		framebuffer_manager *_Framebuffer_manager;
		scene_manager *_Scene_manager;
		
		void copy(const environment &_Environment);
	
	public:
	
		//Constructor
		environment();
		
		//Copy constructor
		environment(const environment &_Environment);
		
		//Destructor
		~environment();
		
		environment &operator=(const environment &_Environment);
		
		void managers(timer_manager *_Timer_manager=0,
					  sound_manager *_Sound_manager=0,
					  font_manager *_Font_manager=0,
					  text_manager *_Text_manager=0,
					  texture_manager *_Texture_manager=0,
					  animation_manager *_Animation_manager=0,
					  particlesystem_manager *_Particlesystem_manager=0,
					  shader_manager *_Shader_manager=0,
					  shaderprogram_manager *_Shaderprogram_manager=0,
					  framebuffer_manager *_Framebuffer_manager=0,
					  scene_manager *_Scene_manager=0);
		
		timer_manager *timer_manager_ptr() const;
		sound_manager *sound_manager_ptr() const;
		
		font_manager *font_manager_ptr() const;
		text_manager *text_manager_ptr() const;
		texture_manager *texture_manager_ptr() const;
		animation_manager *animation_manager_ptr() const;
		particlesystem_manager *particlesystem_manager_ptr() const;
		
		shader_manager *shader_manager_ptr() const;
		shaderprogram_manager *shaderprogram_manager_ptr() const;
		framebuffer_manager *framebuffer_manager_ptr() const;
		scene_manager *scene_manager_ptr() const;
	};

protected:

	typedef std::list<entity*> _Entities;
	
	node_manager *_Node_manager;
	lighting_manager *_Lighting_manager;
	environment _Environment;
	
	render_node *_Node;
	_Entities _Myentities;
	
	script_parser *_Script_parser;
	
	void copy(const entity_manager &_Entity_manager);
	
	//Script specific

	static int read_draworder(const script_class::object &_Object);
	static object *read_parent(entity *_Entity,const script_class::object &_Object);
	static bool read_paused(const script_class::object &_Object);
	
	attachable *read_attachable(component *_Component,const script_class::object &_Object);

	component *read_component(object *_Entity_object,const script_class::object &_Object);
	render_node *read_node(entity *_Entity, object *_Entity_object,const script_class::object &_Object);
	sound *read_sound(entity::sound_group *_Sound_group,const script_class::object &_Object);
	audible::sound_group *read_soundgroup(entity *_Entity,object *_Entity_object,const script_class::object &_Object);
	object *read_entityobject(entity *_Entity,const script_class::object &_Object);

	entity *read_entity(entity *_Entity,const script_class::object &_Object);
	entity::action *read_action(entity::action_group *_Action_group,const script_class::object &_Object);
	entity::action_group *read_actiongroup(entity *_Entity,const script_class::object &_Object);

	static void attachable_properties(attachable *_Attachable,const environment &_Environment,
		const script_class::object &_Object,const script_property::data &_Data);

	static void component_properties(component *_Component,const script_property::data &_Data);
	static void node_properties(render_node *_Node,const script_property::data &_Data);
	static void sound_properties(sound *_Sound,const script_property::data &_Data);
	static void soundgroup_properties(audible::sound_group *_Sound_group,const script_property::data &_Data);
	static void object_properties(object *_Entity_object,const script_class::object &_Object,
		const script_property::data &_Data);

	static void entity_properties(entity *_Entity,const script_property::data &_Data);
	static void action_properties(entity::action *_Action,const script_class::object &_Object,
		const script_property::data &_Data);
	static void actiongroup_properties(entity::action_group *_Action_group,const script_property::data &_Data);
	
	virtual void init_entityscript(script_parser *_Script_parser);
	virtual bool read_object(entity *&_Entity,const script_class::object &_Object,
		const interval &_Draw_order,bool _Visible);
	
	entity *init_entity(file_data *_Script_data,
		const interval &_Draw_order,bool _Visible=true,
			script_parser::result::TYPE _Result_type = script_parser::result::LINEAR,
				bool _Merge = true);

	entity *init_entity_linear(const script_parser::result &_Result,
		const interval &_Draw_order,bool _Visible=true);
	entity *init_entity_tree(const script_parser::result &_Result,
		const interval &_Draw_order,bool _Visible=true);

public:

	typedef _Entities::const_iterator entities_iter; //Keep data safe
	
	//Constructor
	entity_manager(render_node *_Parent_node,int _Draw_order);
	
	//Copy constructor
	entity_manager(const entity_manager &_Entity_manager);
	
	//Virtual destructor
	virtual ~entity_manager();
	
	entity_manager &operator=(const entity_manager &_Entity_manager);
	
	virtual void update(float _Time);
	
	node_manager *node_manager_ptr() const;
	lighting_manager *lighting_manager_ptr() const;
	
	environment &external();
	
	render_node *node() const;
	
	//Entities
	
	entity *create_entity(const interval &_Draw_order,bool _Visible=true);
	entity *create_entity(const interval &_Draw_order,const entity &_Entity);
	
	entity *create_entity(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=true);
	void create_entities(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=true);
	
	void remove_entity(entity *&_Entity);
	void clear_entities();
	
	entities_iter entities_begin() const;
	entities_iter entities_end() const;
};

#endif