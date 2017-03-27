/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: object.cpp
---------------------------------------------------
*/

#include "object.h"

//Dependencies (engine)
#include "render_node.h"
#include "text.h"
#include "math_utility.h"

//Dependencies (entity)
#include "shadow_component.h"
#include "entity_manager.h"
#include "node_manager.h"
#include "lighting_manager.h"

//Protected

object::object(static_entity *_Owner,const std::string &_Name,
	render_node *_Parent_node,int _Draw_order,bool _Visible):
		_Owner(_Owner),_Name(_Name),_Draw_order(_Draw_order),_Elevation(0.0f),
			_Visible(_Visible),_Cast_shadow(false),_Shadow_type(GLOBAL),
				_Shadow_status(OKAY),_Shadow_node(0) {
	_Node=_Owner->owner()->node_manager_ptr()->create_node(_Parent_node,
		_Owner->node()->draw_order()+_Draw_order*2, //Relative with shadow
			_Visible && _Owner->visible());
}

object::object(const object &_Object):audible(_Object),_Owner(_Object._Owner),
	_Name(_Object._Name),_Draw_order(_Object._Draw_order),
		_Shadow_status(OKAY),_Shadow_node(0) {

_Node=_Owner->owner()->node_manager_ptr()->create_node(
	_Object._Node->parent_node(),*_Object._Node,_Object._Node->
		draw_order(),_Object._Node->visible() && _Owner->visible());
copy(_Object);
}

object::object(static_entity *_Owner,const std::string &_Name,
	render_node *_Parent_node,int _Draw_order,const object &_Object):
		audible(_Object),_Owner(_Owner),_Name(_Name),_Draw_order(_Draw_order),
			_Shadow_status(OKAY),_Shadow_node(0) {

_Node=_Owner->owner()->node_manager_ptr()->create_node(_Parent_node,
	*_Object._Node,_Owner->node()->draw_order()+_Draw_order*2, //Relative with shadow
		_Object._Node->visible() && _Owner->visible());
copy(_Object);
}

object::~object() {

clear_components();
_Owner->owner()->node_manager_ptr()->recycle_node(_Node);
}

void object::copy(const object &_Object) {

_Elevation=_Object._Elevation;
_Color=_Object._Color;
_Visible=_Object._Visible;

_Cast_shadow=_Object._Cast_shadow;
_Shadow_type=_Object._Shadow_type;

	//Copy components
	if (!_Object._Mycomponents.empty()) {
	
	component *_Component;
	
		for (_Components::const_iterator _Iter=_Object._Mycomponents.begin(),
			_End=_Object._Mycomponents.end();_Iter!=_End;++_Iter) {
		
		_Component=(*_Iter); //Increase speed
		create_component(_Component->name(),*_Component);
		}
	}
}

//Audible

sound_manager *object::sound_owner() const {
	return _Owner->owner()->external().sound_manager_ptr();
}

render_node *object::sound_node() const {
	return _Node;
}

//Shadows

void object::update_shadows() {

lighting_manager *_Lighting_manager=_Owner->owner()->lighting_manager_ptr();

	if (_Cast_shadow && _Lighting_manager->cast_shadows() &&
		!_Mycomponents.empty()) {
	
		//Recreate shadows
		if (_Shadow_status==RECREATE || !_Shadow_node) {
		
		clear_shadows();
		
			//Add shadow to each component
			for (_Components::const_iterator _Iter=_Mycomponents.begin(),
				_End=_Mycomponents.end();_Iter!=_End;++_Iter)
					add_shadow(*_Iter);
		
		_Shadow_status=NEED_UPDATE;
		}
		
		//Update shadows
		if (_Shadow_status==NEED_UPDATE) {
		
			//Local
			if (_Shadow_type==LOCAL) {
			
				//Change shadow type to local
				if (_Shadow_node->draw_order()!=_Node->draw_order()-1)
					_Shadow_node->draw_order(_Node->draw_order()-1,false);
			}
			//Global
			else {
			
				//Change shadow type to global
				if (_Shadow_node->draw_order()==_Node->draw_order()-1)
					_Shadow_node->draw_order(_Owner->node()->draw_order(),false);
			}
		}
		
		//Update shadow positions and colors
		if (_Shadow_status==NEED_UPDATE || _Lighting_manager->lighting_changed()) {
		
		//Update shadow direction
		_Shadow_node->position(_Lighting_manager->light_direction()*_Elevation);
		
		//Calculate shadow color
		color _Shadow_color=_Lighting_manager->shadowcolor_day().
			mix(_Lighting_manager->shadowcolor_night(),
				_Lighting_manager->overlay_opacity());
		
			for (_Components::const_iterator _Iter=_Mycomponents.begin(),
				_Siter=_Myshadow_components.begin(),_End=_Mycomponents.end(),
					_Send=_Myshadow_components.end();_Iter!=_End && _Siter!=_Send;
						++_Iter,++_Siter) {
			
			//Update shadow component color with adjusted opacity
			color _Color=_Shadow_color;
			_Color.a*=(*_Iter)->opacity();
			(*_Siter)->color_channel(_Color);
			}
		}
	}
	//Recycle shadow node
	else if (_Shadow_node) {
	
	clear_shadows();
	_Owner->owner()->node_manager_ptr()->recycle_node(_Shadow_node);
	}

_Shadow_status=OKAY;
}

void object::add_shadow(component *_Component) {

	//Initialize shadow node
	if (!_Shadow_node) {
	
	_Shadow_node=_Owner->owner()->node_manager_ptr()->create_node(
		_Node,_Shadow_type==LOCAL?_Node->draw_order()-1:
			_Owner->node()->draw_order());
	_Shadow_node->rotation_origin(render_node::LOCAL);
	}

//Add empty shadow component if particle system, else make copy
component *_Shadow_component=(_Component->type()==component::PARTICLE_SYSTEM?
	new shadow_component(this,_Component->name()):new shadow_component(*_Component));

	//Disable styles for text shadows
	if (_Shadow_component->type()==component::TEXT)
		static_cast<text*>(_Shadow_component->data())->
			styles_enabled(false);

_Myshadow_components.push_back(_Shadow_component);
}

void object::remove_shadow(component *_Component) {

component *_Shadow_component;

	for (_Components::iterator _Iter=_Myshadow_components.begin(),
		_End=_Myshadow_components.end();_Iter!=_End;++_Iter) {
	
	_Shadow_component=(*_Iter); //Increase speed
	
		//Find corresponding shadow
		if (_Shadow_component->name()==_Component->name()) {
		
		delete _Shadow_component;		
		_Myshadow_components.erase(_Iter);
		return;
		}
	}
}

void object::clear_shadows() {

	for (_Components::const_iterator _Iter=_Myshadow_components.begin(),
		_End=_Myshadow_components.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Myshadow_components.clear();
}

//Public

object &object::operator=(const object &_Object) {

	//Worth doing
	if (this!=&_Object) {
	
	object::clear_components(); //Don't polymorph
	
	audible::operator=(_Object); //Invoke base operator=
	copy(_Object);
	
	node_manager::copy_node(*_Node,*_Object._Node);
	}

return *this;
}

void object::update(float _Time) {

	//Only update if node is visible
	if (_Node->visible()) update_shadows();

audible::update(_Time);
}

static_entity *object::owner() const {
	return _Owner;
}

const std::string &object::name() const {
	return _Name;
}

int object::draw_order() const {
	return _Draw_order;
}

render_node *object::node() const {
	return _Node;
}

void object::elevation(float _Elevation,bool _Base) {

	if ((_Base && this->_Elevation!=_Elevation) ||
		(!_Base && _Elevation!=0.0f)) {
	
		if (_Base) this->_Elevation=_Elevation;
		//Relative
		else this->_Elevation+=_Elevation; 
		
		//Update shadows
		if (_Shadow_status==OKAY) _Shadow_status=NEED_UPDATE;
	}
}

float object::elevation() const {
	return _Elevation;
}

void object::color_channel(const color &_Color,bool _Base) {

	if (_Base) this->_Color=_Color;
	
	//Something to do
	if (!_Mycomponents.empty()) {
	
		//Change color of all components
		for (_Components::const_iterator _Iter=_Mycomponents.begin(),
			_End=_Mycomponents.end();_Iter!=_End;++_Iter)
				(*_Iter)->color_channel(_Color,_Base);
		
		//Update shadows
		if (_Shadow_status==OKAY) _Shadow_status=NEED_UPDATE;
	}
}

const color &object::color_channel() const {
	return _Color;
}

void object::opacity(float _Opacity,bool _Base) {

	if (_Base) _Color.a=_Opacity;
	
	//Something to do
	if (!_Mycomponents.empty()) {
	
		//Change opacity of all components
		for (_Components::const_iterator _Iter=_Mycomponents.begin(),
			_End=_Mycomponents.end();_Iter!=_End;++_Iter)
				(*_Iter)->opacity(_Opacity,_Base);
		
		//Update shadows
		if (_Shadow_status==OKAY) _Shadow_status=NEED_UPDATE;
	}
}

float object::opacity() const {
	return _Color.a;
}

void object::visible(bool _Visible,bool _Base) {

	if (_Base) this->_Visible=_Visible;

_Node->visible(this->_Visible && _Visible && _Owner->visible(),false);

	if (_Shadow_node) _Shadow_node->visible(_Node->visible());
}

bool object::visible() const {
	return _Node->visible();
}

//Shadows

void object::cast_shadow(bool _Cast_shadow) {

	if (this->_Cast_shadow!=_Cast_shadow) {
	
	this->_Cast_shadow=_Cast_shadow;
	_Shadow_status=RECREATE; //Recreate shadows
	}
}

bool object::cast_shadow() const {
	return _Cast_shadow;
}

void object::shadow_type(const SHADOW_TYPE &_Shadow_type) {

	if (this->_Shadow_type!=_Shadow_type) {
	
	this->_Shadow_type=_Shadow_type;
	
		//Update shadows
		if (_Shadow_status==OKAY) _Shadow_status=NEED_UPDATE;
	}
}

object::SHADOW_TYPE object::shadow_type() const {
	return _Shadow_type;
}

render_node *object::shadow_node() const {
	return _Shadow_node;
}

//Components

component *object::create_component(const std::string &_Name,
	bool _Visible) {

	//Component already exist
	if (component_pointer(_Name)) return 0;

return create_component(_Name,0,_Visible);
}

component *object::create_component(const std::string &_Name,
	resource_data *_Resource_data,bool _Visible) {

component *_Mycomponent=component_pointer(_Name);

	//Component already exist
	if (_Mycomponent) {
	
		//Remove shadow
		if (_Cast_shadow) remove_shadow(_Mycomponent);
	}
	else {
	
	_Mycomponent=new component(this,_Name,_Visible);
	_Mycomponent->color_channel(_Color); //Set default color
	_Mycomponents.push_back(_Mycomponent);
	}

_Mycomponent->data(_Resource_data);
_Shadow_status=RECREATE; //Recreate shadows
return _Mycomponent;
}

component *object::create_component(const std::string &_Name,
	const component &_Component) {

component *_Mycomponent=component_pointer(_Name);

	//Component already exist
	if (_Mycomponent) {
	
		//Remove shadow
		if (_Cast_shadow) remove_shadow(_Mycomponent);
	
	(*_Mycomponent)=_Component;
	}
	else {
	
	_Mycomponent=new component(this,_Name,_Component);
	_Mycomponents.push_back(_Mycomponent);
	}

_Shadow_status=RECREATE; //Recreate shadows
return _Mycomponent;
}

void object::remove_component(component *&_Component) {

	for (_Components::iterator _Iter=_Mycomponents.begin(),
		_End=_Mycomponents.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Component) {
		
			//Remove shadow
			if (_Cast_shadow) remove_shadow(_Component);
		
		delete _Component;
		_Component=0; //Set referenced pointer to null
		
		_Mycomponents.erase(_Iter);
		return;
		}
	}
}

void object::clear_components() {

clear_shadows();

	for (_Components::const_iterator _Iter=_Mycomponents.begin(),
		_End=_Mycomponents.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Mycomponents.clear();
}

object::components_iter object::components_begin() {

_Shadow_status=RECREATE; //One or more component properties may change
return _Mycomponents.begin();
}

object::components_iter object::components_end() {

_Shadow_status=RECREATE; //One or more component properties may change
return _Mycomponents.end();
}

component *object::component_pointer(const std::string &_Name) {

	for (_Components::const_iterator _Iter=_Mycomponents.begin(),
		_End=_Mycomponents.end();_Iter!=_End;++_Iter) {
	
		//Component exist, return pointer
		if ((*_Iter)->_Name==_Name) {
		
		_Shadow_status=RECREATE; //Component properties may change
		return (*_Iter);
		}
	}

return 0;
}