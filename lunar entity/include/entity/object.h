/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: object.h
---------------------------------------------------
*/

#ifndef _OBJECT_
#define _OBJECT_

#include <list>

//Dependencies (entity)
#include "audible.h"
#include "component.h"

//Forward declarations (engine)
namespace lunar {
	class render_node;
} //namespace lunar

//Forward declarations (entity)
class static_entity;

class object:public audible {

	friend static_entity;

public:

	enum SHADOW_TYPE { GLOBAL, LOCAL };

protected:

	enum SHADOW_STATUS { OKAY, NEED_UPDATE, RECREATE };
	
	typedef std::list<component*> _Components;
	
	static_entity *_Owner;
	std::string _Name;
	int _Draw_order;
	
	float _Elevation;
	color _Color;
	bool _Visible;
	
	bool _Cast_shadow;
	SHADOW_TYPE _Shadow_type;
	SHADOW_STATUS _Shadow_status;
	
	render_node *_Node;
	render_node *_Shadow_node;
	
	_Components _Mycomponents;
	_Components _Myshadow_components;
	
	//Protected constructor
	object(static_entity *_Owner,const std::string &_Name,render_node *_Parent_node,
		int _Draw_order,bool _Visible=true);
	
	//Protected copy constructor
	object(const object &_Object);
	
	//Protected alternative copy constructor
	object(static_entity *_Owner,const std::string &_Name,render_node *_Parent_node,
		int _Draw_order,const object &_Object);
	
	//Protected virtual destructor
	virtual ~object();
	
	void copy(const object &_Object);
	
	//Audible
	
	virtual sound_manager *sound_owner() const;
	virtual render_node *sound_node() const;
	
	//Shadows
	
	void update_shadows();
	
	void add_shadow(component *_Component);
	
	void remove_shadow(component *_Component);
	void clear_shadows();

public:

	typedef _Components::const_iterator components_iter; //Keep data safe
	
	object &operator=(const object &_Object);
	
	virtual void update(float _Time);
	
	static_entity *owner() const;
	const std::string &name() const;
	int draw_order() const;
	render_node *node() const;
	
	void elevation(float _Elevation,bool _Base=true);
	float elevation() const;
	
	void color_channel(const color &_Color,bool _Base=true);
	const color &color_channel() const;
	
	virtual void opacity(float _Opacity,bool _Base=true);
	float opacity() const;
	
	virtual void visible(bool _Visible,bool _Base=true);
	bool visible() const;
	
	//Shadows
	
	void cast_shadow(bool _Cast_shadow);
	bool cast_shadow() const;
	
	void shadow_type(const SHADOW_TYPE &_Shadow_type);
	SHADOW_TYPE shadow_type() const;
	
	render_node *shadow_node() const;
	
	//Components
	
	component *create_component(const std::string &_Name,
		bool _Visible=true);
	virtual component *create_component(const std::string &_Name,
		resource_data *_Resource_data,bool _Visible=true);
	virtual component *create_component(const std::string &_Name,
		const component &_Component);
	
	virtual void remove_component(component *&_Component);
	virtual void clear_components();
	
	components_iter components_begin();
	components_iter components_end();
	
	component *component_pointer(const std::string &_Name);
};

#endif