/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: lighting_manager.cpp
---------------------------------------------------
*/

#include "lighting_manager.h"

//Dependencies (engine)
#include "framebuffer_manager.h"
#include "texture.h"

#include "render_node.h"
#include "render_window.h"
#include "entity_manager.h"

//Private

lighting_manager::lighting_manager(entity_manager *_Owner,render_node *_Parent_node,
	int _Overlay_layer,const interval &_Capture_layer):_Owner(_Owner),
		_Overlay_layer(_Overlay_layer),_Capture_layer(_Capture_layer),
			_Light_direction(vector2::NEGATIVE_UNIT_Y),_Overlay_color(color::ZERO),
				_Shadowcolor_day(color(0,0,0,0.25f)),_Shadowcolor_night(
					color(0,0,0,0.05f)),_Cast_shadows(true),_Lighting_changed(true) {

_Node=_Parent_node->create_childnode(_Owner->node()->
	draw_order()+_Overlay_layer,false);
init();
}

lighting_manager::lighting_manager(const lighting_manager &_Lighting_manager):
	_Owner(_Lighting_manager._Owner) {

_Node=_Lighting_manager._Node->parent_node()->create_childnode(
	_Lighting_manager._Node->draw_order(),_Lighting_manager._Node->visible());
_Lighting_changed=true; //Not copyable

copy(_Lighting_manager);
init();
}

lighting_manager::~lighting_manager() {

_Lightmap->owner()->remove(_Lightmap);
_Node->parent_node()->remove_childnode(_Node);
}

void lighting_manager::copy(const lighting_manager &_Lighting_manager) {

_Overlay_layer=_Lighting_manager._Overlay_layer;
_Capture_layer=_Lighting_manager._Capture_layer;

_Light_direction=_Lighting_manager._Light_direction;
_Overlay_color=_Lighting_manager._Overlay_color;
_Shadowcolor_day=_Lighting_manager._Shadowcolor_day;
_Shadowcolor_night=_Lighting_manager._Shadowcolor_night;
_Cast_shadows=_Lighting_manager._Cast_shadows;

_Node->visible(_Lighting_manager._Node->visible());
}

void lighting_manager::init() {

int _Draw_order=_Owner->node()->draw_order();

_Pre_capturenode=_Node->create_childnode(
	(-_Draw_order)+_Capture_layer.from(),false);
_Post_capturenode=_Node->create_childnode(
	(-_Draw_order)+_Capture_layer.to(),false);
_Pre_capturenode->onpredraw(&lighting_manager::pre_draw,this);
_Post_capturenode->onpostdraw(&lighting_manager::post_draw,this);

float _Width=render_window::instance()->ortho_width();
float _Height=render_window::instance()->ortho_height();
_Lightmap=_Owner->external().framebuffer_manager_ptr()->create_framebuffer(
	(unsigned int)_Width,(unsigned int)_Height);
_Lightmap->framebuffer_texture()->origin(rectangle::BOTTOM_LEFT);

_Lightmap_capture.size(_Width,_Height);
_Lightmap_capture.destination(_Lightmap);
_Lightmap_capture.technique(capture::COPY_TEX);
_Node->attach(_Lightmap->framebuffer_texture());
}

void lighting_manager::pre_draw(render_node *_Render_node) {

_Lightmap_capture.start();
render_manager::instance()->clear_screen(_Overlay_color);
}

void lighting_manager::post_draw(render_node *_Render_node) {
	_Lightmap_capture.stop();
}

//Public

lighting_manager &lighting_manager::operator=(const lighting_manager &_Lighting_manager) {

	//Worth doing
	if (this!=&_Lighting_manager) {
	
	copy(_Lighting_manager);
	_Lighting_changed=true; //Properties has changed, notify externals
	}

return *this;
}

entity_manager *lighting_manager::owner() const {
	return _Owner;
}

//Light/shadow

void lighting_manager::light_direction(const vector2 &_Light_direction) {

	if (this->_Light_direction!=_Light_direction) {
	
	this->_Light_direction=_Light_direction;
	_Lighting_changed=true; //Light direction has changed, notify externals
	}
}

const vector2 &lighting_manager::light_direction() const {
	return _Light_direction;
}

void lighting_manager::overlay_visible(bool _Visible) {
	_Node->visible(_Visible);
}

bool lighting_manager::overlay_visible() const {
	return _Node->visible();
}

void lighting_manager::overlay_color(const color &_Color) {

	if (_Overlay_color!=_Color) {
	
		//Overlay opacity has changed, notify externals
		if (_Overlay_color.a!=_Color.a) _Lighting_changed=true;
	
	_Overlay_color=_Color;
	}
}

const color &lighting_manager::overlay_color() const {
	return _Overlay_color;
}

void lighting_manager::overlay_opacity(float _Opacity) {

	if (_Overlay_color.a!=_Opacity) {
	
	_Overlay_color.a=_Opacity;
	_Lighting_changed=true; //Overlay opacity has changed, notify externals
	}
}

float lighting_manager::overlay_opacity() const {
	return _Overlay_color.a;
}

void lighting_manager::shadow_color(const color &_Color_day,const color &_Color_night) {

	if (_Shadowcolor_day!=_Color_day ||
		_Shadowcolor_night!=_Color_night) {
	
	_Shadowcolor_day=_Color_day;
	_Shadowcolor_night=_Color_night;
	_Lighting_changed=true; //Shadow color has changed, notify externals
	}
}

const color &lighting_manager::shadowcolor_day() const {
	return _Shadowcolor_day;
}

const color &lighting_manager::shadowcolor_night() const {
	return _Shadowcolor_night;
}

void lighting_manager::cast_shadows(bool _Cast_shadows) {

	if (this->_Cast_shadows!=_Cast_shadows) {
	
	this->_Cast_shadows=_Cast_shadows;
	_Lighting_changed=true; //Cast shadows has changed, notify externals
	}
}

bool lighting_manager::cast_shadows() const {
	return _Cast_shadows;
}

bool lighting_manager::lighting_changed() const {
	return _Lighting_changed;
}

//Light map

void lighting_manager::overlay_layer(int _Overlay_layer) {

	if (_Overlay_layer>0 &&
		this->_Overlay_layer!=_Overlay_layer) {
	
	_Node->draw_order(_Owner->node()->draw_order()+_Overlay_layer);
	this->_Overlay_layer=_Overlay_layer;
	}
}

int lighting_manager::overlay_layer() const {
	return _Overlay_layer;
}

void lighting_manager::capture_layer(const interval &_Capture_layer) {

int _From=_Capture_layer.from();
int _To=_Capture_layer.to();

	if (_To<0 && _To-_From>1) {
	
	int _Draw_order=_Owner->node()->draw_order();
	
		if (this->_Capture_layer.from()!=_From)
			_Pre_capturenode->draw_order((-_Draw_order)+_From);
		
		if (this->_Capture_layer.to()!=_To)
			_Post_capturenode->draw_order((-_Draw_order)+_To);
	
	this->_Capture_layer=_Capture_layer;
	}
}

interval lighting_manager::capture_layer() const {
	return _Capture_layer;
}

int lighting_manager::overlay_draworder() const {
	return _Node->draw_order();
}

interval lighting_manager::capture_draworder() const {
	return interval(_Pre_capturenode->draw_order(),
		_Post_capturenode->draw_order());
}