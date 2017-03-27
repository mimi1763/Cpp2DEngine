/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_menu.cpp
---------------------------------------------------
*/

#include "ui_menu.h"

//Dependencies (engine)
#include "input_manager.h"
#include "render_node.h"
#include "rectangle.h"

//Dependencies (entity)
#include "ui_manager.h"

//Protected

ui_menu::ui_menu(ui_manager *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_entity(_Owner,_Parent_node,_Draw_order,_Width,_Height,_Visible),
				_Owner(_Owner),_Name(_Name),_Enabled(_Visible),_Hint(0),
					_Active_element(0),_Over_element(0) {
	//Empty
}

ui_menu::ui_menu(const ui_menu &_Menu):ui_entity(_Menu),
	_Owner(_Menu._Owner),_Name(_Menu._Name),_Hint(0),
		_Active_element(0),_Over_element(0) {
	copy(_Menu);
}

ui_menu::ui_menu(ui_manager *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,const ui_menu &_Menu):
		ui_entity(_Owner,_Parent_node,_Draw_order,_Menu),_Owner(_Owner),
			_Name(_Name),_Hint(0),_Active_element(0),_Over_element(0) {
	copy(_Menu);
}

ui_menu::~ui_menu() {

clear_elements();
remove_hint();
}

void ui_menu::copy(const ui_menu &_Menu) {

enabled(_Menu._Enabled); //Critical functionality

	if (_Menu._Hint)
		create_hint(_Menu._Hint->draw_order(),*_Menu._Hint);
	
	if (!_Menu._Myelements.empty()) {
	
	ui_element *_Element;
	
		for (_Elements::const_iterator _Iter=_Menu._Myelements.begin(),
			_End=_Menu._Myelements.end();_Iter!=_End;++_Iter) {
		
		_Element=(*_Iter); //Increase speed
		
		ui_button *_Button=dynamic_cast<ui_button*>(_Element);
		
			if (_Button) {
			
			create_button(_Element->_Name,_Element->draw_order(),*_Button);
			continue;
			}
		
		ui_checkbox *_Checkbox=dynamic_cast<ui_checkbox*>(_Element);
		
			if (_Checkbox) {
			
			create_checkbox(_Element->_Name,_Element->draw_order(),*_Checkbox);
			continue;
			}
		
		ui_groupbox *_Groupbox=dynamic_cast<ui_groupbox*>(_Element);
		
			if (_Groupbox) {
			
			create_groupbox(_Element->_Name,_Element->draw_order(),*_Groupbox);
			continue;
			}
		
		ui_label *_Label=dynamic_cast<ui_label*>(_Element);
		
			if (_Label) {
			
			create_label(_Element->_Name,_Element->draw_order(),*_Label);
			continue;
			}
		
		ui_listbox *_Listbox=dynamic_cast<ui_listbox*>(_Element);
		
			if (_Listbox) {
			
			create_listbox(_Element->_Name,_Element->draw_order(),*_Listbox);
			continue;
			}
		
		ui_progressbar *_Progressbar=dynamic_cast<ui_progressbar*>(_Element);
		
			if (_Progressbar) {
			
			create_progressbar(_Element->_Name,_Element->draw_order(),*_Progressbar);
			continue;
			}
		
		ui_radiobutton *_Radiobutton=dynamic_cast<ui_radiobutton*>(_Element);
		
			if (_Radiobutton) {
			
			create_radiobutton(_Element->_Name,_Element->draw_order(),*_Radiobutton);
			continue;
			}
		
		ui_scrollbar *_Scrollbar=dynamic_cast<ui_scrollbar*>(_Element);
		
			if (_Scrollbar) {
			
			create_scrollbar(_Element->_Name,_Element->draw_order(),*_Scrollbar);
			continue;
			}
		
		ui_textbox *_Textbox=dynamic_cast<ui_textbox*>(_Element);
		
			if (_Textbox) {
			
			create_textbox(_Element->_Name,_Element->draw_order(),*_Textbox);
			continue;
			}
		
		create_element(_Element->_Name,_Element->draw_order(),*_Element);
		}
	
	ui_groupbox *_Groupbox;
	ui_groupbox *_Groupbox2;
	
		//Find newly added groupboxes
		for (_Elements::const_iterator _Iter=_Myelements.begin(),
			_Iter2=_Menu._Myelements.begin(),_End=_Myelements.end();
				_Iter!=_End;++_Iter,++_Iter2) {
		
		_Groupbox=dynamic_cast<ui_groupbox*>(*_Iter); //Increase speed
		
			if (!_Groupbox) continue;
		
		_Groupbox2=static_cast<ui_groupbox*>(*_Iter2); //Must be of equal type
		
			//Add groupbox elements
			for (_Elements::const_iterator _Eiter=_Groupbox2->_Myelements.begin(),
				_Eend=_Groupbox2->_Myelements.end();_Eiter!=_Eend;++_Eiter)
					_Groupbox->_Myelements.push_back(element_pointer((*_Eiter)->_Name));
		}
	}
}

void ui_menu::client_changed(float _Width,float _Height) {

ui_entity::client_changed(_Width,_Height);

	//Nothing to do
	if (_Myelements.empty()) return;
	
	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter)
			(*_Iter)->client_changed(_Width,_Height);
}

//Elements

void ui_menu::element_added(ui_element *_Element) {
	//Empty - override if needed
}

void ui_menu::element_removed(ui_element *_Element) {
	//Empty - override if needed
}

ui_element *ui_menu::intersect_element(const vector2 &_Position) const {

ui_element *_Element;
ui_element *_Intersect_element=0;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Element=(*_Iter); //Increase speed
	
		if (!_Element->_Enabled || !_Element->visible()) continue;
	
	_Element->_Area->prepare(); //Prepare bounds
	
		//Check if cursor is intersecting a element
		if (_Element->_Area->world_orientedboundingbox().
			intersects(_Position)) {
		
			if (!_Intersect_element || _Element->_Node->draw_order()>=
				_Intersect_element->_Node->draw_order())
					_Intersect_element=_Element;
		}
	}

return _Intersect_element;
}

int ui_menu::base_draworder() const {
	return (_Node->draw_order()-_Owner->node()->draw_order());
}

//Tab/focus

void ui_menu::tab(bool _Forward) {

	//Nothing to do
	if (_Myelements.empty()) return;

_Elements::const_iterator _Focused=focused_element();
ui_element *_Element;

	for (_Elements::const_iterator _Iter=(_Forward?(_Focused==_Myelements.end()?
		_Myelements.begin():++_Elements::const_iterator(_Focused)):
			(_Focused==_Myelements.begin()?--_Myelements.end():
				--_Elements::const_iterator(_Focused)));_Iter!=_Focused;) {
	
		//Forward cycle
		if (_Forward && _Iter==_Myelements.end()) {
		
		_Iter=_Myelements.begin();
		_Owner->tab();
		continue;
		}
		
		//Backward cycle
		if (!_Forward && _Iter==_Myelements.end())
			_Iter=--_Myelements.end();
	
	_Element=(*_Iter); //Increase
	
		//Check if element can be focused
		if (_Element->_Enabled && _Element->_Focusable &&
			_Element->visible()) {
		
			if (_Focused!=_Myelements.end()) (*_Focused)->defocus();
		
		_Element->focus();
		return;
		}
		
		//Backward cycle
		if (!_Forward && _Iter==_Myelements.begin()) {
		
		_Iter=_Myelements.end();
		_Owner->tab(false);
		continue;
		}
		
		//Check next/previous element
		if (_Forward) ++_Iter;
		else --_Iter;
	}

_Owner->tab(_Forward);
}

ui_menu::_Elements::const_iterator ui_menu::focused_element() const {

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		//Focused element found, return iterator
		if ((*_Iter)->_Focused) return _Iter;
	}

return _Myelements.end();
}

//Public

ui_menu &ui_menu::operator=(const ui_menu &_Menu) {

	//Worth doing
	if (this!=&_Menu) {
	
	ui_entity::operator=(_Menu); //Invoke base operator=
	
	remove_hint();
	clear_elements();
	copy(_Menu);
	}

return *this;
}

void ui_menu::update(float _Time) {

ui_entity::update(_Time); //Use base functionality

	if (_Hint) _Hint->update(_Time);
	
	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter)
			(*_Iter)->update(_Time);
}

void ui_menu::post_update(float _Time) {
	//Empty - override if needed
}

void ui_menu::key_pressed(unsigned int _Key) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;

ui_element *_Focused_element=_Active_element;

	if (!_Focused_element) {
	
	_Elements::const_iterator _Focused=focused_element();
	
		if (_Focused!=_Myelements.end())
			_Focused_element=(*_Focused);
	}
	
	switch (_Key) {
	
		case VK_RETURN:
		case VK_SPACE: {
		
			if (!_Active_element && _Focused_element) {
			
			_Active_element=_Focused_element;
			_Active_element->press();
			
			//Still active
			_Focused_element=_Active_element;
			}
		
		break;
		}
	}
	
	if (_Focused_element && _Focused_element->key_pressed(_Key))
		return; //Skip global menu functions

//Global menu functions (key pressed)
}

void ui_menu::key_released(unsigned int _Key) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;
	
	switch (_Key) {
	
		case VK_RETURN:
		case VK_SPACE: {
		
			if (_Active_element) {
			
			_Active_element->release();
			_Active_element=0;
			}
		
		break;
		}
	}

ui_element *_Focused_element=_Active_element;

	if (!_Focused_element) {
	
	_Elements::const_iterator _Focused=focused_element();
	
		if (_Focused!=_Myelements.end())
			_Focused_element=(*_Focused);
	}
	
	if (_Focused_element && _Focused_element->key_released(_Key))
		return; //Skip global menu functions
	
	//Global menu functions (key released)
	switch (_Key) {
	
		case VK_TAB:
		tab(!input_manager::instance()->key_active(VK_SHIFT)); //Bidirectional tab
		break;
		
		case VK_DOWN:
		case VK_RIGHT:
		tab(); //Forward tab
		break;
		
		case VK_UP:
		case VK_LEFT:
		tab(false); //Backward tab
		break;
	}
}

void ui_menu::char_pressed(char _Character) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;

ui_element *_Focused_element=_Active_element;

	if (!_Focused_element) {
	
	_Elements::const_iterator _Focused=focused_element();
	
		if (_Focused!=_Myelements.end())
			_Focused_element=(*_Focused);
	}
	
	if (_Focused_element && _Focused_element->char_pressed(_Character))
		return; //Skip global menu functions

//Global menu functions (char pressed)
}

void ui_menu::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled || _Active_element) return;

ui_element *_Element;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Element=(*_Iter); //Increase speed
	
		if (!_Element->_Enabled || !_Element->visible()) continue;
		
		//Check if cursor is over element
		if (_Element->_State==ui_element::OVER) {
		
		_Active_element=_Element;
		_Active_element->press();
		
			//Still active
			if (_Active_element)
				_Active_element->mouse_pressed(_Button,_X,_Y);
		
		break;
		}
	}
}

bool ui_menu::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return false;
	
	if (_Active_element) {
	
		if (_Active_element==_Over_element) {
		
		_Active_element->release();
		
			//Still active
			if (_Active_element)
				_Active_element->mouse_released(_Button,_X,_Y);
		}
		//Cancel release
		else {
		
			//Change state manually
			if (_Active_element->_Focused)
				_Active_element->change_state(ui_element::FOCUS);
			else _Active_element->change_state(ui_element::NORMAL);
		}
	
	_Active_element=0;
	}

return (_Over_element && _Over_element->functional());
}

bool ui_menu::mouse_moved(unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return false;

ui_element *_Element=intersect_element(vector2((float)_X,(float)_Y));
ui_element *_Entered_element=0;

	//Element has changed
	if (_Element && _Element!=_Over_element)
		_Entered_element=_Element;
	
	//Exit previous element
	if (_Over_element && _Element!=_Over_element) {
	
	_Over_element->exit();
	_Over_element=0;
	}
	
	//Enter new element
	if (_Entered_element) {
	
	_Over_element=_Entered_element;
	_Over_element->enter();
	}
	
	//Still over
	if (_Over_element)
		_Over_element->mouse_moved(_X,_Y);

return (_Over_element && _Over_element->functional());
}

void ui_menu::mouse_scrolled(int _Delta,unsigned int _X,
	unsigned int _Y) {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;

ui_element *_Element=intersect_element(vector2((float)_X,(float)_Y));

	if (_Element && _Element==_Over_element)
		_Over_element->mouse_scrolled(_Delta,_X,_Y);
}

ui_manager *ui_menu::owner() const {
	return _Owner;
}

const std::string &ui_menu::name() const {
	return _Name;
}

void ui_menu::show(bool _Modal) {

ui_menu *_Active_menu=_Owner->active_menu();

	//Deactive active menu
	if (_Active_menu && _Active_menu!=this)
		_Active_menu->clean();

//Show
visible(true);
ui_element *_Myelement;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter)
	{
		_Myelement=(*_Iter); //Increase speed

		_Myelement->update_state();
		_Myelement->visible(true);
	}

_Owner->activate(this,_Modal);
}

void ui_menu::close() {

	//Close
	if (_Owner->deactivate(this)) {
	
	ui_hint *_Hint=hint_pointer();
	
		if (_Hint) _Hint->hide(true);
		
		for (_Elements::const_iterator _Iter=_Myelements.begin(),
			_End=_Myelements.end();_Iter!=_End;++_Iter)
				(*_Iter)->visible(false);
	
	visible(false);
	}
}

void ui_menu::enabled(bool _Enabled) {

	if (this->_Enabled!=_Enabled) {
	
		//Focus first element
		if (_Enabled) tab();
		else {
		
		clean();
		defocus();
		}
	
	this->_Enabled=_Enabled;
	}
}

bool ui_menu::enabled() const {
	return _Enabled;
}

void ui_menu::clean() {

	//Cancel active element
	if (_Active_element) {
	
		//Change state manually
		if (_Active_element->_Focused)
			_Active_element->change_state(ui_element::FOCUS);
		else _Active_element->change_state(ui_element::NORMAL);
	}

_Active_element=0;

	//Exit over element
	if (_Over_element) {
	
	_Over_element->exit();
	_Over_element=0;
	}
}

void ui_menu::defocus() {

	//Nothing to do
	if (_Myelements.empty() || !_Enabled) return;

_Elements::const_iterator _Focused=focused_element();

	if (_Focused!=_Myelements.end()) (*_Focused)->defocus();
}

//Hint

ui_hint *ui_menu::create_hint(const interval &_Draw_order,
	float _Width,float _Height) {

	if (!_Hint)
		_Hint=new ui_hint(_Owner,_Node,interval(
			base_draworder())+_Draw_order,_Width,_Height);

return _Hint;
}

ui_hint *ui_menu::create_hint(const interval &_Draw_order,
	const ui_hint &_Hint) {

	if (!this->_Hint)
		this->_Hint=new ui_hint(_Owner,_Node,interval(
			base_draworder())+_Draw_order,_Hint);

return this->_Hint;
}

void ui_menu::remove_hint() {

	if (_Hint) {
	
	delete _Hint;
	_Hint=0;
	}
}

ui_hint *ui_menu::hint_pointer() const {
	return (_Hint?_Hint:_Owner->hint_pointer());
}

//Elements
//Element

ui_element *ui_menu::create_element(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_element *_Myelement=new ui_element(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Myelement);
element_added(_Myelement);
return _Myelement;
}

ui_element *ui_menu::create_element(const std::string &_Name,
	const interval &_Draw_order,const ui_element &_Element) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_element *_Myelement=new ui_element(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Element);
_Myelements.push_back(_Myelement);
element_added(_Myelement);
return _Myelement;
}

//Button

ui_button *ui_menu::create_button(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_button *_Mybutton=new ui_button(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mybutton);
element_added(_Mybutton);
return _Mybutton;
}

ui_button *ui_menu::create_button(const std::string &_Name,
	const interval &_Draw_order,const ui_button &_Button) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_button *_Mybutton=new ui_button(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Button);
_Myelements.push_back(_Mybutton);
element_added(_Mybutton);
return _Mybutton;
}

//Checkbox

ui_checkbox *ui_menu::create_checkbox(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_checkbox *_Mycheckbox=new ui_checkbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mycheckbox);
element_added(_Mycheckbox);
return _Mycheckbox;
}

ui_checkbox *ui_menu::create_checkbox(const std::string &_Name,
	const interval &_Draw_order,const ui_checkbox &_Checkbox) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_checkbox *_Mycheckbox=new ui_checkbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Checkbox);
_Myelements.push_back(_Mycheckbox);
element_added(_Mycheckbox);
return _Mycheckbox;
}

//Groupbox

ui_groupbox *ui_menu::create_groupbox(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_groupbox *_Mygroupbox=new ui_groupbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mygroupbox);
element_added(_Mygroupbox);
return _Mygroupbox;
}

ui_groupbox *ui_menu::create_groupbox(const std::string &_Name,
	const interval &_Draw_order,const ui_groupbox &_Groupbox) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_groupbox *_Mygroupbox=new ui_groupbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Groupbox);
_Myelements.push_back(_Mygroupbox);
element_added(_Mygroupbox);
return _Mygroupbox;
}

//Label

ui_label *ui_menu::create_label(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_label *_Mylabel=new ui_label(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mylabel);
element_added(_Mylabel);
return _Mylabel;
}

ui_label *ui_menu::create_label(const std::string &_Name,
	const interval &_Draw_order,const ui_label &_Label) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_label *_Mylabel=new ui_label(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Label);
_Myelements.push_back(_Mylabel);
element_added(_Mylabel);
return _Mylabel;
}

//Listbox

ui_listbox *ui_menu::create_listbox(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_listbox *_Mylistbox=new ui_listbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mylistbox);
element_added(_Mylistbox);
return _Mylistbox;
}

ui_listbox *ui_menu::create_listbox(const std::string &_Name,
	const interval &_Draw_order,const ui_listbox &_Listbox) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_listbox *_Mylistbox=new ui_listbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Listbox);
_Myelements.push_back(_Mylistbox);
element_added(_Mylistbox);
return _Mylistbox;
}

//Progressbar

ui_progressbar *ui_menu::create_progressbar(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_progressbar *_Myprogressbar=new ui_progressbar(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Myprogressbar);
element_added(_Myprogressbar);
return _Myprogressbar;
}

ui_progressbar *ui_menu::create_progressbar(const std::string &_Name,
	const interval &_Draw_order,const ui_progressbar &_Progressbar) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_progressbar *_Myprogressbar=new ui_progressbar(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Progressbar);
_Myelements.push_back(_Myprogressbar);
element_added(_Myprogressbar);
return _Myprogressbar;
}

//Radiobutton

ui_radiobutton *ui_menu::create_radiobutton(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_radiobutton *_Myradiobutton=new ui_radiobutton(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Myradiobutton);
element_added(_Myradiobutton);
return _Myradiobutton;
}

ui_radiobutton *ui_menu::create_radiobutton(const std::string &_Name,
	const interval &_Draw_order,const ui_radiobutton &_Radiobutton) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_radiobutton *_Myradiobutton=new ui_radiobutton(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Radiobutton);
_Myelements.push_back(_Myradiobutton);
element_added(_Myradiobutton);
return _Myradiobutton;
}

//Scrollbar

ui_scrollbar *ui_menu::create_scrollbar(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_scrollbar *_Myscrollbar=new ui_scrollbar(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Myscrollbar);
element_added(_Myscrollbar);
return _Myscrollbar;
}

ui_scrollbar *ui_menu::create_scrollbar(const std::string &_Name,
	const interval &_Draw_order,const ui_scrollbar &_Scrollbar) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_scrollbar *_Myscrollbar=new ui_scrollbar(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Scrollbar);
_Myelements.push_back(_Myscrollbar);
element_added(_Myscrollbar);
return _Myscrollbar;
}

//Textbox
	
ui_textbox *ui_menu::create_textbox(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_textbox *_Mytextbox=new ui_textbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Width,_Height,
		_Visible && _Node->visible());
_Myelements.push_back(_Mytextbox);
element_added(_Mytextbox);
return _Mytextbox;
}

ui_textbox *ui_menu::create_textbox(const std::string &_Name,
	const interval &_Draw_order,const ui_textbox &_Textbox) {

	//Element already exist
	if (element_pointer(_Name)) return 0;

ui_textbox *_Mytextbox=new ui_textbox(this,_Name,_Node,
	interval(base_draworder())+_Draw_order,_Textbox);
_Myelements.push_back(_Mytextbox);
element_added(_Mytextbox);
return _Mytextbox;
}

void ui_menu::remove_element(ui_element *&_Element) {

	for (_Elements::iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Element) {
		
			//Element is adopted
			if (_Element->_Node->parent_node()!=_Node) {
			
			ui_groupbox *_Groupbox;
			
				//Find adoptee
				for (_Elements::iterator _Giter=_Myelements.begin(),
					_Gend=_Myelements.end();_Giter!=_Gend;++_Giter) {
				
				_Groupbox=dynamic_cast<ui_groupbox*>(*_Giter); //Increase speed
				
					if (!_Groupbox || _Groupbox==_Element) continue;
				
				bool _Found=false;
				
					//Find element
					for (ui_groupbox::_Elements::const_iterator _Eiter=_Groupbox->
						_Myelements.begin(),_Eend=_Groupbox->_Myelements.end();
							_Eiter!=_Eend;++_Eiter) {
					
						if ((*_Eiter)==_Element) {
						
						_Groupbox->remove_element(_Element); //Orphan
						_Found=true;
						break;
						}
					}
					
					if (_Found) break;
				}
			}
		
		element_removed(_Element);
		
		_Element->enabled(false);
		delete _Element;
		_Element=0; //Set referenced pointer to null
		
		_Myelements.erase(_Iter);
		return;
		}
	}
}

void ui_menu::clear_elements() {

enabled(false);

ui_groupbox *_Groupbox;

	//Orphan all adopted elements
	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Groupbox=dynamic_cast<ui_groupbox*>(*_Iter); //Increase speed
	
		//Orphan all
		if (_Groupbox) _Groupbox->clear_elements();
	}

ui_element *_Element;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
	_Element=(*_Iter); //Increase speed
	
	element_removed(_Element);
	delete _Element;
	}

_Myelements.clear();
_Enabled=true;
}

ui_menu::elements_iter ui_menu::elements_begin() const {
	return _Myelements.begin();
}

ui_menu::elements_iter ui_menu::elements_end() const {
	return _Myelements.end();
}

ui_element *ui_menu::element_pointer(const std::string &_Name) const {

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter) {
	
		//Element exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}