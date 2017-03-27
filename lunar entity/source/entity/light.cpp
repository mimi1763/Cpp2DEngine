/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: light.cpp
---------------------------------------------------
*/

#include "light.h"

//Dependencies (engine)
#include "render_node.h"
#include "attachable.h"

//Dependencies (entity)
#include "light_component.h"
#include "entity_manager.h"
#include "node_manager.h"
#include "lighting_manager.h"

//Protected

light::light(static_entity *_Owner,const std::string &_Name,
	render_node *_Parent_node,int _Draw_order,bool _Visible):
		object(_Owner,_Name,_Parent_node,_Draw_order,_Visible),
			_Enabled(true),_Brightness(1.0f),_Update_light(false) {

_Light_node=_Owner->owner()->node_manager_ptr()->create_node(_Node,
	interval(_Owner->owner()->lighting_manager_ptr()->capture_draworder().from()+1,
		_Owner->owner()->lighting_manager_ptr()->capture_draworder().to()-1),
			_Visible && visible());
_Light_node->onpredraw(&light::pre_draw,this);
_Light_node->onpostdraw(&light::post_draw,this);
}

light::light(const light &_Light):object(_Light),
	_Update_light(false) {

_Light_node=_Owner->owner()->node_manager_ptr()->create_node(_Node,
	*_Light._Light_node,interval(_Owner->owner()->lighting_manager_ptr()->
		capture_draworder().from()+1,_Owner->owner()->lighting_manager_ptr()->
			capture_draworder().to()-1),_Light._Light_node->visible() && visible());
_Light_node->onpredraw(&light::pre_draw,this);
_Light_node->onpostdraw(&light::post_draw,this);

copy(_Light);
}

light::light(static_entity *_Owner,const std::string &_Name,
	render_node *_Parent_node,int _Draw_order,const light &_Light):
		object(_Owner,_Name,_Parent_node,_Draw_order,_Light),
			_Update_light(false) {

_Light_node=_Owner->owner()->node_manager_ptr()->create_node(_Node,
	*_Light._Light_node,interval(_Owner->owner()->lighting_manager_ptr()->
		capture_draworder().from()+1,_Owner->owner()->lighting_manager_ptr()->
			capture_draworder().to()-1),_Light._Light_node->visible() && visible());
_Light_node->onpredraw(&light::pre_draw,this);
_Light_node->onpostdraw(&light::post_draw,this);

copy(_Light);
}

light::~light() {
	clear_lightcomponents();
}

void light::copy(const light &_Light) {

_Enabled=_Light._Enabled;
_Brightness=_Light._Brightness;

	//Copy light components
	if (!_Light._Mylight_components.empty()) {
	
	component *_Component;
	
		for (_Components::const_iterator _Iter=_Light._Mylight_components.begin(),
			_End=_Light._Mylight_components.end();_Iter!=_End;++_Iter) {
		
		_Component=(*_Iter); //Increase speed
		create_lightcomponent(_Component->name(),*_Component);
		}
	}
}

void light::update_lights() {

	if (_Update_light) {
	
	component *_Component;
	
		//Add light to corresponding component
		for (_Components::const_iterator _Iter=_Mycomponents.begin(),
			_End=_Mycomponents.end();_Iter!=_End;++_Iter) {
		
		_Component=(*_Iter); //Increase speed
		
			//Component is missing light component
			if (!lightcomponent_pointer(_Component->name()))
				add_light(_Component);
		}
	
	_Update_light=false;
	}
}

void light::add_light(component *_Component) {

	//Reuse particle system as light component (don't copy)
	if (_Component->type()==component::PARTICLE_SYSTEM) {
	
		//Light component doesn't exist
		if (!lightcomponent_pointer(_Component->name()))
			_Mylight_components.push_back(new light_component(
				this,_Component->name())); //Add empty light component
	
	return;
	}

create_lightcomponent(_Component->name(),*_Component); //Create light component as copy
}

void light::switch_node(render_node *_Render_node) {

component *_Component;

	for (_Components::const_iterator _Iter=_Mylight_components.begin(),
		_End=_Mylight_components.end();_Iter!=_End;++_Iter) {
	
	_Component=(*_Iter); //Increase speed
	
		//Empty light component defined for this component,
		//render corresponding component as light component
		if (!_Component->data()) {
		
		_Component=component_pointer(_Component->name());
		
			if (_Component) {
			
			//Static variable to remember blending between pre and post draw
			static render_manager::BLENDING _Blending=render_manager::
				DEFAULT_ALPHA_BLEND;
			
				//Switch component from node to light node (pre draw)
				if (_Render_node==_Light_node) {
				
				_Node->detach(_Component->data());
				
				_Blending=_Component->data()->blending(); //Keep original blending
				_Component->data()->blending(render_manager::LIGHT_BLEND);
				_Light_node->attach(_Component->data());
				}
				//Switch component from light node to node (post draw)
				else if (_Render_node==_Node) {
				
				_Light_node->detach(_Component->data());
				
				_Component->data()->blending(_Blending); //Revert back to original blending
				_Node->attach(_Component->data());
				}
			}
		}
	}
}

void light::pre_draw(render_node *_Render_node) {
	switch_node(_Light_node);
}

void light::post_draw(render_node *_Render_node) {
	switch_node(_Node);
}

//Public

light &light::operator=(const light &_Light) {

	//Worth doing
	if (this!=&_Light) {
	
	clear_lightcomponents();
	
	object::operator=(_Light); //Invoke base operator=
	copy(_Light);
	node_manager::copy_node(*_Light_node,*_Light._Light_node);
	}

return *this;
}

void light::update(float _Time) {

	//Only update if light node is visible
	if (_Light_node->visible()) update_lights();

object::update(_Time); //Use base functionality
}

void light::opacity(float _Opacity,bool _Base) {

object::opacity(_Opacity,_Base); //Use base functionality
brightness(_Opacity,_Base);
}

float light::opacity() const {
	return object::opacity(); //Must redirect because of name reuse
}

void light::visible(bool _Visible,bool _Base) {

object::visible(_Visible,_Base); //Use base functionality
_Light_node->visible(_Enabled && visible());
}

bool light::visible() const {
	return object::visible(); //Must redirect because of name reuse
}

void light::enabled(bool _Enabled) {

this->_Enabled=_Enabled;
_Light_node->visible(_Enabled && visible());
}

bool light::enabled() const {
	return _Light_node->visible();
}

void light::brightness(float _Brightness,bool _Base) {

	//Change opacity of all light components
	for (_Components::iterator _Iter=_Mylight_components.begin(),
		_End=_Mylight_components.end();_Iter!=_End;++_Iter)
			(*_Iter)->opacity(_Brightness,_Base);

this->_Brightness=_Brightness;
}

float light::brightness() const {
	return _Brightness;
}

render_node *light::light_node() const {
	return _Light_node;
}

//Components

component *light::create_component(const std::string &_Name,
	resource_data *_Resource_data,bool _Visible) {

component *_Mycomponent=object::create_component(_Name,_Resource_data,_Visible);

_Update_light=true; //Create light component
return _Mycomponent;
}

component *light::create_component(const std::string &_Name,
	const component &_Component) {

component *_Mycomponent=object::create_component(_Name,_Component);

_Update_light=true; //Create light component
return _Mycomponent;
}

void light::remove_component(component *&_Component) {

component *_Mycomponent=lightcomponent_pointer(_Component->name());

	//Remove corresponding light component
	if (_Mycomponent) remove_lightcomponent(_Mycomponent);

object::remove_component(_Component);
}

void light::clear_components() {

clear_lightcomponents();
object::clear_components();
}

//Light components

component *light::create_lightcomponent(const std::string &_Name,
	bool _Visible) {

	//Light component already exist
	if (lightcomponent_pointer(_Name)) return 0;

return create_lightcomponent(_Name,0,_Visible);
}

component *light::create_lightcomponent(const std::string &_Name,
	resource_data *_Resource_data,bool _Visible) {

component *_Mycomponent=lightcomponent_pointer(_Name);

	//Light component does not exist
	if (!_Mycomponent) {
	
	_Mycomponent=new light_component(this,_Name,_Visible);
	_Mycomponent->color_channel(_Color); //Set default color
	_Mycomponent->opacity(_Brightness); //Set default brightness
	_Mylight_components.push_back(_Mycomponent);
	}

_Mycomponent->data(_Resource_data);
return _Mycomponent;
}

component *light::create_lightcomponent(const std::string &_Name,
	const component &_Component) {

component *_Mycomponent=lightcomponent_pointer(_Name);

	//Light component already exist
	if (_Mycomponent) (*_Mycomponent)=_Component;
	else {
	
	_Mycomponent=new light_component(this,_Name,_Component);
	_Mycomponent->opacity(_Brightness); //Set default brightness
	_Mylight_components.push_back(_Mycomponent);
	}

return _Mycomponent;
}

void light::remove_lightcomponent(component *&_Component) {

	for (_Components::iterator _Iter=_Mylight_components.begin(),
		_End=_Mylight_components.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Component) {
		
		delete static_cast<light_component*>(_Component);
		_Component=0; //Set referenced pointer to null
		
		_Mylight_components.erase(_Iter);
		return;
		}
	}
}

void light::clear_lightcomponents() {

	for (_Components::const_iterator _Iter=_Mylight_components.begin(),
		_End=_Mylight_components.end();_Iter!=_End;++_Iter)
			delete static_cast<light_component*>(*_Iter);

_Mylight_components.clear();
}

light::components_iter light::lightcomponents_begin() const {
	return _Mylight_components.begin();
}

light::components_iter light::lightcomponents_end() const {
	return _Mylight_components.end();
}

component *light::lightcomponent_pointer(const std::string &_Name) const {

	for (_Components::const_iterator _Iter=_Mylight_components.begin(),
		_End=_Mylight_components.end();_Iter!=_End;++_Iter) {
	
		//Light component exist, return pointer
		if ((*_Iter)->name()==_Name) return (*_Iter);
	}

return 0;
}