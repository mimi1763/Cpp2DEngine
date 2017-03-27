/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_listbox.h
---------------------------------------------------
*/

#ifndef _UI_LISTBOX_
#define _UI_LISTBOX_

//Dependencies (entity)
#include "ui_scrollable.h"

//Forward declarations (engine)
namespace lunar {
	class text;
} //namespace lunar

class ui_listbox:public ui_scrollable {

	friend ui_menu;

public:

	enum ALIGNMENT { LEFT, CENTER, RIGHT };
	enum ICON_ALIGNMENT { LEFT_SIDE, RIGHT_SIDE };
	enum ICON_POSITIONING { FIXED, FLOATING };
	
	struct additional:public ui_entity::additional {
	
	static const std::string TEXT;
	static const std::string SELECTION;
	};
	
	struct item {
	
	std::string data;
	std::string icon;
	};

protected:

	typedef std::vector<item> _Items;
	
	ALIGNMENT _Alignment;
	vector2 _Text_offset;
	unsigned int _Item_index;
	bool _Fit_selection;
	
	ICON_ALIGNMENT _Icon_alignment;
	ICON_POSITIONING _Icon_positioning;
	float _Icon_width;
	float _Icon_spacing;
	bool _Show_icons;
	
	float _Repeat_rate;
	float _Repeat_delay;
	_Items _Myitems;
	
	//Helper variables
	
	bool _Update_items;
	bool _Update_selection;
	
	unsigned int _Repeat_key;
	float _Repeat_time;
	
	//Protected constructor
	ui_listbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_listbox(const ui_listbox &_Listbox);
	
	//Protected alternative copy constructor
	ui_listbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_listbox &_Listbox);
	
	//Protected virtual destructor
	virtual ~ui_listbox();
	
	void copy(const ui_listbox &_Listbox);
	
	//Events
	
	virtual void scroll(unsigned int _Units,bool _Up=true);
	
	virtual unsigned int scrollbar_count() const;
	virtual unsigned int scrollbar_viewcount() const;
	virtual unsigned int scrollbar_position() const;
	
	//Utility
	
	static void clean_data(std::string &_Data);
	
	//Helper
	
	unsigned int item_location(const vector2 &_Position) const;
	text *text_component(object *_Object) const;
	
	void update_items();
	void update_selection();
	
	void show_icon(unsigned int _Item_index);
	void hide_icon(unsigned int _Item_index);
	void remove_icon(unsigned int _Item_index);

public:

	typedef _Items::const_iterator items_iter; //Keep data safe
	
	ui_listbox &operator=(const ui_listbox &_Listbox);
	
	virtual void update(float _Time);
	
	virtual bool key_pressed(unsigned int _Key);
	virtual bool key_released(unsigned int _Key);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual bool functional() const;
	
	void alignment(const ALIGNMENT &_Alignment);
	ALIGNMENT alignment() const;
	
	void text_offset(const vector2 &_Offset);
	const vector2 &text_offset() const;
	
	void item_index(unsigned int _Item_index);
	unsigned int item_index() const;
	
	void fit_selection(bool _Fit_selection);
	bool fit_selection() const;
	
	//Icons
	
	void icon_alignment(const ICON_ALIGNMENT &_Icon_alignment);
	ICON_ALIGNMENT icon_alignment() const;
	
	void icon_positioning(const ICON_POSITIONING &_Icon_positioning);
	ICON_POSITIONING icon_positioning() const;
	
	void icon_width(float _Icon_width);
	float icon_width() const;
	
	void icon_spacing(float _Icon_spacing);
	float icon_spacing() const;
	
	void show_icons(bool _Show_icons);
	bool show_icons() const;
	
	//Selection
	
	void select(unsigned int _Off);
	void deselect();
	
	void repeat_rate(float _Repeat_rate);
	float repeat_rate() const;
	
	void repeat_delay(float _Repeat_delay);
	float repeat_delay() const;
	
	//Items
	
	void add_item(const std::string &_Data);
	void add_item(const item &_Item);
	
	void insert_item(const std::string &_Data,unsigned int _Off=0);
	void insert_item(const item &_Item,unsigned int _Off=0);
	
	void change_item(const std::string &_Data,unsigned int _Off);
	void change_item(const item &_Item,unsigned int _Off);
	
	void remove_item(unsigned int _Off=-1);
	void clear_items();
	
	items_iter items_begin() const;
	items_iter items_end() const;
};

#endif