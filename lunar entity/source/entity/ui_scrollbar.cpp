/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_scrollbar.cpp
---------------------------------------------------
*/

#include "ui_scrollbar.h"

//Dependencies
#include <cmath>

//Dependencies (engine)
#include "input_manager.h"
#include "render_node.h"
#include "texturable.h"

#include "generic_utility.h"
#include "math_utility.h"

//Dependencies (entity)
#include "ui_scrollable.h"

//Public

//Additional
//Public

const std::string ui_scrollbar::additional::BAR="BAR";

//Scrollbar
//Protected

ui_scrollbar::ui_scrollbar(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Type(VERTICAL),_Start(0),
					_Stop(0),_Position(0),_Min_barsize(15.0f),
						_Max_barsize(0.0f),_Update_bar(true),
							_Drag_bar(false),_Scrollable(0) {

//Default scrollbar set up
_Focusable=true;
}

ui_scrollbar::ui_scrollbar(const ui_scrollbar &_Scrollbar):
	ui_element(_Scrollbar) {

_Update_bar=true; //Not copyable
_Drag_bar=false; //Not copyable
_Scrollable=0; //Not copyable

copy(_Scrollbar);
}

ui_scrollbar::ui_scrollbar(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_scrollbar &_Scrollbar):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Scrollbar) {

_Update_bar=true; //Not copyable
_Drag_bar=false; //Not copyable
_Scrollable=0; //Not copyable

copy(_Scrollbar);
}

ui_scrollbar::~ui_scrollbar() {
	detach_scrollable();
}

void ui_scrollbar::copy(const ui_scrollbar &_Scrollbar) {

_Type=_Scrollbar._Type;
_Start=_Scrollbar._Start;
_Stop=_Scrollbar._Stop;
_Position=_Scrollbar._Position;

_Min_barsize=_Scrollbar._Min_barsize;
_Max_barsize=_Scrollbar._Max_barsize;
}

//States

void ui_scrollbar::change_state(const STATE &_State) {

object *_Bar=state_object(additional::BAR,this->_State);

	//Hide previous state
	if (_Bar) visible(_Bar,false,true);

_Bar=state_object(additional::BAR,_State);

	//Show new state
	if (_Bar) visible(_Bar,true,true);

ui_element::change_state(_State);
update_bar(); //Force update immediately
}

//Bar

texturable *ui_scrollbar::bar_component(object *_Object) const {

	//Nothing to do
	if (!_Object) return 0;

texturable *_Texturable=0;
attachable *_Data;

	//Find first texturable component in object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Texturable=dynamic_cast<texturable*>(_Data)))
			break;
	}

return _Texturable;
}

void ui_scrollbar::update_bar() {

object *_Object=state_object(additional::BAR,_State);

	//No bar object found
	if (!_Object) return;

_Object->visible(_Stop!=_Start);

	//Nothing more to do
	if (_Stop==_Start) return;

texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not found
	if (!_Manageable) return;

_Manageable->update();

	//Manageable update failed
	if (_Manageable->status()!=manageable::OKAY)
		return;

unsigned int _Count=(_Scrollable?_Scrollable->scrollbar_viewcount():1);
float _Size=(float)_Count/(_Stop-_Start+_Count)*(_Type==HORIZONTAL?
	_Texturable->texture_width():_Texturable->texture_height());

	//Make sure bar size is not smaller than min bar size
	if (_Size<_Min_barsize) _Size=_Min_barsize;
	//Make sure bar size is not larger than max bar size
	else if (_Max_barsize>=_Min_barsize && _Size>_Max_barsize)
		_Size=_Max_barsize;
	
	if (_Type==HORIZONTAL && _Size>_Texturable->texture_width())
		_Size=(float)_Texturable->texture_width();
	else if (_Type==VERTICAL && _Size>_Texturable->texture_height())
		_Size=(float)_Texturable->texture_height();

_Texturable->size(0.0f,0.0f);
_Texturable->auto_size(false);
_Texturable->auto_crop(false);

_Texturable->cut_mode(_Type==HORIZONTAL?texturable::CUT_TOP_RIGHT:
	texturable::CUT_BOTTOM_LEFT);
_Texturable->crop_size(_Type==HORIZONTAL?_Size:_Texturable->texture_width(),
	_Type==VERTICAL?_Size:_Texturable->texture_height());

_Texturable->crop_position(vector2(_Type==HORIZONTAL?(float)
	_Texturable->texture_width()*0.5f-_Texturable->crop_width()*0.5f:0.0f,
		_Type==VERTICAL?(float)_Texturable->texture_height()*0.5f-
			_Texturable->crop_height()*0.5f:0.0f));

_Texturable->size(_Texturable->crop_width(),_Texturable->crop_height());

//Position

float _Percent=(_Start!=_Stop?(float)
	(this->_Position-_Start)/(_Stop-_Start):1.0f)-0.5f;

vector2 _Position=vector2(_Type==HORIZONTAL?(_Area->width()-
	_Texturable->width())*_Percent:0.0f,_Type==VERTICAL?
		-(_Area->height()-_Texturable->height())*_Percent:0.0f);

_Object->node()->position(utility::position(_Position,_Texturable->width(),
	_Texturable->height(),_Texturable->origin())+utility::position(
		vector2::ZERO,_Area->width(),_Area->height(),rectangle::CENTER,
			_Area->origin()));
}

//Public

ui_scrollbar &ui_scrollbar::operator=(const ui_scrollbar &_Scrollbar) {

	//Worth doing
	if (this!=&_Scrollbar) {
	
	ui_element::operator=(_Scrollbar); //Invoke base operator=
	copy(_Scrollbar);
	
	_Update_bar=true;
	}

return *this;
}

void ui_scrollbar::update(float _Time) {

	if (_Update_bar && visible()) {
	
	update_bar();
	_Update_bar=false;
	}

ui_element::update(_Time); //Use base functionality
}

bool ui_scrollbar::key_released(unsigned int _Key) {

	//Nothing to do
	if (!_Focused || _Start==_Stop) return false;

unsigned int _Adjusted_key=_Key;

	//Adjust key if horizontal
	if (_Type==HORIZONTAL) {
	
		if (_Key==VK_UP)
			_Adjusted_key=VK_DOWN;
		else if (_Key==VK_DOWN)
			_Adjusted_key=VK_UP;
	}
	
	switch (_Adjusted_key) {
	
		case VK_UP:
		case VK_LEFT: {
		
			if (_Position>_Start) {
			
			position(_Position-1);
			change(); //Changed by user
			}
		
		return true;
		}
		
		case VK_DOWN:
		case VK_RIGHT: {
		
			if (_Position<_Stop) {
			
			position(_Position+1);
			change(); //Changed by user
			}
		
		return true;
		}
	}

return false;
}

void ui_scrollbar::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Start==_Stop || _Drag_bar) return;

object *_Object=state_object(additional::BAR,_State);
texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not updated
	if (!_Manageable || _Manageable->status()!=manageable::OKAY)
		return;

_Drag_bar=_Texturable->world_orientedboundingbox().
	intersects(vector2((float)_X,(float)_Y));
}

void ui_scrollbar::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Start==_Stop) return;
	
	//Cancel drag
	if (_Drag_bar) {
	
	_Drag_bar=false;
	return;
	}

object *_Object=state_object(additional::BAR,_State);
texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not updated
	if (!_Manageable || _Manageable->status()!=manageable::OKAY)
		return;

float _Percent=0.0f;

	if (_Type==HORIZONTAL) {
	
	float _Pos=utility::position(vector2((float)_X,0.0f),
		_Area->width(),_Area->height(),_Area->origin()).x-
			(_Object->node()->world_position().x-
				_Object->node()->position().x);
	
		if (_Pos>=_Object->node()->position().x)
			_Percent=_Object->node()->position().x+_Texturable->width();
		else _Percent=_Object->node()->position().x-_Texturable->width();
	
	float _Width=_Area->width()-_Texturable->width();
	_Percent=(_Percent+_Width*0.5f)/_Width;
	}
	else {
	
	float _Pos=utility::position(vector2(0.0f,(float)_Y),
		_Area->width(),_Area->height(),_Area->origin()).y-
			(_Object->node()->world_position().y-
				_Object->node()->position().y);
	
		if (_Pos<=_Object->node()->position().y)
			_Percent=_Object->node()->position().y-_Texturable->height();
		else _Percent=_Object->node()->position().y+_Texturable->height();
	
	float _Height=_Area->height()-_Texturable->height();
	_Percent=1.0f-((_Percent+_Height*0.5f)/_Height);
	}

unsigned int _Prev_position=_Position;
percent(_Percent);

	//Changed by user
	if (_Position!=_Prev_position) change();
}

void ui_scrollbar::mouse_moved(unsigned int _X,
	unsigned int _Y) {

	//Nothing to do
	if (_Start==_Stop || !_Drag_bar) return;
	
	//Cancel drag
	if (!input_manager::instance()->mouse_active(0) &&
		!input_manager::instance()->mouse_active(1)) {
	
	_Drag_bar=false;
	return;
	}

object *_Object=state_object(additional::BAR,_State);
texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not updated
	if (!_Manageable || _Manageable->status()!=manageable::OKAY)
		return;

float _Percent=0.0f;

	if (_Type==HORIZONTAL) {
	
	_Percent=utility::position(vector2((float)_X,0.0f),
		_Area->width(),_Area->height(),_Area->origin()).x-
			(_Object->node()->world_position().x-
				_Object->node()->position().x);
	
	float _Width=_Area->width()-_Texturable->width();
	_Percent=(_Percent+_Width*0.5f)/_Width;
	}
	else {
	
	_Percent=utility::position(vector2(0.0f,(float)_Y),
		_Area->width(),_Area->height(),_Area->origin()).y-
			(_Object->node()->world_position().y-
				_Object->node()->position().y);
	
	float _Height=_Area->height()-_Texturable->height();
	_Percent=1.0f-((_Percent+_Height*0.5f)/_Height);
	}

unsigned int _Prev_position=_Position;
percent(_Percent);

	//Changed by user
	if (_Position!=_Prev_position) change();
}

void ui_scrollbar::mouse_scrolled(int _Delta,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (!_Focused || _Start==_Stop) return;
	
	//Up/down
	if (_Delta!=0) key_released(_Delta>0?VK_UP:VK_DOWN);
}

bool ui_scrollbar::functional() const {
	return (_Start!=_Stop || ui_element::functional());
}

void ui_scrollbar::type(const TYPE &_Type) {

	if (this->_Type!=_Type) {
	
	this->_Type=_Type;
	_Update_bar=true;
	}
}

ui_scrollbar::TYPE ui_scrollbar::type() const {
	return _Type;
}

void ui_scrollbar::range(unsigned int _Start,unsigned int _Stop) {

	//Not allowed for user to change range when attached to a scrollable
	if (_Scrollable) return;

unsigned int _New_start=(_Start>_Stop?_Stop:_Start);
unsigned int _New_stop=(_Stop<_Start?_Start:_Stop);

	if (this->_Start!=_New_start || this->_Stop!=_New_stop) {
	
	this->_Start=_New_start;
	this->_Stop=_New_stop;
	
		if (_Position<_New_start) _Position=_New_start;
		else if (_Position>_New_stop) _Position=_New_stop;
	
	_Update_bar=true;
	}
}

unsigned int ui_scrollbar::start() const {
	return _Start;
}

unsigned int ui_scrollbar::stop() const {
	return _Stop;
}

void ui_scrollbar::position(unsigned int _Position) {

unsigned int _New_position=_Position;

	if (_Position<_Start) _New_position=_Start;
	else if (_Position>_Stop) _New_position=_Stop;
	
	if (this->_Position!=_New_position) {
	
		//Update scrollable
		if (_Scrollable) {
		
		bool _Up=(_New_position<this->_Position);
		_Scrollable->scroll(_Up?this->_Position-_New_position:
			_New_position-this->_Position,_Up);
		}
	
	this->_Position=_New_position;
	_Update_bar=true;
	}
}

unsigned int ui_scrollbar::position() const {
	return _Position;
}

void ui_scrollbar::percent(float _Percent) {

float _New_percent=_Percent;

	if (_Percent<0.0f) _New_percent=0.0f;
	else if (_Percent>1.0f) _New_percent=1.0f;
	
	if (_New_percent!=percent())
		position((unsigned int)math::round(math::lerp(
			(float)_Start,(float)_Stop,_New_percent)));
}

float ui_scrollbar::percent() const {
	return (_Start!=_Stop?(float)(_Position-_Start)/(_Stop-_Start):1.0f);
}

void ui_scrollbar::bar_size(float _Min_size,float _Max_size) {

float _New_minsize=(_Min_size<0.0f?0.0f:_Min_size);
float _New_maxsize=(_Max_size<0.0f?0.0f:_Max_size);

	if (_Min_barsize!=_New_minsize || _Max_barsize!=_New_maxsize) {
	
	_Min_barsize=_New_minsize;
	_Max_barsize=_New_maxsize;
	_Update_bar=true;
	}
}

float ui_scrollbar::min_barsize() const {
	return _Min_barsize;
}

float ui_scrollbar::max_barsize() const {
	return _Max_barsize;
}

//Attach/detach

void ui_scrollbar::attach_scrollable(ui_scrollable *_Scrollable) {

	if (this->_Scrollable!=_Scrollable) {
	
	detach_scrollable();
	this->_Scrollable=_Scrollable; //Attach
	
		if (_Scrollable) {
		
		_Scrollable->attach_scrollbar(this);
		focusable(false);
		}
	
	_Update_bar=true;
	}
}

void ui_scrollbar::detach_scrollable() {

	//Nothing to detach
	if (!_Scrollable) return;

ui_scrollable *_Temp_scrollable=_Scrollable;
_Scrollable=0; //Detach
_Temp_scrollable->detach_scrollbar();

focusable(true);
_Update_bar=true;
}

ui_scrollable *ui_scrollbar::scrollable_pointer() const {
	return _Scrollable;
}