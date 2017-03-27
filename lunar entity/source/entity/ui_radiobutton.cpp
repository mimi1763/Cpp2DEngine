/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_radiobutton.cpp
---------------------------------------------------
*/

#include "ui_radiobutton.h"

//Dependencies (entity)
#include "ui_menu.h"

//Protected

ui_radiobutton::ui_radiobutton(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_checkbox(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Tag(0) {
	//Empty
}

ui_radiobutton::ui_radiobutton(const ui_radiobutton &_Radiobutton):
	ui_checkbox(_Radiobutton) {
	copy(_Radiobutton);
}

ui_radiobutton::ui_radiobutton(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_radiobutton &_Radiobutton):ui_checkbox(
			_Owner,_Name,_Parent_node,_Draw_order,_Radiobutton) {
	copy(_Radiobutton);
}

ui_radiobutton::~ui_radiobutton() {
	//Empty
}

void ui_radiobutton::copy(const ui_radiobutton &_Radiobutton) {
	tag(_Radiobutton._Tag); //Critical functionality
}

//Events

void ui_radiobutton::release() {

	//Only allowed to check (not uncheck)
	if (!_Checked) {
	
	checked(true);
	change(); //Changed by user
	}

ui_element::release(); //Skip checkbox functionality
}

//States

void ui_radiobutton::uncheck(unsigned int _Tag) {

ui_radiobutton *_Radiobutton;

	//Uncheck all radiobuttons with equal tag
	for (ui_menu::_Elements::const_iterator _Iter=_Owner->_Myelements.begin(),
		_End=_Owner->_Myelements.end();_Iter!=_End;++_Iter) {
	
		if ((_Radiobutton=dynamic_cast<ui_radiobutton*>(*_Iter)) &&
			this!=_Radiobutton && _Radiobutton->_Tag==_Tag)
				_Radiobutton->ui_checkbox::checked(false);
	}
}

//Public

ui_radiobutton &ui_radiobutton::operator=(const ui_radiobutton &_Radiobutton) {

	//Worth doing
	if (this!=&_Radiobutton) {
	
	ui_checkbox::operator=(_Radiobutton); //Invoke base operator=
	copy(_Radiobutton);
	}

return *this;
}

void ui_radiobutton::checked(bool _Checked) {

	//Only allowed to check (not uncheck)
	if (!this->_Checked && _Checked) {
	
	uncheck(_Tag);
	ui_checkbox::checked(_Checked);
	}
}

void ui_radiobutton::tag(unsigned int _Tag) {

	if (this->_Tag!=_Tag) {
	
		if (_Checked) {
		
		ui_radiobutton *_Radiobutton;
		
			//Uncheck if tag is not unique
			for (ui_menu::_Elements::const_iterator _Iter=_Owner->_Myelements.begin(),
				_End=_Owner->_Myelements.end();_Iter!=_End;++_Iter) {
			
				if ((_Radiobutton=dynamic_cast<ui_radiobutton*>(*_Iter)) &&
					this!=_Radiobutton && _Radiobutton->_Tag==_Tag) {
				
				ui_checkbox::checked(false);
				break;
				}
			}
		}
	
	this->_Tag=_Tag;
	}
}

unsigned int ui_radiobutton::tag() const {
	return _Tag;
}