/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_button.h
---------------------------------------------------
*/

#ifndef _UI_BUTTON_
#define _UI_BUTTON_

//Dependencies (entity)
#include "ui_element.h"

class ui_button:public ui_element {

	friend ui_menu;

public:

	enum ACTION { SHOW_MENU, SHOW_MODAL_MENU, CLOSE_MENU, ENABLE_MENU, DISABLE_MENU };

protected:

	typedef std::pair<ACTION,std::string> _Procedure;
	typedef std::vector<_Procedure> _Procedures; //Need random access
	
	_Procedures _Myprocedures;
	
	//Protected constructor
	ui_button(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_button(const ui_button &_Button);
	
	//Protected alternative copy constructor
	ui_button(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_button &_Button);
	
	//Protected virtual destructor
	virtual ~ui_button();
	
	void copy(const ui_button &_Button);
	
	//Events
	
	virtual void release();

public:

	ui_button &operator=(const ui_button &_Button);
	
	virtual bool functional() const;
	
	//Procedure
	
	void add_procedure(const ACTION &_Action,const std::string &_Argument);
	void change_procedure(unsigned int _Index,
		const ACTION &_Action,const std::string &_Argument);
	
	void remove_procedure(unsigned int _Index);
	void clear_procedure();
};

#endif