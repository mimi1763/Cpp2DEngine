/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_label.h
---------------------------------------------------
*/

#ifndef _UI_LABEL_
#define _UI_LABEL_

//Dependencies (entity)
#include "ui_element.h"

class ui_label:public ui_element {

	friend ui_menu;

protected:

	//Protected constructor
	ui_label(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_label(const ui_label &_Label);
	
	//Protected alternative copy constructor
	ui_label(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_label &_Label);
	
	//Protected virtual destructor
	virtual ~ui_label();
	
	void copy(const ui_label &_Label);

public:

	ui_label &operator=(const ui_label &_Label);
};

#endif