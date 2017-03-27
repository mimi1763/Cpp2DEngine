/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_scrollable.h
---------------------------------------------------
*/

#ifndef _UI_SCROLLABLE_
#define _UI_SCROLLABLE_

//Dependencies (entity)
#include "ui_element.h"

//Forward declarations (entity)
class ui_scrollbar;

class ui_scrollable:public ui_element {

	friend ui_scrollbar;

protected:

	unsigned int _Scroll_rate;
	bool _Scrolled;
	
	ui_scrollbar *_Scrollbar;
	
	//Protected constructor
	ui_scrollable(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_scrollable(const ui_scrollable &_Scrollable);
	
	//Protected alternative copy constructor
	ui_scrollable(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_scrollable &_Scrollable);
	
	//Protected virtual destructor
	virtual ~ui_scrollable();
	
	void copy(const ui_scrollable &_Scrollable);
	
	//Events
	
	virtual void scroll(unsigned int _Units,bool _Up=true)=0;
	
	virtual unsigned int scrollbar_count() const;
	virtual unsigned int scrollbar_viewcount() const;
	virtual unsigned int scrollbar_position() const;
	
	void update_scrollbar() const;

public:

	ui_scrollable &operator=(const ui_scrollable &_Scrollable);
	
	virtual bool key_pressed(unsigned int _Key);
	virtual bool key_released(unsigned int _Key);
	
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);
	
	void scroll_rate(unsigned int _Scroll_rate);
	unsigned int scroll_rate() const;
	
	void scroll_top();
	void scroll_up();
	void scroll_down();
	void scroll_bottom();
	
	//Attach/detach
	
	void attach_scrollbar(ui_scrollbar *_Scrollbar);
	void detach_scrollbar();
	
	ui_scrollbar *scrollbar_pointer() const;
};

#endif