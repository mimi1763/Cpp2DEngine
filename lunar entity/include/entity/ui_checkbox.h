/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_checkbox.h
---------------------------------------------------
*/

#ifndef _UI_CHECKBOX_
#define _UI_CHECKBOX_

//Dependencies (entity)
#include "ui_element.h"

class ui_checkbox:public ui_element {

	friend ui_menu;

public:

	struct additional:public ui_entity::additional {
		static const std::string CHECK;
	};

protected:

	bool _Checked;
	
	//Protected constructor
	ui_checkbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_checkbox(const ui_checkbox &_Checkbox);
	
	//Protected alternative copy constructor
	ui_checkbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_checkbox &_Checkbox);
	
	//Protected virtual destructor
	virtual ~ui_checkbox();
	
	void copy(const ui_checkbox &_Checkbox);
	
	//Events
	
	virtual void release();
	
	//States
	
	void show_state(const STATE &_State);
	void hide_state(const STATE &_State);
	
	virtual void change_state(const STATE &_State);

public:

	ui_checkbox &operator=(const ui_checkbox &_Checkbox);
	
	virtual bool functional() const;
	
	virtual void checked(bool _Checked);
	bool checked() const;
};

#endif