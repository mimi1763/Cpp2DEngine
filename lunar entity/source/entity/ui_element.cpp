/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_element.cpp
---------------------------------------------------
*/

#include "ui_element.h"

//Dependencies
#include <algorithm>
#include <utility>

//Dependencies (entity)
#include "ui_menu.h"

//Public

//State
//Public

const std::string ui_element::state::NORMAL="NORMAL";
const std::string ui_element::state::FOCUS="FOCUS";
const std::string ui_element::state::OVER="OVER";
const std::string ui_element::state::PRESS="PRESS";
const std::string ui_element::state::DISABLE="DISABLE";

//Callback event
//Public

const std::string ui_element::callback_event::PRESS="PRESS";
const std::string ui_element::callback_event::RELEASE="RELEASE";
const std::string ui_element::callback_event::FOCUS="FOCUS";
const std::string ui_element::callback_event::DEFOCUS="DEFOCUS";
const std::string ui_element::callback_event::ENTER="ENTER";
const std::string ui_element::callback_event::EXIT="EXIT";
const std::string ui_element::callback_event::CHANGE="CHANGE";

const unsigned int ui_element::STATES=DISABLE+1;

//Element
//Protected

ui_element::ui_element(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,float _Width,
		float _Height,bool _Visible):ui_entity(_Owner->owner(),
			_Parent_node,_Draw_order,_Width,_Height,_Visible),_Owner(_Owner),
				_Name(_Name),_Enabled(true),_Focused(false),_Focusable(false),
					_State(NORMAL),_Onpress(0),_Onrelease(0),_Onfocus(0),
						_Ondefocus(0),_Onenter(0),_Onexit(0),_Onchange(0) {

	for (unsigned int i=0;i<STATES;++i)
		_Caption_color[i]=color::BLACK;
}

ui_element::ui_element(const ui_element &_Element):ui_entity(_Element),
	_Owner(_Element._Owner),_Name(_Element._Name) {

_Focused=false; //Not copyable
_State=NORMAL; //Not copyable
_Onpress=_Onrelease=_Onfocus=_Ondefocus=_Onenter=_Onexit=_Onchange=0; //No callbacks

copy(_Element);
}

ui_element::ui_element(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_element &_Element):ui_entity(_Owner->owner(),
			_Parent_node,_Draw_order,_Element),_Owner(_Owner),
				_Name(_Name) {

_Focused=false; //Not copyable
_State=NORMAL; //Not copyable
_Onpress=_Onrelease=_Onfocus=_Ondefocus=_Onenter=_Onexit=_Onchange=0; //No callbacks

copy(_Element);
}

ui_element::~ui_element() {

release_onpress();
release_onrelease();
release_onfocus();
release_ondefocus();
release_onenter();
release_onexit();
release_onchange();
}

void ui_element::copy(const ui_element &_Element) {

enabled(_Element._Enabled); //Critical functionality
focusable(_Element._Focusable); //Critical functionality
_Hint_caption=_Element._Hint_caption;

	for (unsigned int i=0;i<STATES;++i)
		_Caption_color[i]=_Element._Caption_color[i];
}

//Events

void ui_element::press() {

	if (!_Focused && _Focusable) {
	
	_Owner->defocus();
	focus();
	}

change_state(PRESS);

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::PRESS);

	if (_Sound_group) _Sound_group->play();
	
	//User callback
	if (_Onpress) (*_Onpress)(this);
}

void ui_element::release() {

	if (this==_Owner->_Over_element) change_state(OVER);
	else if (_Focused) change_state(FOCUS);
	else change_state(NORMAL);

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::RELEASE);

	if (_Sound_group) _Sound_group->play();
	
	//User callback
	if (_Onrelease) (*_Onrelease)(this);
}

void ui_element::focus() {

_Focused=true;

	if (_State==NORMAL) change_state(FOCUS);

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::FOCUS);

	if (_Sound_group) _Sound_group->play();
	
	//User callback
	if (_Onfocus) (*_Onfocus)(this);
}

void ui_element::defocus() {

_Focused=false;

	if (_State==FOCUS) change_state(NORMAL);

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::DEFOCUS);

	if (_Sound_group) _Sound_group->play();
	
	//User callback
	if (_Ondefocus) (*_Ondefocus)(this);
}

void ui_element::enter() {

	if (_State!=PRESS) change_state(OVER);

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::ENTER);

	if (_Sound_group) _Sound_group->play();
	
	//Show hint
	if (!_Hint_caption.empty()) {
	
	ui_hint *_Hint=_Owner->hint_pointer();
	
		if (_Hint) _Hint->show(_Hint_caption);
	}
	
	//User callback
	if (_Onenter) (*_Onenter)(this);
}

void ui_element::exit() {

	if (_State!=PRESS) {
	
		if (_Focused) change_state(FOCUS);
		else change_state(NORMAL);
	}

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::EXIT);

	if (_Sound_group) _Sound_group->play();
	
	//Hide hint
	if (!_Hint_caption.empty()) {
	
	ui_hint *_Hint=_Owner->hint_pointer();
	
		if (_Hint) _Hint->hide();
	}
	
	//User callback
	if (_Onexit) (*_Onexit)(this);
}

void ui_element::change() {

//Find and play associated sounds
sound_group *_Sound_group=soundgroup_pointer(callback_event::CHANGE);

	if (_Sound_group) _Sound_group->play();
	
	//User callback
	if (_Onchange) (*_Onchange)(this);
}

//States

std::string ui_element::state_name(const STATE &_State) {

std::string _Name;

	switch (_State) {
	
		case FOCUS:
		_Name=state::FOCUS;
		break;
		
		case OVER:
		_Name=state::OVER;
		break;
		
		case PRESS:
		_Name=state::PRESS;
		break;
		
		case DISABLE:
		_Name=state::DISABLE;
		break;
		
		default:
		_Name=state::NORMAL;
		break;
	};

return _Name.insert(0,"_");
}

object *ui_element::state_object(const std::string &_Part,
	const STATE &_State) const {

object *_Object=object_pointer(_Part+state_name(_State));

	//1st fallback
	if (!_Object) {
	
		if (_Focused)
			_Object=object_pointer(_Part+state_name(FOCUS));
		
		//2nd fallback
		if (!_Object)
			_Object=object_pointer(_Part+state_name(NORMAL));
	}

return _Object;
}

void ui_element::change_state(const STATE &_State) {

object *_Center=state_object(part::CENTER,this->_State);
object *_Border=state_object(part::BORDER,this->_State);

	//Hide previous state
	if (_Center) visible(_Center,false,true);
	if (_Border) visible(_Border,false,true);

_Center=state_object(part::CENTER,_State);
_Border=state_object(part::BORDER,_State);

	//Show new state
	if (_Center) visible(_Center,true,true);
	if (_Border) visible(_Border,true,true);

this->_State=_State;
update_captioncolor();
}

void ui_element::update_state() {
	change_state(_State);
}

//Caption

void ui_element::update_caption() {

ui_entity::update_caption(); //Use base functionality
update_captioncolor();
}

void ui_element::update_captioncolor() {

object *_Object=object_pointer(additional::CAPTION);

	//Update caption color based on current state
	if (_Object && _Caption_color[_State]!=color::ZERO)
		_Object->color_channel(_Caption_color[_State]);
}

//Public

ui_element &ui_element::operator=(const ui_element &_Element) {

	//Worth doing
	if (this!=&_Element) {
	
	ui_entity::operator=(_Element); //Invoke base operator=
	
		//Set state manually to normal
		//Function enabled() will not always trigger change
		if (_Enabled && _Element._Enabled) change_state(NORMAL);
	
	copy(_Element);
	}

return *this;
}

void ui_element::update(float _Time) {

	//Only update if entity is visible
	if (visible()) {
	
		if (_Part_status!=OKAY) {
		
		update_parts(state_name(NORMAL));
		update_parts(state_name(FOCUS));
		update_parts(state_name(OVER));
		update_parts(state_name(PRESS));
		update_parts(state_name(DISABLE));
		}
	}

ui_entity::update(_Time); //Use base functionality
}

bool ui_element::key_pressed(unsigned int _Key) {
	return false; //Empty - override if needed
}

bool ui_element::key_released(unsigned int _Key) {
	return false; //Empty - override if needed
}

bool ui_element::char_pressed(char _Character) {
	return false; //Empty - override if needed
}

void ui_element::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {
	//Empty - override if needed
}

void ui_element::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {
	//Empty - override if needed
}

void ui_element::mouse_moved(unsigned int _X,unsigned int _Y) {
	//Empty - override if needed
}

void ui_element::mouse_scrolled(int _Delta,unsigned int _X,
	unsigned int _Y) {
	//Empty - override if needed
}

ui_menu *ui_element::owner() const {
	return _Owner;
}

const std::string &ui_element::name() const {
	return _Name;
}

void ui_element::enabled(bool _Enabled) {

	if (this->_Enabled!=_Enabled) {
	
		if (_Enabled) change_state(NORMAL);
		else {
		
			if (this==_Owner->_Active_element)
				_Owner->_Active_element=0;
			
			if (this==_Owner->_Over_element) {
			
			exit();
			_Owner->_Over_element=0;
			}
			
			if (_Focused) defocus();
		
		change_state(DISABLE);
		}
	
	this->_Enabled=_Enabled;
	}
}

bool ui_element::enabled() const {
	return _Enabled;
}

void ui_element::focused(bool _Focused) {

	//Cannot focus a disabled element
	if (!_Enabled) return;
	
	if (this->_Focused!=_Focused) {
	
		if (_Focused) {
		
		_Owner->defocus();
		focus();
		}
		else defocus();
	}
}

bool ui_element::focused() const {
	return _Focused;
}

void ui_element::focusable(bool _Focusable) {

	if (this->_Focusable!=_Focusable) {
	
		if (_Focused && !_Focusable) defocus();
	
	this->_Focusable=_Focusable;
	}
}

bool ui_element::focusable() const {
	return _Focusable;
}

void ui_element::tab_order(unsigned int _Tab_order) {

ui_menu::_Elements::iterator _Iter=std::find(
	_Owner->_Myelements.begin(),_Owner->_Myelements.end(),this);

	//Found element
	if (_Iter!=_Owner->_Myelements.end()) {
	
	_Owner->_Myelements.erase(_Iter);
	std::advance(_Iter=_Owner->_Myelements.begin(),_Tab_order);
	_Owner->_Myelements.insert(_Iter,this);
	}
}

unsigned int ui_element::tab_order() const {

unsigned int _Tab_order=0;

	for (ui_menu::_Elements::const_iterator _Iter=_Owner->
		_Myelements.begin(),_End=_Owner->_Myelements.end();
			_Iter!=_End;++_Iter,++_Tab_order) {
	
		//Found element
		if ((*_Iter)==this) break;
	}

return _Tab_order;
}

bool ui_element::functional() const {
	return (_Onpress || _Onrelease || _Onfocus || _Ondefocus ||
		_Onenter || _Onexit || _Onchange);
}

//Caption

void ui_element::hint_caption(const std::string &_Hint_caption) {
	this->_Hint_caption=_Hint_caption;
}

const std::string &ui_element::hint_caption() const {
	return _Hint_caption;
}

void ui_element::caption_color(const color &_Color) {

	for (unsigned int i=0;i<STATES;++i) {
	
		if (_Caption_color[i]!=_Color) {
		
		_Caption_color[i]=_Color;
		_Update_caption=true;
		}
	}
}

void ui_element::caption_color(const color &_Color,const STATE &_State) {

	if (_Caption_color[_State]!=_Color) {
	
	_Caption_color[_State]=_Color;
	_Update_caption=true;
	}
}

const color &ui_element::caption_color(const STATE &_State) const {
	return _Caption_color[_State];
}

//Callbacks

void ui_element::onpress(static_callback<ui_element*>::function _Function) {

	if (_Onpress) release_onpress();

_Onpress=new static_callback<ui_element*>(_Function);
}

void ui_element::onrelease(static_callback<ui_element*>::function _Function) {

	if (_Onrelease) release_onrelease();

_Onrelease=new static_callback<ui_element*>(_Function);
}

void ui_element::onfocus(static_callback<ui_element*>::function _Function) {

	if (_Onfocus) release_onfocus();

_Onfocus=new static_callback<ui_element*>(_Function);
}

void ui_element::ondefocus(static_callback<ui_element*>::function _Function) {

	if (_Ondefocus) release_ondefocus();

_Ondefocus=new static_callback<ui_element*>(_Function);
}

void ui_element::onenter(static_callback<ui_element*>::function _Function) {

	if (_Onenter) release_onenter();

_Onenter=new static_callback<ui_element*>(_Function);
}

void ui_element::onexit(static_callback<ui_element*>::function _Function) {

	if (_Onexit) release_onexit();

_Onexit=new static_callback<ui_element*>(_Function);
}

void ui_element::onchange(static_callback<ui_element*>::function _Function) {

	if (_Onchange) release_onchange();

_Onchange=new static_callback<ui_element*>(_Function);
}

void ui_element::release_onpress() {

	if (_Onpress) {
	
	delete _Onpress;
	_Onpress=0;
	}
}

void ui_element::release_onrelease() {

	if (_Onrelease) {
	
	delete _Onrelease;
	_Onrelease=0;
	}
}

void ui_element::release_onfocus() {

	if (_Onfocus) {
	
	delete _Onfocus;
	_Onfocus=0;
	}
}

void ui_element::release_ondefocus() {

	if (_Ondefocus) {
	
	delete _Ondefocus;
	_Ondefocus=0;
	}
}

void ui_element::release_onenter() {

	if (_Onenter) {
	
	delete _Onenter;
	_Onenter=0;
	}
}

void ui_element::release_onexit() {

	if (_Onexit) {
	
	delete _Onexit;
	_Onexit=0;
	}
}

void ui_element::release_onchange() {

	if (_Onchange) {
	
	delete _Onchange;
	_Onchange=0;
	}
}