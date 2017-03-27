/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_label.cpp
---------------------------------------------------
*/

#include "ui_label.h"

//Protected

ui_label::ui_label(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible) {

//Default label set up
_Caption_origin=TOP_LEFT;
}

ui_label::ui_label(const ui_label &_Label):ui_element(_Label) {
	copy(_Label);
}

ui_label::ui_label(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_label &_Label):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Label) {
	copy(_Label);
}

ui_label::~ui_label() {
	//Empty
}

void ui_label::copy(const ui_label &_Label) {
	//Empty - override default memcpy behaviour
}

//Public

ui_label &ui_label::operator=(const ui_label &_Label) {

	//Worth doing
	if (this!=&_Label) {
	
	ui_element::operator=(_Label); //Invoke base operator=
	copy(_Label);
	}

return *this;
}