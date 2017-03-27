/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_button.cpp
---------------------------------------------------
*/

#include "ui_button.h"

//Dependencies (entity)
#include "ui_manager.h"

//Protected

ui_button::ui_button(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible) {

//Default button set up
_Focusable=true;
}

ui_button::ui_button(const ui_button &_Button):ui_element(_Button) {
	copy(_Button);
}

ui_button::ui_button(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_button &_Button):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Button) {
	copy(_Button);
}

ui_button::~ui_button() {
	//Empty
}

void ui_button::copy(const ui_button &_Button) {
	_Myprocedures=_Button._Myprocedures;
}

//Events

void ui_button::release() {

	for (size_t i=0;i<_Myprocedures.size();++i) {
	
	const _Procedure &_Myprocedure=_Myprocedures[i];
	ui_menu *_Menu=_Owner->owner()->menu_pointer(_Myprocedure.second);
	
		//Nothing to do
		if (!_Menu) continue;
		
		switch (_Myprocedure.first) {
		
			case SHOW_MENU:
			_Menu->show(false);
			break;
			
			case SHOW_MODAL_MENU:
			_Menu->show();
			break;
			
			case CLOSE_MENU:
			_Menu->close();
			break;
			
			case ENABLE_MENU:
			_Menu->enabled(true);
			break;
			
			case DISABLE_MENU:
			_Menu->enabled(false);
			break;
		};
	}

ui_element::release(); //Use base functionality
}

//Public

ui_button &ui_button::operator=(const ui_button &_Button) {

	//Worth doing
	if (this!=&_Button) {
	
	ui_element::operator=(_Button); //Invoke base operator=
	copy(_Button);
	}

return *this;
}

bool ui_button::functional() const {
	return (!_Myprocedures.empty() || ui_element::functional());
}

//Procedure

void ui_button::add_procedure(const ACTION &_Action,
	const std::string &_Argument) {
	_Myprocedures.push_back(_Procedure(_Action,_Argument));
}

void ui_button::change_procedure(unsigned int _Index,
	const ACTION &_Action,const std::string &_Argument) {

	if (_Index<_Myprocedures.size()) {
	
	_Myprocedures[_Index].first=_Action;
	_Myprocedures[_Index].second=_Argument;
	}
}

void ui_button::remove_procedure(unsigned int _Index) {

	if (_Index<_Myprocedures.size())
		_Myprocedures.erase(_Myprocedures.begin()+_Index);
}

void ui_button::clear_procedure() {
	_Myprocedures.clear();
}