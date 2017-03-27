/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_groupbox.cpp
---------------------------------------------------
*/

#include "ui_groupbox.h"

//Dependencies (engine)
#include "render_node.h"

//Dependencies (entity)
#include "ui_menu.h"

//Protected

ui_groupbox::ui_groupbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_element(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible) {

//Default groupbox set up
_Caption_origin=TOP_LEFT;
}

ui_groupbox::ui_groupbox(const ui_groupbox &_Groupbox):
	ui_element(_Groupbox) {
	copy(_Groupbox);
}

ui_groupbox::ui_groupbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_groupbox &_Groupbox):ui_element(_Owner,_Name,
			_Parent_node,_Draw_order,_Groupbox) {
	copy(_Groupbox);
}

ui_groupbox::~ui_groupbox() {
	clear_elements(); //Must orphan all
}

void ui_groupbox::copy(const ui_groupbox &_Groupbox) {
	//Empty - override default memcpy behaviour
}

//Public

ui_groupbox &ui_groupbox::operator=(const ui_groupbox &_Groupbox) {

	//Worth doing
	if (this!=&_Groupbox) {
	
	ui_element::operator=(_Groupbox); //Invoke base operator=
	copy(_Groupbox);
	}

return *this;
}

void ui_groupbox::add_element(ui_element *_Element) {

	//Nothing to add
	if (!_Element) return;
	
	//Invalid owner
	if (_Element->owner()!=_Owner) return;
	
	//Already adopted
	if (_Element->node()->parent_node()!=_Element->owner()->node()) return;

_Node->adopt(_Element->node()); //Adopt
_Myelements.push_back(_Element); //Add
}

void ui_groupbox::remove_element(ui_element *_Element) {

	for (_Elements::iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Element) {
		
		_Owner->node()->adopt(_Element->node()); //Orphan
		_Myelements.erase(_Iter); //Remove
		return;
		}
	}
}

void ui_groupbox::clear_elements() {

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter)
			_Owner->node()->adopt((*_Iter)->node()); //Orphan

_Myelements.clear();
}

ui_groupbox::elements_iter ui_groupbox::elements_begin() const {
	return _Myelements.begin();
}

ui_groupbox::elements_iter ui_groupbox::elements_end() const {
	return _Myelements.end();
}

ui_element *ui_groupbox::element_pointer(const std::string &_Name) const {

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		//Element exist, return pointer
		if ((*_Iter)->name()==_Name) return (*_Iter);
	}

return 0;
}