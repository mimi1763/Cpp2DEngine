/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_menu.h
---------------------------------------------------
*/

#ifndef _UI_MENU_
#define _UI_MENU_

//Dependencies (entity)
#include "ui_hint.h"
#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_groupbox.h"
#include "ui_label.h"
#include "ui_listbox.h"
#include "ui_progressbar.h"
#include "ui_radiobutton.h"
#include "ui_scrollbar.h"
#include "ui_textbox.h"

//Forward declarations (entity)
class ui_manager;

class ui_menu:public ui_entity {

	friend ui_manager;
	
	friend ui_element;
	friend ui_radiobutton;

protected:

	typedef std::list<ui_element*> _Elements;
	
	ui_manager *_Owner;
	std::string _Name;
	bool _Enabled;
	ui_hint *_Hint;
	
	ui_element *_Active_element;
	ui_element *_Over_element;
	_Elements _Myelements;
	
	//Protected constructor
	ui_menu(ui_manager *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_menu(const ui_menu &_Menu);
	
	//Protected alternative copy constructor
	ui_menu(ui_manager *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_menu &_Menu);
	
	//Protected virtual destructor
	virtual ~ui_menu();
	
	void copy(const ui_menu &_Menu);
	
	virtual void client_changed(float _Width,float _Height);
	
	//Elements
	
	virtual void element_added(ui_element *_Element);
	virtual void element_removed(ui_element *_Element);
	
	virtual ui_element *intersect_element(const vector2 &_Position) const;
	
	int base_draworder() const;
	
	//Tab/focus
	
	void tab(bool _Forward=true);
	_Elements::const_iterator focused_element() const;

public:

	typedef _Elements::const_iterator elements_iter; //Keep data safe
	
	ui_menu &operator=(const ui_menu &_Menu);
	
	virtual void update(float _Time);
	virtual void post_update(float _Time);
	
	virtual void key_pressed(unsigned int _Key);
	virtual void key_released(unsigned int _Key);
	virtual void char_pressed(char _Character);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual bool mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual bool mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);
	
	ui_manager *owner() const;
	const std::string &name() const;
	
	void show(bool _Modal=true);
	void close();
	
	void enabled(bool _Enabled);
	bool enabled() const;
	
	virtual void clean();
	void defocus();
	
	//Hint
	
	ui_hint *create_hint(const interval &_Draw_order,float _Width,
		float _Height);
	ui_hint *create_hint(const interval &_Draw_order,const ui_hint &_Hint);
	
	void remove_hint();
	ui_hint *hint_pointer() const;
	
	//Elements
	//Element
	
	ui_element *create_element(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_element *create_element(const std::string &_Name,const interval &_Draw_order,
		const ui_element &_Element);
	
	//Button
	
	ui_button *create_button(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_button *create_button(const std::string &_Name,const interval &_Draw_order,
		const ui_button &_Button);
	
	//Checkbox
	
	ui_checkbox *create_checkbox(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_checkbox *create_checkbox(const std::string &_Name,const interval &_Draw_order,
		const ui_checkbox &_Checkbox);
	
	//Groupbox
	
	ui_groupbox *create_groupbox(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_groupbox *create_groupbox(const std::string &_Name,const interval &_Draw_order,
		const ui_groupbox &_Groupbox);
	
	//Label
	
	ui_label *create_label(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_label *create_label(const std::string &_Name,const interval &_Draw_order,
		const ui_label &_Label);
	
	//Listbox
	
	ui_listbox *create_listbox(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_listbox *create_listbox(const std::string &_Name,const interval &_Draw_order,
		const ui_listbox &_Listbox);
	
	//Progressbar
	
	ui_progressbar *create_progressbar(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_progressbar *create_progressbar(const std::string &_Name,const interval &_Draw_order,
		const ui_progressbar &_Progressbar);
	
	//Radiobutton
	
	ui_radiobutton *create_radiobutton(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_radiobutton *create_radiobutton(const std::string &_Name,const interval &_Draw_order,
		const ui_radiobutton &_Radiobutton);
	
	//Scrollbar
	
	ui_scrollbar *create_scrollbar(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_scrollbar *create_scrollbar(const std::string &_Name,const interval &_Draw_order,
		const ui_scrollbar &_Scrollbar);
	
	//Textbox
	
	ui_textbox *create_textbox(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=true);
	ui_textbox *create_textbox(const std::string &_Name,const interval &_Draw_order,
		const ui_textbox &_Textbox);
	
	void remove_element(ui_element *&_Element);
	void clear_elements();
	
	elements_iter elements_begin() const;
	elements_iter elements_end() const;
	
	ui_element *element_pointer(const std::string &_Name) const;
};

#endif