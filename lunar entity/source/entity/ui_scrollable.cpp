/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_scrollable.cpp
---------------------------------------------------
*/

#include "ui_scrollable.h"

//Dependencies (entity)
#include "ui_scrollbar.h"

//Protected

ui_scrollable::ui_scrollable(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Scroll_rate(3),
					_Scrolled(false),_Scrollbar(0) {

//Default scrollable set up
_Focusable=true;
}

ui_scrollable::ui_scrollable(const ui_scrollable &_Scrollable):
	ui_element(_Scrollable) {

_Scrolled=false; //Not copyable
_Scrollbar=0; //Not copyable

copy(_Scrollable);
}

ui_scrollable::ui_scrollable(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_scrollable &_Scrollable):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Scrollable) {

_Scrolled=false; //Not copyable
_Scrollbar=0; //Not copyable

copy(_Scrollable);
}

ui_scrollable::~ui_scrollable() {
	detach_scrollbar();
}

void ui_scrollable::copy(const ui_scrollable &_Scrollable) {
	_Scroll_rate=_Scrollable._Scroll_rate;
}

unsigned int ui_scrollable::scrollbar_count() const {
	return 0; //Empty - override if needed
}

unsigned int ui_scrollable::scrollbar_viewcount() const {
	return 0; //Empty - override if needed
}

unsigned int ui_scrollable::scrollbar_position() const {
	return 0; //Empty - override if needed
}

void ui_scrollable::update_scrollbar() const {

	if (_Scrollbar) {
	
	unsigned int _Count=scrollbar_count();
	unsigned int _View_count=scrollbar_viewcount();
	unsigned int _Position=scrollbar_position();
	
	unsigned int _Stop=(_View_count>0 && _Count>_View_count?
		_Count-_View_count:0);
	
		if (_Position>_Stop) _Position=_Stop;
		
		if (_Scrollbar->_Start!=0 || _Scrollbar->_Stop!=_Stop ||
			_Scrollbar->_Position!=_Position) {
		
		_Scrollbar->_Start=0;
		_Scrollbar->_Stop=_Stop;
		_Scrollbar->_Position=_Position;
		_Scrollbar->_Update_bar=true;
		}
	}
}

//Public

ui_scrollable &ui_scrollable::operator=(const ui_scrollable &_Scrollable) {

	//Worth doing
	if (this!=&_Scrollable) {
	
	ui_element::operator=(_Scrollable); //Invoke base operator=
	copy(_Scrollable);
	}

return *this;
}

bool ui_scrollable::key_pressed(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;
	
	switch (_Key) {
	
		//Page up/down
		case VK_PRIOR:
		case VK_NEXT:
		scroll(0,_Key==VK_PRIOR);
		return true;
	}

return false;
}

bool ui_scrollable::key_released(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;
	
	switch (_Key) {
	
		//Page up/down
		case VK_PRIOR:
		case VK_NEXT:
		return true;
	}

return false;
}

void ui_scrollable::mouse_scrolled(int _Delta,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (!_Focused) return;
	
	//Up/down
	if (_Delta!=0) scroll(_Scroll_rate,_Delta>0);
}

void ui_scrollable::scroll_rate(unsigned int _Scroll_rate) {
	this->_Scroll_rate=_Scroll_rate;
}

unsigned int ui_scrollable::scroll_rate() const {
	return _Scroll_rate;
}

void ui_scrollable::scroll_top() {
	scroll(0);
}

void ui_scrollable::scroll_up() {
	scroll(1);
}

void ui_scrollable::scroll_down() {
	scroll(1,false);
}

void ui_scrollable::scroll_bottom() {
	scroll(0,false);
}

//Attach/detach

void ui_scrollable::attach_scrollbar(ui_scrollbar *_Scrollbar) {

	if (this->_Scrollbar!=_Scrollbar) {
	
	detach_scrollbar();
	this->_Scrollbar=_Scrollbar; //Attach
	
		if (_Scrollbar) {
		
		_Scrollbar->attach_scrollable(this);
		update_scrollbar();
		}
	}
}

void ui_scrollable::detach_scrollbar() {

	//Nothing to detach
	if (!_Scrollbar) return;

ui_scrollbar *_Temp_scrollbar=_Scrollbar;
_Scrollbar=0; //Detach
_Temp_scrollbar->detach_scrollable();
}

ui_scrollbar *ui_scrollable::scrollbar_pointer() const {
	return _Scrollbar;
}