/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_manager.h
---------------------------------------------------
*/

#ifndef _UI_MANAGER_
#define _UI_MANAGER_

//Dependencies (engine)
#include "render_window.h"

//Dependencies (entity)
#include "entity_manager.h"
#include "ui_hud.h"
#include "ui_hint.h"

class ui_manager:public entity_manager {

	friend ui_menu;

public:

	struct cursor {
	
		struct state {
		
		static const std::string NORMAL;
		static const std::string OVER;
		static const std::string PRESS;
		};
	};
	
	struct additional {
		static const std::string CURSOR;
	};

protected:

	enum CURSOR_STATE { NORMAL, OVER, PRESS };
	
	typedef std::list<ui_menu*> _Menus;
	
	struct _Form {
	
	ui_menu *_Active_menu;
	_Menus _Active_menus;
	};
	
	typedef std::vector<_Form> _Forms;
	
	unsigned int _Client_width;
	unsigned int _Client_height;
	
	entity *_Cursor;
	CURSOR_STATE _Cursor_state;
	ui_hint *_Hint;
	
	_Menus _Mymenus;
	_Forms _Myforms;
	
	bool _Update_client;
	void **_Script_session;
	
	void copy(const ui_manager &_Manager);
	
	virtual void client_changed(float _Width,float _Height);
	
	//Menu specific
	
	bool activate(ui_menu *_Menu,bool _Modal=true);
	bool deactivate(ui_menu *_Menu);
	void deactivate();
	
	void tab(bool _Forward=true);
	
	//Script specific
	
	virtual void init_entityscript(script_parser *_Script_parser);
	virtual bool read_object(entity *&_Entity,const script_class::object &_Object,
		const interval &_Draw_order,bool _Visible);
	
	ui_menu *init_ui(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=true);
	
	//States
	
	static std::string cursor_statename(const CURSOR_STATE &_Cursor_state);
	void cursor_state(const CURSOR_STATE &_Cursor_state);

public:

	typedef _Menus::const_iterator menus_iterator; //Keep data safe
	
	//Constructor
	ui_manager(render_node *_Parent_node,int _Draw_order,
		unsigned int _Client_width,unsigned int _Client_height);
	
	//Copy constructor
	ui_manager(const ui_manager &_Manager);
	
	//Virtual destructor
	virtual ~ui_manager();
	
	ui_manager &operator=(const ui_manager &_Manager);
	
	virtual void update(float _Time);
	void post_update(float _Time);
	
	bool message(HWND _Window_handle,UINT _Message,
		WPARAM _Wparam,LPARAM _Lparam);
	
	void key_pressed(unsigned int _Key);
	void key_released(unsigned int _Key);
	void char_pressed(char _Character);
	
	void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	void mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	void mouse_moved(unsigned int _X,unsigned int _Y);
	
	void mouse_scrolled(int _Delta,unsigned int _X,
		unsigned int _Y);
	
	void client_size(unsigned int _Client_width,
		unsigned int _Client_height);
	unsigned int client_width() const;
	unsigned int client_height() const;
	
	//Cursor
	
	entity *create_cursor(bool _Visible=true);
	entity *create_cursor(const entity &_Entity);
	
	void remove_cursor();
	entity *cursor_pointer() const;
	
	//Hint
	
	ui_hint *create_hint(const interval &_Draw_order,float _Width,
		float _Height);
	ui_hint *create_hint(const interval &_Draw_order,const ui_hint &_Hint);
	
	void remove_hint();
	ui_hint *hint_pointer() const;
	
	//UI
	
	ui_menu *create_ui(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=false);
	
	//Menus
	//Menu
	
	ui_menu *create_menu(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=false);
	ui_menu *create_menu(const std::string &_Name,const interval &_Draw_order,
		const ui_menu &_Menu);
	
	ui_menu *create_menu(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=false);
	void create_menus(file_data *_Script_data,const interval &_Draw_order,
		bool _Visible=false);
	
	//Hud
	
	ui_hud *create_hud(const std::string &_Name,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible=false);
	ui_hud *create_hud(const std::string &_Name,const interval &_Draw_order,
		const ui_hud &_Hud);
	
	void remove_menu(ui_menu *&_Menu);
	void clear_menus();
	
	menus_iterator menus_begin() const;
	menus_iterator menus_end() const;
	
	ui_menu *menu_pointer(const std::string &_Name) const;
	ui_menu *active_menu() const;
};

#endif