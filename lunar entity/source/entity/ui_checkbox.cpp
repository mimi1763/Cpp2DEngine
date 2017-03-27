/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_checkbox.cpp
---------------------------------------------------
*/

#include "ui_checkbox.h"

//Public

const std::string ui_checkbox::additional::CHECK="CHECK";

//Protected

ui_checkbox::ui_checkbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Checked(false) {

//Default checkbox set up
_Focusable=true;

	if (_Caption_width<400) _Caption_width=400;

_Caption_origin=OUTSIDE_RIGHT;
}

ui_checkbox::ui_checkbox(const ui_checkbox &_Checkbox):
	ui_element(_Checkbox) {
	copy(_Checkbox);
}

ui_checkbox::ui_checkbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_checkbox &_Checkbox):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Checkbox) {
	copy(_Checkbox);
}

ui_checkbox::~ui_checkbox() {
	//Empty
}

void ui_checkbox::copy(const ui_checkbox &_Checkbox) {
	checked(_Checkbox._Checked); //Critical functionality
}

//Events

void ui_checkbox::release() {

checked(!_Checked);
change(); //Changed by user
ui_element::release(); //Use base functionality
}

//States

void ui_checkbox::show_state(const STATE &_State) {

object *_Check=state_object(additional::CHECK,_State);

	//Show current state
	if (_Check) visible(_Check,true,true);
}

void ui_checkbox::hide_state(const STATE &_State) {

object *_Check=state_object(additional::CHECK,_State);

	//Hide current state
	if (_Check) visible(_Check,false,true);
}

void ui_checkbox::change_state(const STATE &_State) {

	if (_Checked) {
	
	hide_state(this->_State); //Hide previous state
	show_state(_State); //Show new state
	}

ui_element::change_state(_State);
}

//Public

ui_checkbox &ui_checkbox::operator=(const ui_checkbox &_Checkbox) {

	//Worth doing
	if (this!=&_Checkbox) {
	
	ui_element::operator=(_Checkbox); //Invoke base operator=
	copy(_Checkbox);
	}

return *this;
}

bool ui_checkbox::functional() const {
	return (_Focusable || ui_element::functional());
}

void ui_checkbox::checked(bool _Checked) {

	if (this->_Checked!=_Checked) {
	
	this->_Checked=_Checked;
	
		//Show current state
		if (_Checked) show_state(_State);
		//Hide current state
		else hide_state(_State);
	}
}

bool ui_checkbox::checked() const {
	return _Checked;
}