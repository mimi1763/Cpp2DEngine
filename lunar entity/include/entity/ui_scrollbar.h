/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_scrollbar.h
---------------------------------------------------
*/

#ifndef _UI_SCROLLBAR_
#define _UI_SCROLLBAR_

//Dependencies (entity)
#include "ui_element.h"

//Forward declarations (engine)
namespace lunar {
	class texturable;
} //namespace lunar

//Forward declarations (entity)
class ui_scrollable;

class ui_scrollbar:public ui_element {

	friend ui_menu;
	friend ui_scrollable;

public:

	enum TYPE { VERTICAL, HORIZONTAL };
	
	struct additional:public ui_entity::additional {
		static const std::string BAR;
	};

protected:

	TYPE _Type;
	unsigned int _Start;
	unsigned int _Stop;
	unsigned int _Position;
	
	float _Min_barsize;
	float _Max_barsize;
	
	bool _Update_bar;
	bool _Drag_bar;
	ui_scrollable *_Scrollable;
	
	//Protected constructor
	ui_scrollbar(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_scrollbar(const ui_scrollbar &_Scrollbar);
	
	//Protected alternative copy constructor
	ui_scrollbar(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_scrollbar &_Scrollbar);
	
	//Protected virtual destructor
	virtual ~ui_scrollbar();
	
	void copy(const ui_scrollbar &_Scrollbar);
	
	//States
	
	virtual void change_state(const STATE &_State);
	
	//Bar
	
	texturable *bar_component(object *_Object) const;
	
	void update_bar();

public:

	ui_scrollbar &operator=(const ui_scrollbar &_Scrollbar);
	
	virtual void update(float _Time);
	
	virtual bool key_released(unsigned int _Key);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);
	
	virtual bool functional() const;
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void range(unsigned int _Start,unsigned int _Stop);
	unsigned int start() const;
	unsigned int stop() const;
	
	void position(unsigned int _Position);
	unsigned int position() const;
	
	void percent(float _Percent);
	float percent() const;
	
	void bar_size(float _Min_size,float _Max_size);
	float min_barsize() const;
	float max_barsize() const;
	
	//Attach/detach
	
	void attach_scrollable(ui_scrollable *_Scrollable);
	void detach_scrollable();
	
	ui_scrollable *scrollable_pointer() const;
};

#endif