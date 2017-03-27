/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: static_entity.h
---------------------------------------------------
*/

#ifndef _STATIC_ENTITY_
#define _STATIC_ENTITY_

//Dependencies (entity)
#include "light.h"
#include "interval.h"

//Forward declarations (entity)
class entity_manager;

class static_entity:public audible {

public:

	typedef std::list<object*> objects;

protected:

	struct _Branch {
	
	object *_Object;
	object *_Parent_object;
	};
	
	typedef std::list<_Branch*> _Branches;
	
	entity_manager *_Owner;
	interval _Draw_order;
	
	float _Elevation;
	color _Color;
	
	bool _Sound_enabled;
	float _Sound_volume;
	bool _Light_enabled;
	float _Light_brightness;
	
	render_node *_Node;
	render_node *_Root_node;
	_Branches _Mybranches;
	
	//Protected constructor
	static_entity(entity_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,bool _Visible=true);
	
	//Protected copy constructor
	static_entity(const static_entity &_Static_entity);
	
	//Protected alternative copy constructor
	static_entity(entity_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,const static_entity &_Static_entity);
	
	//Protected virtual destructor
	virtual ~static_entity();
	
	void copy(const static_entity &_Static_entity);
	
	virtual void object_created(object *_Object);
	virtual void object_removed(object *_Object);
	
	//Audible
	
	virtual sound_manager *sound_owner() const;
	virtual render_node *sound_node() const;
	
	//Branches
	
	void create_branch(object *_Object,object *_Parent_object);

	_Branch *branch_pointer(const std::string &_Name) const;
	
	void traverse(_Branch *_Branch,_Branches &_Mybranches) const;
	void traverse(object *_Object,objects &_Objects) const;

public:

	static_entity &operator=(const static_entity &_Static_entity);
	
	virtual void update(float _Time);
	
	entity_manager *owner() const;
	const interval &draw_order() const;
	render_node *node() const;

	void append(const static_entity &_Static_entity);
	
	void elevation(float _Elevation);
	void elevation(object *_Parent_object,float _Elevation,
		bool _Forced=false);
	float elevation() const;
	
	void color_channel(const color &_Color);
	void color_channel(object *_Parent_object,const color &_Color,
		bool _Forced=false);
	const color &color_channel() const;
	
	void opacity(float _Opacity);
	void opacity(object *_Parent_object,float _Opacity,
		bool _Forced=false);
	float opacity() const;
	
	void visible(bool _Visible);
	void visible(object *_Parent_object,bool _Visible,
		bool _Forced=false);
	bool visible() const;
	
	//Sound specific
	
	void sound_enabled(bool _Enabled);
	void sound_enabled(object *_Parent_object,bool _Enabled,
		bool _Forced=false);
	bool sound_enabled() const;
	
	void sound_volume(float _Volume);
	void sound_volume(object *_Parent_object,float _Volume,
		bool _Forced=false);
	float sound_volume() const;
	
	//Light specific
	
	void light_enabled(bool _Enabled);
	void light_enabled(object *_Parent_object,bool _Enabled);
	bool light_enabled() const;
	
	void light_brightness(float _Brightness);
	void light_brightness(object *_Parent_object,float _Brightness,
		bool _Forced=false);
	float light_brightness() const;
	
	//Objects
	
	object *create_object(const std::string &_Name,int _Draw_order,
		bool _Visible=true);
	object *create_object(const std::string &_Name,object *_Parent_object,
		int _Draw_order,bool _Visible=true);
	object *create_object(const std::string &_Name,int _Draw_order,
		const object &_Object);
	object *create_object(const std::string &_Name,object *_Parent_object,
		int _Draw_order,const object &_Object);
	
	light *create_light(const std::string &_Name,int _Draw_order,
		bool _Visible=true);
	light *create_light(const std::string &_Name,object *_Parent_object,
		int _Draw_order,bool _Visible=true);
	light *create_light(const std::string &_Name,int _Draw_order,
		const light &_Light);
	light *create_light(const std::string &_Name,object *_Parent_object,
		int _Draw_order,const light &_Light);
	
	void remove_object(object *&_Object);
	void clear_objects();
	
	object *object_pointer(const std::string &_Name) const;
	object *object_parent(const std::string &_Name) const;
	
	void object_pointers(objects &_Objects) const;
	void object_pointers(object *_Parent_object,objects &_Objects) const;
};

#endif