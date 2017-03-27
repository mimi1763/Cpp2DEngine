/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: component.cpp
---------------------------------------------------
*/

#include "component.h"

//Dependencies (engine)
#include "render_node.h"
#include "text_manager.h"
#include "texture_manager.h"
#include "animation_manager.h"
#include "particlesystem_manager.h"

//Dependencies (entity)
#include "entity_manager.h"

//Protected

component::component(object *_Owner,const std::string &_Name,
	bool _Visible):_Owner(_Owner),_Name(_Name),
		_Type(NONE),_Visible(_Visible),_Data(0) {
	//Empty
}

component::component(const component &_Component):
	_Owner(_Component._Owner),_Name(_Component._Name),
		_Type(NONE),_Data(0) {
	copy(_Component);
}

component::component(object *_Owner,const std::string &_Name,
	const component &_Component):_Owner(_Owner),_Name(_Name),
		_Type(NONE),_Data(0) {
	copy(_Component);
}

component::~component() {
	clear_data();
}

void component::copy(const component &_Component) {

	//Clear data or change type
	if (!_Component._Data || _Type!=_Component._Type)
		clear_data();
	
	//Initialize data
	if (_Component._Data) {
	
		switch (_Component._Type) {
		
			case TEXT:
			
				if (_Data) (*static_cast<text*>(_Data))=
					(*static_cast<text*>(_Component._Data));
				else _Data=_Owner->owner()->owner()->external().text_manager_ptr()->
					create_text(*static_cast<text*>(_Component._Data));
			
			break;
			
			case TEXTURE:
			
				if (_Data) (*static_cast<texture*>(_Data))=
					(*static_cast<texture*>(_Component._Data));
				else _Data=_Owner->owner()->owner()->external().texture_manager_ptr()->
					create_texture(*static_cast<texture*>(_Component._Data));
			
			break;
			
			case ANIMATION:
			
				if (_Data) (*static_cast<animation*>(_Data))=
					(*static_cast<animation*>(_Component._Data));
				else _Data=_Owner->owner()->owner()->external().animation_manager_ptr()->
					create_animation(*static_cast<animation*>(_Component._Data));
			
			break;
			
			case PARTICLE_SYSTEM:
			
				if (_Data) (*static_cast<particle_system*>(_Data))=
					(*static_cast<particle_system*>(_Component._Data));
				else _Data=_Owner->owner()->owner()->external().particlesystem_manager_ptr()->
					create_particlesystem(*static_cast<particle_system*>(_Component._Data));
			
			break;
		};
		
		//Attach data to object node
		if (_Type!=_Component._Type) attach();
	}

_Type=_Component._Type;
color_channel(_Component._Color);
visible(_Component._Visible);
}

void component::attach() {

	if (_Data) _Owner->node()->attach(_Data);
}

void component::detach() {

	if (_Data) _Owner->node()->detach(_Data);
}

//Public

component &component::operator=(const component &_Component) {

	//Worth doing
	if (this!=&_Component) copy(_Component);

return *this;
}

object *component::owner() const {
	return _Owner;
}

const std::string &component::name() const {
	return _Name;
}

component::TYPE component::type() const {
	return _Type;
}

void component::color_channel(const color &_Color,bool _Base) {

	if (_Base) this->_Color=_Color;
	
	//Nothing more to do
	if (_Type==NONE || _Type==PARTICLE_SYSTEM) return;
	
	switch (_Type) {
	
		case TEXT: {
		
		text *_Text=static_cast<text*>(_Data);
		
			if (_Base) _Text->color_value(_Color);
			//Use default color
			else if (_Color==color::ZERO)
				_Text->color_value(this->_Color);
			//Don't include opacity
			else {
			
			color _New_color=_Color;
			_New_color.a=_Text->color_value().a;
			_Text->color_value(_New_color);
			}
		
		break;
		}
		
		default: {
		
		renderable *_Renderable=static_cast<renderable*>(_Data);
		
			if (_Base) _Renderable->color_value(_Color);
			//Use base color
			else if (_Color==color::ZERO)
				_Renderable->color_value(this->_Color);
			//Don't include opacity
			else {
			
			color _New_color=_Color;
			_New_color.a=_Renderable->color_value().a;
			_Renderable->color_value(_New_color);
			}
		
		break;
		}
	};
}

const color &component::color_channel(bool _Base) const {

	if (_Base || _Type==NONE || _Type==PARTICLE_SYSTEM)
		return _Color;
	
	switch (_Type) {
	
		case TEXT:
		return static_cast<text*>(_Data)->color_value();
		
		default:
		return static_cast<renderable*>(_Data)->color_value();
	};
}

void component::opacity(float _Opacity,bool _Base) {

	if (_Base) _Color.a=_Opacity;
	
	//Nothing to do
	if (_Type==NONE || _Type==PARTICLE_SYSTEM) return;
	
	switch (_Type) {
	
		case TEXT: {
		
		text *_Text=static_cast<text*>(_Data);
		color _New_color=_Text->color_value();
		_New_color.a=(_Base?_Opacity:_Color.a-(1.0f-_Opacity));
		
			//Clamp
			if (_New_color.a<0.0f) _New_color.a=0.0f;
			else if (_New_color.a>1.0f) _New_color.a=1.0f;
		
		_Text->color_value(_New_color);
		break;
		}
		
		default: {
		
		renderable *_Renderable=static_cast<renderable*>(_Data);
		color _New_color=_Renderable->color_value();
		_New_color.a=(_Base?_Opacity:_Color.a-(1.0f-_Opacity));
		
			//Clamp
			if (_New_color.a<0.0f) _New_color.a=0.0f;
			else if (_New_color.a>1.0f) _New_color.a=1.0f;
		
		_Renderable->color_value(_New_color);
		break;
		}
	};
}

float component::opacity(bool _Base) const {
	return color_channel(_Base).a;
}

void component::visible(bool _Visible) {

this->_Visible=_Visible;

	if (_Data) _Data->visible(_Visible);
}

bool component::visible() const {
	return (_Data?_Data->visible():_Visible);
}

//Data

attachable *component::data(resource_data *_Resource_data) {

clear_data();

	//Nothing to do
	if (!_Resource_data) return 0;
	
	//Text
	if (dynamic_cast<font_data*>(_Resource_data)) {
	
	_Data=_Owner->owner()->owner()->external().text_manager_ptr()->
		create_text("",static_cast<font_data*>(_Resource_data),
			color::WHITE,_Visible);
	_Type=TEXT;
	}
	//Texture
	else if (dynamic_cast<texture_data*>(_Resource_data)) {
	
	_Data=_Owner->owner()->owner()->external().texture_manager_ptr()->
		create_texture(static_cast<texture_data*>(_Resource_data),
			_Visible);
	_Type=TEXTURE;
	}
	//Animation
	else if (dynamic_cast<animation_data*>(_Resource_data)) {
	
	_Data=_Owner->owner()->owner()->external().animation_manager_ptr()->
		create_animation(static_cast<animation_data*>(_Resource_data),
			true,_Visible);
	_Type=ANIMATION;
	}
	//Particle system
	else if (dynamic_cast<file_data*>(_Resource_data)) {
	
	_Data=_Owner->owner()->owner()->external().particlesystem_manager_ptr()->
		create_particlesystem(static_cast<file_data*>(_Resource_data),
			_Visible);
	_Type=PARTICLE_SYSTEM;
	}

color_channel(_Color);
attach(); //Attach data to object node
return _Data;
}

attachable *component::data() const {
	return _Data;
}

void component::clear_data() {

	//Nothing to do
	if (!_Data) return;

manageable *_Manageable=dynamic_cast<manageable*>(_Data);

	//Remove and detach data from object node
	if (_Manageable) _Manageable->owner()->remove(_Manageable);

_Type=NONE;
_Data=0;
}