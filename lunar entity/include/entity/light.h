/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: light.h
---------------------------------------------------
*/

#ifndef _LIGHT_
#define _LIGHT_

//Dependencies (entity)
#include "object.h"

class light:public object {

	friend static_entity;

protected:

	bool _Enabled;
	float _Brightness;
	bool _Update_light;
	
	render_node *_Light_node;
	_Components _Mylight_components;
	
	//Protected constructor
	light(static_entity *_Owner,const std::string &_Name,render_node *_Parent_node,
		int _Draw_order,bool _Visible=true);
	
	//Protected copy constructor
	light(const light &_Light);
	
	//Protected alternative copy constructor
	light(static_entity *_Owner,const std::string &_Name,render_node *_Parent_node,
		int _Draw_order,const light &_Light);
	
	//Protected virtual destructor
	virtual ~light();
	
	void copy(const light &_Light);
	
	//Lights
	
	void update_lights();
	
	void add_light(component *_Component);
	void switch_node(render_node *_Render_node);
	
	void pre_draw(render_node *_Render_node);
	void post_draw(render_node *_Render_node);

public:

	light &operator=(const light &_Light);
	
	virtual void update(float _Time);
	
	virtual void opacity(float _Opacity,bool _Base=true);
	float opacity() const;
	
	virtual void visible(bool _Visible,bool _Base=true);
	bool visible() const;
	
	void enabled(bool _Enabled);
	bool enabled() const;
	
	void brightness(float _Brightness,bool _Base=true);
	float brightness() const;
	
	render_node *light_node() const;
	
	//Components
	
	virtual component *create_component(const std::string &_Name,
		resource_data *_Resource_data,bool _Visible=true);
	virtual component *create_component(const std::string &_Name,
		const component &_Component);
	
	virtual void remove_component(component *&_Component);
	virtual void clear_components();
	
	//Light components
	
	component *create_lightcomponent(const std::string &_Name,
		bool _Visible=true);
	component *create_lightcomponent(const std::string &_Name,
		resource_data *_Resource_data,bool _Visible=true);
	component *create_lightcomponent(const std::string &_Name,
		const component &_Component);
	
	void remove_lightcomponent(component *&_Component);
	void clear_lightcomponents();
	
	components_iter lightcomponents_begin() const;
	components_iter lightcomponents_end() const;
	
	component *lightcomponent_pointer(const std::string &_Name) const;
};

#endif