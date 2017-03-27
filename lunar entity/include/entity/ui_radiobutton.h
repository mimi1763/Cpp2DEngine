/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_radiobutton.h
---------------------------------------------------
*/

#ifndef _UI_RADIOBUTTON_
#define _UI_RADIOBUTTON_

//Dependencies (entity)
#include "ui_checkbox.h"

class ui_radiobutton:public ui_checkbox {

	friend ui_menu;

protected:

	unsigned int _Tag;
	
	//Protected constructor
	ui_radiobutton(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_radiobutton(const ui_radiobutton &_Radiobutton);
	
	//Protected alternative copy constructor
	ui_radiobutton(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_radiobutton &_Radiobutton);
	
	//Protected virtual destructor
	virtual ~ui_radiobutton();
	
	void copy(const ui_radiobutton &_Radiobutton);
	
	//Events
	
	virtual void release();
	
	//States
	
	void uncheck(unsigned int _Tag);

public:

	ui_radiobutton &operator=(const ui_radiobutton &_Radiobutton);
	
	virtual void checked(bool _Checked);
	
	void tag(unsigned int _Tag);
	unsigned int tag() const;
};

#endif