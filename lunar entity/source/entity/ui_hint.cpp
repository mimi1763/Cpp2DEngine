/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_hint.cpp
---------------------------------------------------
*/

#include "ui_hint.h"

//Dependencies (engine)
#include "render_window.h"
#include "input_manager.h"
#include "render_node.h"
#include "text.h"
#include "font.h"

#include "generic_utility.h"

//Protected

ui_hint::ui_hint(ui_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,float _Width,float _Height):
		ui_entity(_Owner,_Parent_node,_Draw_order,_Width,_Height,false),
			_Type(DYNAMIC_SIZE),_Follow(false),_Fadein_delay(0.5f),
				_Fadeout_delay(0.5f),_Fadein_time(0.1f),_Fadeout_time(0.1f),
					_Hold_time(5.0f),_Phase(FADE_IN),_Phase_time(0.0f),
						_Update_position(true) {
	//Empty
}

ui_hint::ui_hint(const ui_hint &_Hint):ui_entity(_Hint) {
	copy(_Hint);
}

ui_hint::ui_hint(ui_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,const ui_hint &_Hint):
		ui_entity(_Owner,_Parent_node,_Draw_order,_Hint) {
	copy(_Hint);
}

ui_hint::~ui_hint() {
	//Empty
}

void ui_hint::copy(const ui_hint &_Hint) {

_Type=_Hint._Type;
_Follow=_Hint._Follow;

_Fadein_delay=_Hint._Fadein_delay;
_Fadeout_delay=_Hint._Fadeout_delay;
_Fadein_time=_Hint._Fadein_time;
_Fadeout_time=_Hint._Fadeout_time;
_Hold_time=_Hint._Hold_time;

_Border_size=_Hint._Border_size;
_Offset_position=_Hint._Offset_position;

_Phase=_Hint._Phase;
_Phase_time=_Hint._Phase_time;
_Update_position=_Hint._Update_position;
}

void ui_hint::update_size() {

object *_Object=object_pointer(additional::CAPTION);

	if (!_Object) return;

text *_Text=0;
attachable *_Data;

	//Find first text component in caption object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Text=dynamic_cast<text*>(_Data)))
			break;
	}
	
	//Caption text not found
	if (!_Text) return;

_Text->text_font()->update(); //Update font

	//Font update failed
	if (_Text->text_font()->status()!=manageable::OKAY)
		return;

_Text->text_data(_Caption);
_Text->wrap_width(0);
_Text->max_lines(0);
_Text->update();

	//Text update failed
	if (_Text->status()!=manageable::OKAY)
		return;

//Set hint size based on caption size
size((float)_Text->width()+_Border_size.x*2.0f,
	(float)_Text->height()+_Border_size.y*2.0f);
}

void ui_hint::update_position() {

//Render window ortho size
float _Ortho_width=render_window::instance()->ortho_width();
float _Ortho_height=render_window::instance()->ortho_height();

//Input manager mouse position
unsigned int _X=input_manager::instance()->mouse_x();
unsigned int _Y=input_manager::instance()->mouse_y();

//Hint size
float _Width=width();
float _Height=height();

//Convert area to aabb based on mouse position
vector2 _Position(vector2((float)_X,(float)_Y)+_Offset_position);
aabb _Aabb(utility::position(_Position,_Width,_Height,
	rectangle::BOTTOM_LEFT,_Area->origin()),
		utility::position(_Position,_Width,_Height,
			rectangle::TOP_RIGHT,_Area->origin()));

	//Outside right edge, adjust left
	if (_Aabb.maximum().x>_Ortho_width)
		_Position.x-=_Aabb.maximum().x-_Ortho_width;
	
	//Outside left edge, adjust right
	if (_Aabb.minimum().x<0.0f)
		_Position.x+=-_Aabb.minimum().x;
	
	//Outside bottom edge, adjust up
	if (_Aabb.minimum().y<0.0f)
		_Position.y+=-_Aabb.minimum().y;
	
	//Outside top edge, adjust down
	if (_Aabb.maximum().y>_Ortho_height)
		_Position.y-=_Aabb.maximum().y-_Ortho_height;

//Set hint position based on mouse position and ortho size
_Node->position(_Position-_Node->parent_node()->world_position());
}

//Caption

void ui_hint::update_caption() {

	//Set hint size based on caption size
	if (_Type==DYNAMIC_SIZE) update_size();

ui_entity::update_caption(); //Use base functionality
}

//Public

ui_hint &ui_hint::operator=(const ui_hint &_Hint) {

	//Worth doing
	if (this!=&_Hint) {
	
	ui_entity::operator=(_Hint); //Invoke base operator=
	copy(_Hint);
	}

return *this;
}

void ui_hint::update(float _Time) {

ui_entity::update(_Time); //Use base functionality

	if (visible()) {
	
	_Phase_time+=_Time;
	
		//Update position
		if (_Phase==HOLD || (_Phase==FADE_IN &&
			_Phase_time>_Fadein_delay)) {
		
			if (_Update_position) {
			
			update_position();
			
				//Stop updating hint position
				if (!_Follow) _Update_position=false;
			}
		}
		
		//Hold hint
		if (_Phase==HOLD) {
		
			if (_Hold_time>0.0f && _Phase_time>=_Hold_time) {
			
			_Phase=FADE_OUT;
			_Phase_time=0.0f;
			}
		}
		else {
		
		float _Opacity=opacity();
		
			//Fade in hint
			if (_Phase==FADE_IN) {
			
				if (_Phase_time>_Fadein_delay) {
				
					if ((_Opacity+=_Time/_Fadein_time)>=1.0f) {
					
					_Opacity=1.0f;
					_Phase=HOLD;
					_Phase_time=0.0f;
					}
				}
			}
			//Fade out hint
			else if (_Phase==FADE_OUT) {
			
				if (_Phase_time>_Fadeout_delay) {
				
					if ((_Opacity-=_Time/_Fadeout_time)<=0.0f) {
					
					_Opacity=0.0f;
					_Phase=FADE_IN;
					_Phase_time=0.0f;
					visible(false);
					}
				}
			}
		
		//Update opacity for hint
		opacity(_Opacity);
		}
	}
}

void ui_hint::show(const std::string &_Caption) {

	//Make sure opacity is zeroed
	if (!visible()) opacity(0.0f);

_Phase=FADE_IN;
_Phase_time=0.0f;
_Update_position=true;

float _Opacity=opacity();

	//Skip fade in delay
	if (_Opacity>0.0f)
		_Phase_time=_Fadein_delay+(_Fadein_time*_Opacity);

caption(_Caption);
visible(true);
}

void ui_hint::hide(bool _Immediately) {

	if (!visible()) return;

_Phase=FADE_OUT;
_Phase_time=0.0f;

	if (_Immediately) {
	
	_Phase=FADE_IN;
	opacity(0.0f);
	visible(false);
	return;
	}

float _Opacity=opacity();

	//Skip fade out delay
	if (_Opacity<1.0f)
		_Phase_time=_Fadeout_delay+(_Fadeout_time*(1.0f-_Opacity));
}

void ui_hint::type(const TYPE &_Type) {

	if (this->_Type!=_Type) {
	
		//Update caption
		if (_Type==DYNAMIC_SIZE) _Update_caption=true;
	
	this->_Type=_Type;
	}
}

ui_hint::TYPE ui_hint::type() const {
	return _Type;
}

void ui_hint::follow(bool _Follow) {

	if (this->_Follow!=_Follow) {
	
		if (_Follow) _Update_position=true;
	
	this->_Follow=_Follow;
	}
}

bool ui_hint::follow() const {
	return _Follow;
}

void ui_hint::fadein_delay(float _Fadein_delay) {
	this->_Fadein_delay=_Fadein_delay;
}

float ui_hint::fadein_delay() const {
	return _Fadein_delay;
}

void ui_hint::fadeout_delay(float _Fadeout_delay) {
	this->_Fadeout_delay=_Fadeout_delay;
}

float ui_hint::fadeout_delay() const {
	return _Fadeout_delay;
}

void ui_hint::fadein_time(float _Fadein_time) {
	this->_Fadein_time=_Fadein_time;
}

float ui_hint::fadein_time() const {
	return _Fadein_time;
}

void ui_hint::fadeout_time(float _Fadeout_time) {
	this->_Fadeout_time=_Fadeout_time;
}

float ui_hint::fadeout_time() const {
	return _Fadeout_time;
}

void ui_hint::hold_time(float _Hold_time) {
	this->_Hold_time=_Hold_time;
}

float ui_hint::hold_time() const {
	return _Hold_time;
}

void ui_hint::border_size(const vector2 &_Border_size) {

	if (this->_Border_size!=_Border_size) {
	
		//Update size
		if (_Type==DYNAMIC_SIZE) _Update_caption=true;
	
	this->_Border_size=_Border_size;
	}
}

vector2 ui_hint::border_size() const {
	return _Border_size;
}

void ui_hint::offset_position(const vector2 &_Offset_position) {

	if (this->_Offset_position!=_Offset_position) {
	
	this->_Offset_position=_Offset_position;
	_Update_position=true; //Update position
	}
}

vector2 ui_hint::offset_position() const {
	return _Offset_position;
}