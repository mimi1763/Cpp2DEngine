/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_element.h
---------------------------------------------------
*/

#ifndef _UI_ELEMENT_
#define _UI_ELEMENT_

//Dependencies (engine)
#include "callback.h"

//Dependencies (entity)
#include "ui_entity.h"

//Forward declarations (entity)
class ui_menu;

class ui_element:public ui_entity {

	friend ui_menu;

public:

	enum STATE { NORMAL, FOCUS, OVER, PRESS, DISABLE };
	
	struct state {
	
	static const std::string NORMAL;
	static const std::string FOCUS;
	static const std::string OVER;
	static const std::string PRESS;
	static const std::string DISABLE;
	};
	
	struct callback_event {
	
	static const std::string PRESS;
	static const std::string RELEASE;
	static const std::string FOCUS;
	static const std::string DEFOCUS;
	static const std::string ENTER;
	static const std::string EXIT;
	static const std::string CHANGE;
	};
	
	static const unsigned int STATES;

protected:

	ui_menu *_Owner;
	std::string _Name;
	bool _Enabled;
	
	bool _Focused;
	bool _Focusable;
	STATE _State;
	
	std::string _Hint_caption;
	color _Caption_color[5];
	
	//Callbacks
	
	callback<ui_element*> *_Onpress;
	callback<ui_element*> *_Onrelease;
	callback<ui_element*> *_Onfocus;
	callback<ui_element*> *_Ondefocus;
	callback<ui_element*> *_Onenter; //Mouse only
	callback<ui_element*> *_Onexit; //Mouse only
	callback<ui_element*> *_Onchange;
	
	//Protected constructor
	ui_element(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_element(const ui_element &_Element);
	
	//Protected alternative copy constructor
	ui_element(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_element &_Element);
	
	//Protected virtual destructor
	virtual ~ui_element();
	
	void copy(const ui_element &_Element);
	
	//Events
	
	virtual void press();
	virtual void release();
	virtual void focus();
	virtual void defocus();
	virtual void enter();
	virtual void exit();
	virtual void change();
	
	//States
	
	static std::string state_name(const STATE &_State);
	object *state_object(const std::string &_Part,
		const STATE &_State) const;
	
	virtual void change_state(const STATE &_State);
	void update_state();
	
	//Caption
	
	virtual void update_caption();
	void update_captioncolor();

public:

	ui_element &operator=(const ui_element &_Element);
	
	virtual void update(float _Time);
	
	virtual bool key_pressed(unsigned int _Key);
	virtual bool key_released(unsigned int _Key);
	virtual bool char_pressed(char _Character);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);
	
	ui_menu *owner() const;
	const std::string &name() const;
	
	void enabled(bool _Enabled);
	bool enabled() const;
	
	void focused(bool _Focused);
	bool focused() const;
	
	void focusable(bool _Focusable);
	bool focusable() const;
	
	void tab_order(unsigned int _Tab_order);
	unsigned int tab_order() const;
	
	virtual bool functional() const;
	
	//Caption
	
	void hint_caption(const std::string &_Hint_caption);
	const std::string &hint_caption() const;
	
	void caption_color(const color &_Color);
	void caption_color(const color &_Color,const STATE &_State);
	const color &caption_color(const STATE &_State=NORMAL) const;
	
	//Callbacks
	//On press
	
	template <typename _Ty>
	void onpress(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onpress) release_onpress();
	
	_Onpress=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onpress(static_callback<ui_element*>::function _Function);
	
	//On release
	
	template <typename _Ty>
	void onrelease(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onrelease) release_onrelease();
	
	_Onrelease=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onrelease(static_callback<ui_element*>::function _Function);
	
	//On focus
	
	template <typename _Ty>
	void onfocus(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onfocus) release_onfocus();
	
	_Onfocus=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onfocus(static_callback<ui_element*>::function _Function);
	
	//On defocus
	
	template <typename _Ty>
	void ondefocus(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Ondefocus) release_ondefocus();
	
	_Ondefocus=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void ondefocus(static_callback<ui_element*>::function _Function);
	
	//On enter
	
	template <typename _Ty>
	void onenter(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onenter) release_onenter();
	
	_Onenter=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onenter(static_callback<ui_element*>::function _Function);
	
	//On exit
	
	template <typename _Ty>
	void onexit(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onexit) release_onexit();
	
	_Onexit=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onexit(static_callback<ui_element*>::function _Function);
	
	//On change
	
	template <typename _Ty>
	void onchange(typename member_callback<_Ty,ui_element*>::function _Function,
		_Ty *_Class) {
	
		if (_Onchange) release_onchange();
	
	_Onchange=new member_callback<_Ty,ui_element*>(_Function,_Class);
	}
	
	void onchange(static_callback<ui_element*>::function _Function);
	
	void release_onpress();
	void release_onrelease();
	void release_onfocus();
	void release_ondefocus();
	void release_onenter();
	void release_onexit();
	void release_onchange();
};

#endif