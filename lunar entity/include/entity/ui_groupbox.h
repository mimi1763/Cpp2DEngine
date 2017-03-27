/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_groupbox.h
---------------------------------------------------
*/

#ifndef _UI_GROUPBOX_
#define _UI_GROUPBOX_

//Dependencies (entity)
#include "ui_element.h"

class ui_groupbox:public ui_element {

	friend ui_menu;

protected:

	typedef std::list<ui_element*> _Elements;
	
	_Elements _Myelements;
	
	//Protected constructor
	ui_groupbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_groupbox(const ui_groupbox &_Groupbox);
	
	//Protected alternative copy constructor
	ui_groupbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_groupbox &_Groupbox);
	
	//Protected virtual destructor
	virtual ~ui_groupbox();
	
	void copy(const ui_groupbox &_Groupbox);

public:

	typedef _Elements::const_iterator elements_iter; //Keep data safe
	
	ui_groupbox &operator=(const ui_groupbox &_Groupbox);
	
	void add_element(ui_element *_Element);
	
	void remove_element(ui_element *_Element);
	void clear_elements();
	
	elements_iter elements_begin() const;
	elements_iter elements_end() const;
	
	ui_element *element_pointer(const std::string &_Name) const;
};

#endif