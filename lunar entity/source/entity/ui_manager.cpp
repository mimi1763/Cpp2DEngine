/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_manager.cpp
---------------------------------------------------
*/

#include "ui_manager.h"

//Dependencies
#include <limits>

//Dependencies (engine)
#include "script_parser.h"
#include "rectangle.h"
#include "render_node.h"

//Dependencies (entity)
#include "input_manager.h"

//Undefine macro max if defined
#ifdef max
#undef max
#endif

//Public

//Cursor
//Public

//State
//Public

const std::string ui_manager::cursor::state::NORMAL="NORMAL";
const std::string ui_manager::cursor::state::OVER="OVER";
const std::string ui_manager::cursor::state::PRESS="PRESS";

//Additional
//Public

const std::string ui_manager::additional::CURSOR="CURSOR";

//UI manager
//Protected

void ui_manager::copy(const ui_manager &_Manager) {

_Client_width=_Manager._Client_width;
_Client_height=_Manager._Client_height;
_Update_client=_Manager._Update_client;

	if (_Manager._Cursor)
		create_cursor(*_Manager._Cursor);
	
	if (_Manager._Hint)
		create_hint(_Manager._Hint->draw_order(),*_Manager._Hint);
	
	if (!_Manager._Mymenus.empty()) {
	
	ui_menu *_Menu;
	
		for (_Menus::const_iterator _Iter=_Manager._Mymenus.begin(),
			_End=_Manager._Mymenus.end();_Iter!=_End;++_Iter) {
		
		_Menu=(*_Iter); //Increase speed
		ui_hud *_Hud=dynamic_cast<ui_hud*>(_Menu);
		
			if (_Hud) create_hud(_Hud->_Name,_Hud->draw_order(),*_Hud);
			else create_menu(_Menu->_Name,_Menu->draw_order(),*_Menu);
		}
	}
}

void ui_manager::client_changed(float _Width,float _Height) {

	//Nothing to do
	if (_Mymenus.empty()) return;
	
	for (_Menus::const_iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter)
			(*_Iter)->client_changed(_Width,_Height);
}

//Menu specific

bool ui_manager::activate(ui_menu *_Menu,bool _Modal) {

	//Nothing to activate
	if (!_Menu) return false;

deactivate(_Menu);

	if (_Modal || _Myforms.empty()) {
	
	_Form _Myform;
	_Myform._Active_menu=_Menu;
	_Myform._Active_menus.push_back(_Menu);
	
	_Myforms.push_back(_Myform); //Activate
	}
	else {
	
	_Myforms.back()._Active_menus.push_back(_Menu);
	_Myforms.back()._Active_menu=_Menu; //Activate
	}

_Menu->enabled(true);
return true;
}

bool ui_manager::deactivate(ui_menu *_Menu) {

	//Nothing to deactivate
	if (!_Menu) return false;
	
	for (ui_manager::_Forms::reverse_iterator _Iter=_Myforms.rbegin(),
		_End=_Myforms.rend();_Iter!=_End;++_Iter) {
	
	_Form &_Myform=(*_Iter); //Increase speed
	
		for (_Menus::reverse_iterator _Aiter=_Myform._Active_menus.rbegin(),
			_Aend=_Myform._Active_menus.rend();_Aiter!=_Aend;++_Aiter) {
		
			//No duplicates allowed
			if ((*_Aiter)==_Menu) {
			
			_Myform._Active_menus.erase(--_Aiter.base());
			
				if (_Myform._Active_menus.empty())
					_Myforms.erase(--_Iter.base());
				else {
				
					if (_Myform._Active_menu==_Menu)
						_Myform._Active_menu=_Myform._Active_menus.front();
				}
			
			_Menu->enabled(false);
			return true;
			}
		}
	}

return false;
}

void ui_manager::deactivate() {

	while (!_Myforms.empty()) {
	
	_Form &_Myform=_Myforms.back();
	
		for (_Menus::reverse_iterator _Aiter=_Myform._Active_menus.rbegin(),
			_Aend=_Myform._Active_menus.rend();_Aiter!=_Aend;++_Aiter)
				(*_Aiter)->enabled(false);
	
	_Myforms.pop_back();
	}
}

void ui_manager::tab(bool _Forward) {

	//Nothing to do
	if (_Myforms.empty()) return;

_Form &_Myform=_Myforms.back();

	//Nothing more to activate
	if (_Myform._Active_menus.size()<2) return;

_Menus::const_iterator _Active=_Myform._Active_menus.end();

	for (_Menus::const_iterator _Iter=_Myform._Active_menus.begin(),
		_End=_Myform._Active_menus.end();_Iter!=_End;++_Iter) {
	
		//Active menu found, return iterator
		if ((*_Iter)==_Myform._Active_menu) {
		
		_Active=_Iter;
		break;
		}
	}

ui_menu *_Menu;

	for (_Menus::const_iterator _Iter=(_Forward?(_Active==_Myform._Active_menus.end()?
		_Myform._Active_menus.begin():++_Menus::const_iterator(_Active)):
			(_Active==_Myform._Active_menus.begin()?--_Myform._Active_menus.end():
				--_Menus::const_iterator(_Active)));_Iter!=_Active;) {
	
		//Forward cycle
		if (_Forward && _Iter==_Myform._Active_menus.end()) {
		
		_Iter=_Myform._Active_menus.begin();
		continue;
		}
		
		//Backward cycle
		if (!_Forward && _Iter==_Myform._Active_menus.end())
			_Iter=--_Myform._Active_menus.end();
	
	_Menu=(*_Iter); //Increase
	
		//Check if menu can be activated
		if (_Menu->_Enabled) {
		
		_Myform._Active_menu->clean();
		_Myform._Active_menu=_Menu;
		break;
		}
		
		//Backward cycle
		if (!_Forward && _Iter==_Myform._Active_menus.begin()) {
		
		_Iter=_Myform._Active_menus.end();
		continue;
		}
		
		//Check next/previous element
		if (_Forward) ++_Iter;
		else --_Iter;
	}
}

//Script specific

void ui_manager::init_entityscript(script_parser *_Script_parser) {

entity_manager::init_entityscript(_Script_parser); //Use base functionality
script_property *_Property;

//UI entity class

script_class *_Uientity_class=_Script_parser->create_class("ui_entity");
_Uientity_class->tag("ui");

_Property=_Uientity_class->create_property("size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Uientity_class->create_property("origin");
_Property->add_parameter("top_left,top_center,top_right,center_left,"
	"center,center_right,bottom_left,bottom_center,bottom_right");

_Property=_Uientity_class->create_property("positioning");
_Property->add_parameter("static,dynamic_x_axis,dynamic_y_axis,dynamic_xy_axis");

_Property=_Uientity_class->create_property("sizing");
_Property->add_parameter("static,dynamic_x_axis,dynamic_y_axis,dynamic_xy_axis");

_Property=_Uientity_class->create_property("positioning_unit");
_Property->add_parameter("pixel,percent");
_Property->add_parameter("pixel,percent");
_Property->required_parameters(1);

_Property=_Uientity_class->create_property("sizing_unit");
_Property->add_parameter("pixel,percent");
_Property->add_parameter("pixel,percent");
_Property->required_parameters(1);

_Property=_Uientity_class->create_property("positioning_area");
_Property->add_parameter(script_property::DT_VECTOR2);
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Uientity_class->create_property("auto_size");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Uientity_class->create_property("generate_mode");
_Property->add_parameter("force_off,modify_texcoords,rotate_node");

_Property=_Uientity_class->create_property("caption");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Uientity_class->create_property("caption_size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Uientity_class->create_property("caption_offset");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Uientity_class->create_property("caption_origin");
_Property->add_parameter("top_left,top_center,top_right,left,center,"
	"right,bottom_left,bottom_center,bottom_right,outside_top_left,"
		"outside_left,outside_bottom_left,outside_top_right,"
			"outside_right,outside_bottom_right");

//Hint class

script_class *_Hint_class=_Script_parser->create_class("hint");
_Hint_class->tag("ui");

_Property=_Hint_class->create_property("type");
_Property->add_parameter("fixed_size,dynamic_size");

_Property=_Hint_class->create_property("follow");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Hint_class->create_property("fadein_delay");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Hint_class->create_property("fadeout_delay");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Hint_class->create_property("fadein_time");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Hint_class->create_property("fadeout_time");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Hint_class->create_property("hold_time");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Hint_class->create_property("border_size");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Hint_class->create_property("offset_position");
_Property->add_parameter(script_property::DT_VECTOR2);

//Element class

script_class *_Element_class=_Script_parser->create_class("element");
_Element_class->tag("ui");

_Property=_Element_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Element_class->create_property("focused");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Element_class->create_property("focusable");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Element_class->create_property("tab_order");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Element_class->create_property("hint_caption");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Element_class->create_property("caption_color");
_Property->add_parameter(script_property::DT_COLOR);
_Property->add_parameter("normal,focus,over,press,disable");
_Property->required_parameters(1);

//Button class

script_class *_Button_class=_Script_parser->create_class("button");
_Button_class->tag("ui");

_Property=_Button_class->create_property("procedure");
_Property->add_parameter("show_menu,show_modal_menu,close_menu,"
	"enable_menu,disable_menu");
_Property->add_parameter(script_property::DT_STRING);

//Checkbox class

script_class *_Checkbox_class=_Script_parser->create_class("checkbox");
_Checkbox_class->tag("ui");

_Property=_Checkbox_class->create_property("checked");
_Property->add_parameter(script_property::DT_BOOL);

//Groupbox class

script_class *_Groupbox_class=_Script_parser->create_class("groupbox");
_Groupbox_class->tag("ui");

_Property=_Groupbox_class->create_property("element");
_Property->add_parameter(script_property::DT_STRING);
_Property->variable_parameters(true);

//Label class

script_class *_Label_class=_Script_parser->create_class("label");
_Label_class->tag("ui");

//Listbox class

script_class *_Listbox_class=_Script_parser->create_class("listbox");
_Listbox_class->tag("ui");

_Property=_Listbox_class->create_property("item");
_Property->add_parameter(script_property::DT_STRING);
_Property->add_parameter(script_property::DT_STRING);
_Property->required_parameters(1);

_Property=_Listbox_class->create_property("alignment");
_Property->add_parameter("left,center,right");

_Property=_Listbox_class->create_property("text_offset");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Listbox_class->create_property("item_index");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Listbox_class->create_property("fit_selection");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Listbox_class->create_property("icon_alignment");
_Property->add_parameter("left_side,right_side");

_Property=_Listbox_class->create_property("icon_positioning");
_Property->add_parameter("fixed,floating");

_Property=_Listbox_class->create_property("icon_width");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Listbox_class->create_property("icon_spacing");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Listbox_class->create_property("show_icons");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Listbox_class->create_property("repeat_rate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Listbox_class->create_property("repeat_delay");
_Property->add_parameter(script_property::DT_FLOAT);

//Progressbar class

script_class *_Progressbar_class=_Script_parser->create_class("progressbar");
_Progressbar_class->tag("ui");

_Property=_Progressbar_class->create_property("type");
_Property->add_parameter("none,horizontal,vertical,animation");

_Property=_Progressbar_class->create_property("range");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("progress");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("percent");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("flip");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Progressbar_class->create_property("locked");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Progressbar_class->create_property("snap_interval");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("interpolation");
_Property->add_parameter("forward,backward,bidirectional");

_Property=_Progressbar_class->create_property("interpolation_time");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("interpolation_latency");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Progressbar_class->create_property("caption_format");
_Property->add_parameter(script_property::DT_STRING);

//Radiobutton class

script_class *_Radiobutton_class=_Script_parser->create_class("radiobutton");
_Radiobutton_class->tag("ui");

_Property=_Radiobutton_class->create_property("tag");
_Property->add_parameter(script_property::DT_UINT32);

//Scrollbar class

script_class *_Scrollbar_class=_Script_parser->create_class("scrollbar");
_Scrollbar_class->tag("ui");

_Property=_Scrollbar_class->create_property("type");
_Property->add_parameter("vertical,horizontal");

_Property=_Scrollbar_class->create_property("range");
_Property->add_parameter(script_property::DT_UINT32);
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Scrollbar_class->create_property("position");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Scrollbar_class->create_property("bar_size");
_Property->add_parameter(script_property::DT_FLOAT);
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Scrollbar_class->create_property("scrollable");
_Property->add_parameter(script_property::DT_STRING);

//Scrollable class

script_class *_Scrollable_class=_Script_parser->create_class("scrollable");
_Scrollable_class->tag("ui");

_Property=_Scrollable_class->create_property("scroll_rate");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Scrollable_class->create_property("scrollbar");
_Property->add_parameter(script_property::DT_STRING);

//Textbox class

script_class *_Textbox_class=_Script_parser->create_class("textbox");
_Textbox_class->tag("ui");

_Property=_Textbox_class->create_property("mode");
_Property->add_parameter("alpha_numeric,alpha,numeric");

_Property=_Textbox_class->create_property("text_data");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Textbox_class->create_property("alignment");
_Property->add_parameter("left,center,right");

_Property=_Textbox_class->create_property("off_bound");
_Property->add_parameter("hide,trim");

_Property=_Textbox_class->create_property("text_offset");
_Property->add_parameter(script_property::DT_VECTOR2);

_Property=_Textbox_class->create_property("multiple_lines");
_Property->add_parameter(script_property::DT_BOOL);

_Property=_Textbox_class->create_property("max_characters");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Textbox_class->create_property("mask");
_Property->add_parameter(script_property::DT_STRING);

_Property=_Textbox_class->create_property("cursor_position");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Textbox_class->create_property("reveal_distance");
_Property->add_parameter(script_property::DT_UINT32);

_Property=_Textbox_class->create_property("repeat_rate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Textbox_class->create_property("repeat_delay");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Textbox_class->create_property("blink_rate");
_Property->add_parameter(script_property::DT_FLOAT);

_Property=_Textbox_class->create_property("blink_hold");
_Property->add_parameter(script_property::DT_FLOAT);

//Menu class

script_class *_Menu_class=_Script_parser->create_class("menu");
_Menu_class->tag("ui");

_Property=_Menu_class->create_property("enabled");
_Property->add_parameter(script_property::DT_BOOL);

//Hud class

script_class *_Hud_class=_Script_parser->create_class("hud");
_Hud_class->tag("ui");

//Region class

script_class *_Region_class=_Script_parser->create_class("region");
_Region_class->tag("ui");

_Property=_Region_class->create_property("mode");
_Property->add_parameter("horizontal,vertical");

_Property=_Region_class->create_property("element");
_Property->add_parameter(script_property::DT_STRING);
_Property->variable_parameters(true);

//Cursor class

script_class *_Cursor_class=_Script_parser->create_class("cursor");
_Cursor_class->tag("ui");

//Scopes

script_scope *_Element_scope=_Script_parser->create_scope("element");
_Element_scope->register_class(_Hint_class);
_Element_scope->register_class(_Element_class);
_Element_scope->register_class(_Button_class);
_Element_scope->register_class(_Checkbox_class);
_Element_scope->register_class(_Groupbox_class);
_Element_scope->register_class(_Label_class);
_Element_scope->register_class(_Listbox_class);
_Element_scope->register_class(_Progressbar_class);
_Element_scope->register_class(_Radiobutton_class);
_Element_scope->register_class(_Scrollbar_class);
_Element_scope->register_class(_Textbox_class);

script_scope *_Menu_scope=_Script_parser->create_scope("menu");
_Menu_scope->register_class(_Menu_class);
_Menu_scope->register_class(_Hud_class);
_Menu_scope->register_class(_Cursor_class);
_Menu_scope->register_class(_Hint_class);

script_scope *_Region_scope=_Script_parser->create_scope("region");
_Region_scope->register_class(_Region_class);

_Script_parser->register_scope(_Menu_scope);
_Menu_class->register_scope(_Element_scope);
_Hud_class->register_scope(_Region_scope);

//Inheritance

script_class *_Entity_class=_Script_parser->class_pointer("entity");

_Hint_class->register_inheritance(_Uientity_class);
_Button_class->register_inheritance(_Element_class);
_Checkbox_class->register_inheritance(_Element_class);
_Groupbox_class->register_inheritance(_Element_class);
_Label_class->register_inheritance(_Element_class);
_Progressbar_class->register_inheritance(_Element_class);
_Scrollbar_class->register_inheritance(_Element_class);
_Scrollable_class->register_inheritance(_Element_class);

_Listbox_class->register_inheritance(_Scrollable_class);
_Radiobutton_class->register_inheritance(_Checkbox_class);
_Textbox_class->register_inheritance(_Scrollable_class);
_Hud_class->register_inheritance(_Menu_class);

_Uientity_class->register_inheritance(_Entity_class);
_Element_class->register_inheritance(_Uientity_class);
_Menu_class->register_inheritance(_Uientity_class);
_Cursor_class->register_inheritance(_Entity_class);
}

bool ui_manager::read_object(entity *&_Entity,const script_class::object &_Object,
	const interval &_Draw_order,bool _Visible) {

	//Not correct object
	if (_Object.tag()!="ui") {
	
		//Switch to correct entity scope
		if (string_utility::compare(_Object.parent(),"menu") ||
			string_utility::compare(_Object.parent(),"hud")) {
		
			if (_Script_session[0] && _Entity!=_Script_session[0])
				_Entity=static_cast<entity*>(_Script_session[0]);
		}
	
	return false;
	}

ui_menu *_Menu=static_cast<ui_menu*>(_Script_session[0]); //Current menu
ui_element *_Element=static_cast<ui_element*>(_Script_session[1]); //Current element
ui_hud::region *_Region=static_cast<ui_hud::region*>(_Script_session[2]); //Current hud region

	//Cursor
	if (_Object=="cursor") {
	
	_Entity=create_cursor(_Visible); //Update entity
	
		//Skip object
		if (!_Entity) return true;
	}
	//Hint
	else if (_Object=="hint") {
	
	_Entity=0;
	
		//Update entity
		if (_Object.parent().empty())
			_Entity=create_hint(read_draworder(_Object),0.0f,0.0f);
		else if (_Menu)
			_Entity=_Menu->create_hint(read_draworder(_Object),0.0f,0.0f);
		
		//Skip object
		if (!_Entity) return true;
	}
	//Menu
	else if (_Object=="menu" || _Object=="hud") {
	
	_Menu=0;
	
		if (!_Object.name().empty()) {
		
			if (_Object=="hud")
				_Menu=create_hud(_Object.name(),_Draw_order+
					read_draworder(_Object),0.0f,0.0f,_Visible);
			else _Menu=create_menu(_Object.name(),_Draw_order+
				read_draworder(_Object),0.0f,0.0f,_Visible);
		}
	
	_Entity=_Menu; //Update entity
	_Script_session[0]=_Menu; //Update session
	
		//Skip object
		if (!_Menu) return true;
	}
	//Region
	else if (_Object=="region") {
	
	_Region=0;
	
		if (_Menu && !_Object.name().empty()) {
		
		ui_groupbox *_Groupbox=dynamic_cast<ui_groupbox*>(
			_Menu->element_pointer(_Object.name()));
		
			if (_Groupbox) {
			
			ui_hud *_Hud=static_cast<ui_hud*>(_Menu);
			_Region=_Hud->create_region(_Groupbox);
			}
		}
	
	_Script_session[2]=_Region; //Update session
	
		//Skip object
		if (!_Region) return true;
	}
	//Element
	else {
	
	_Element=0;
	
		if (_Menu && !_Object.name().empty()) {
		
			//Element
			if (_Object=="element")
				_Element=_Menu->create_element(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Button
			else if (_Object=="button")
				_Element=_Menu->create_button(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Checkbox
			else if (_Object=="checkbox")
				_Element=_Menu->create_checkbox(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Groupbox
			else if (_Object=="groupbox")
				_Element=_Menu->create_groupbox(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Label
			else if (_Object=="label")
				_Element=_Menu->create_label(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Listbox
			else if (_Object=="listbox")
				_Element=_Menu->create_listbox(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Progressbar
			else if (_Object=="progressbar")
				_Element=_Menu->create_progressbar(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Radiobutton
			else if (_Object=="radiobutton")
				_Element=_Menu->create_radiobutton(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Scrollbar
			else if (_Object=="scrollbar")
				_Element=_Menu->create_scrollbar(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
			//Textbox
			else if (_Object=="textbox")
				_Element=_Menu->create_textbox(_Object.name(),
					read_draworder(_Object),0.0f,0.0f,_Visible);
		}
	
	_Entity=_Element; //Update entity
	_Script_session[1]=_Element; //Update session
	
		//Skip object
		if (!_Element) return true;
	}
	
	//Set properties
	for (script_class::object::data_iterator _Diter=_Object.data_begin(),
		_Dend=_Object.data_end();_Diter!=_Dend;++_Diter) {
	
	const script_property::data &_Data=(*_Diter); //Increase speed
	
		//Region properties
		if (_Object=="region") {
		
			if (_Data=="mode") {
			
				if (string_utility::compare(_Data.value(0),"horizontal"))
					_Region->mode(ui_hud::region::HORIZONTAL);
				else if (string_utility::compare(_Data.value(0),"vertical"))
					_Region->mode(ui_hud::region::VERTICAL);
			}
			else if (_Data=="element") {
			
			unsigned int _Size=_Data.values();
			
				for (unsigned int i=0;i<_Size;++i) {
				
				ui_groupbox *_Groupbox=dynamic_cast<ui_groupbox*>(
					_Menu->element_pointer(_Data.value(i)));
				
					if (_Groupbox)
						_Region->add_element(_Groupbox);
				}
			}
		}
		//UI entity properties
		else {
		
		//Entity properties
		entity_properties(_Entity,_Data);
		
		ui_entity *_Ui_entity=static_cast<ui_entity*>(_Entity);
		
			//Shared properties
			if (_Data=="size")
				_Ui_entity->size(_Data.value<float>(0),_Data.value<float>(1));
			else if (_Data=="origin") {
			
				if (string_utility::compare(_Data.value(0),"top_left"))
					_Ui_entity->area()->origin(rectangle::TOP_LEFT);
				else if (string_utility::compare(_Data.value(0),"top_center"))
					_Ui_entity->area()->origin(rectangle::TOP_CENTER);
				else if (string_utility::compare(_Data.value(0),"top_right"))
					_Ui_entity->area()->origin(rectangle::TOP_RIGHT);
				else if (string_utility::compare(_Data.value(0),"center_left"))
					_Ui_entity->area()->origin(rectangle::CENTER_LEFT);
				else if (string_utility::compare(_Data.value(0),"center"))
					_Ui_entity->area()->origin(rectangle::CENTER);
				else if (string_utility::compare(_Data.value(0),"center_right"))
					_Ui_entity->area()->origin(rectangle::CENTER_RIGHT);
				else if (string_utility::compare(_Data.value(0),"bottom_left"))
					_Ui_entity->area()->origin(rectangle::BOTTOM_LEFT);
				else if (string_utility::compare(_Data.value(0),"bottom_center"))
					_Ui_entity->area()->origin(rectangle::BOTTOM_CENTER);
				else if (string_utility::compare(_Data.value(0),"bottom_right"))
					_Ui_entity->area()->origin(rectangle::BOTTOM_RIGHT);
			}
			else if (_Data=="positioning") {
			
				if (string_utility::compare(_Data.value(0),"static"))
					_Ui_entity->positioning(ui_entity::STATIC);
				else if (string_utility::compare(_Data.value(0),"dynamic_x_axis"))
					_Ui_entity->positioning(ui_entity::DYNAMIC_X_AXIS);
				else if (string_utility::compare(_Data.value(0),"dynamic_y_axis"))
					_Ui_entity->positioning(ui_entity::DYNAMIC_Y_AXIS);
				else if (string_utility::compare(_Data.value(0),"dynamic_xy_axis"))
					_Ui_entity->positioning(ui_entity::DYNAMIC_XY_AXIS);
			}
			else if (_Data=="sizing") {
			
				if (string_utility::compare(_Data.value(0),"static"))
					_Ui_entity->sizing(ui_entity::STATIC);
				else if (string_utility::compare(_Data.value(0),"dynamic_x_axis"))
					_Ui_entity->sizing(ui_entity::DYNAMIC_X_AXIS);
				else if (string_utility::compare(_Data.value(0),"dynamic_y_axis"))
					_Ui_entity->sizing(ui_entity::DYNAMIC_Y_AXIS);
				else if (string_utility::compare(_Data.value(0),"dynamic_xy_axis"))
					_Ui_entity->sizing(ui_entity::DYNAMIC_XY_AXIS);
			}
			else if (_Data=="positioning_unit") {
			
			ui_entity::LAYOUT_UNIT _X=(string_utility::compare(
				_Data.value(0),"percent")?ui_entity::PERCENT:ui_entity::PIXEL);
			ui_entity::LAYOUT_UNIT _Y=(_Data.values()<2?_X:string_utility::compare(
				_Data.value(1),"percent")?ui_entity::PERCENT:ui_entity::PIXEL);
			
			_Ui_entity->positioning_unit(_X,_Y);
			}
			else if (_Data=="sizing_unit") {
			
			ui_entity::LAYOUT_UNIT _X=(string_utility::compare(
				_Data.value(0),"percent")?ui_entity::PERCENT:ui_entity::PIXEL);
			ui_entity::LAYOUT_UNIT _Y=(_Data.values()<2?_X:string_utility::compare(
				_Data.value(1),"percent")?ui_entity::PERCENT:ui_entity::PIXEL);
			
			_Ui_entity->sizing_unit(_X,_Y);
			}
			else if (_Data=="positioning_area")
				_Ui_entity->positioning_area(aabb(_Data.value<vector2>(0),
					_Data.value<vector2>(1)));
			else if (_Data=="auto_size")
				_Ui_entity->auto_size(_Data.value<bool>(0));
			else if (_Data=="generate_mode") {
			
				if (string_utility::compare(_Data.value(0),"force_off"))
					_Ui_entity->generate_mode(ui_entity::FORCE_OFF);
				else if (string_utility::compare(_Data.value(0),"modify_texcoords"))
					_Ui_entity->generate_mode(ui_entity::MODIFY_TEXCOORDS);
				else if (string_utility::compare(_Data.value(0),"rotate_node"))
					_Ui_entity->generate_mode(ui_entity::ROTATE_NODE);
			}
			else if (_Data=="caption")
				_Ui_entity->caption(_Data.value(0));
			else if (_Data=="caption_size")
				_Ui_entity->caption_size(_Data.value<float>(0),_Data.value<float>(1));
			else if (_Data=="caption_offset")
				_Ui_entity->caption_offset(_Data.value<vector2>(0));
			else if (_Data=="caption_origin") {
			
				if (string_utility::compare(_Data.value(0),"top_left"))
					_Ui_entity->caption_origin(ui_entity::TOP_LEFT);
				else if (string_utility::compare(_Data.value(0),"top_center"))
					_Ui_entity->caption_origin(ui_entity::TOP_CENTER);
				else if (string_utility::compare(_Data.value(0),"top_right"))
					_Ui_entity->caption_origin(ui_entity::TOP_RIGHT);
				else if (string_utility::compare(_Data.value(0),"left"))
					_Ui_entity->caption_origin(ui_entity::LEFT);
				else if (string_utility::compare(_Data.value(0),"center"))
					_Ui_entity->caption_origin(ui_entity::CENTER);
				else if (string_utility::compare(_Data.value(0),"right"))
					_Ui_entity->caption_origin(ui_entity::RIGHT);
				else if (string_utility::compare(_Data.value(0),"bottom_left"))
					_Ui_entity->caption_origin(ui_entity::BOTTOM_LEFT);
				else if (string_utility::compare(_Data.value(0),"bottom_center"))
					_Ui_entity->caption_origin(ui_entity::BOTTOM_CENTER);
				else if (string_utility::compare(_Data.value(0),"bottom_right"))
					_Ui_entity->caption_origin(ui_entity::BOTTOM_RIGHT);
				else if (string_utility::compare(_Data.value(0),"outside_top_left"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_TOP_LEFT);
				else if (string_utility::compare(_Data.value(0),"outside_left"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_LEFT);
				else if (string_utility::compare(_Data.value(0),"outside_bottom_left"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_BOTTOM_LEFT);
				else if (string_utility::compare(_Data.value(0),"outside_top_right"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_TOP_RIGHT);
				else if (string_utility::compare(_Data.value(0),"outside_right"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_RIGHT);
				else if (string_utility::compare(_Data.value(0),"outside_bottom_right"))
					_Ui_entity->caption_origin(ui_entity::OUTSIDE_BOTTOM_RIGHT);
			}
			
			//Hint properties
			else if (_Object=="hint") {
			
			ui_hint *_Hint=static_cast<ui_hint*>(_Entity);
			
				if (_Data=="type") {
				
					if (string_utility::compare(_Data.value(0),"fixed_size"))
						_Hint->type(ui_hint::FIXED_SIZE);
					else if (string_utility::compare(_Data.value(0),"dynamic_size"))
						_Hint->type(ui_hint::DYNAMIC_SIZE);
				}
				else if (_Data=="follow")
					_Hint->follow(_Data.value<bool>(0));
				else if (_Data=="fadein_delay")
					_Hint->fadein_delay(_Data.value<float>(0));
				else if (_Data=="fadeout_delay")
					_Hint->fadeout_delay(_Data.value<float>(0));
				else if (_Data=="fadein_time")
					_Hint->fadein_time(_Data.value<float>(0));
				else if (_Data=="fadeout_time")
					_Hint->fadeout_time(_Data.value<float>(0));
				else if (_Data=="hold_time")
					_Hint->hold_time(_Data.value<float>(0));
				else if (_Data=="border_size")
					_Hint->border_size(_Data.value<vector2>(0));
				else if (_Data=="offset_position")
					_Hint->offset_position(_Data.value<vector2>(0));
			}
			//Menu properties
			else if (_Object=="menu" || _Object=="hud") {
			
				//Shared properties
				if (_Data=="enabled")
					_Menu->enabled(_Data.value<bool>(0));
				
				//Hud properties
				else if (_Object=="hud") {
					//Empty
				}
			}
			//Element properties
			else {
			
				//Shared properties
				if (_Data=="enabled")
					_Element->enabled(_Data.value<bool>(0));
				else if (_Data=="focused")
					_Element->focused(_Data.value<bool>(0));
				else if (_Data=="focusable")
					_Element->focusable(_Data.value<bool>(0));
				else if (_Data=="tab_order")
					_Element->tab_order(_Data.value<unsigned int>(0));
				else if (_Data=="hint_caption")
					_Element->hint_caption(_Data.value(0));
				else if (_Data=="caption_color") {
				
					if (_Data.values()>1) {
					
						if (string_utility::compare(_Data.value(1),"normal"))
							_Element->caption_color(_Data.value<color>(0),ui_element::NORMAL);
						else if (string_utility::compare(_Data.value(1),"focus"))
							_Element->caption_color(_Data.value<color>(0),ui_element::FOCUS);
						else if (string_utility::compare(_Data.value(1),"over"))
							_Element->caption_color(_Data.value<color>(0),ui_element::OVER);
						else if (string_utility::compare(_Data.value(1),"press"))
							_Element->caption_color(_Data.value<color>(0),ui_element::PRESS);
						else if (string_utility::compare(_Data.value(1),"disable"))
							_Element->caption_color(_Data.value<color>(0),ui_element::DISABLE);
					}
					else _Element->caption_color(_Data.value<color>(0));
				}
				
				//Button properties
				else if (_Object=="button") {
				
				ui_button *_Button=static_cast<ui_button*>(_Element);
				
					if (_Data=="procedure") {
					
						if (string_utility::compare(_Data.value(0),"show_menu"))
							_Button->add_procedure(ui_button::SHOW_MENU,_Data.value(1));
						else if (string_utility::compare(_Data.value(0),"show_modal_menu"))
							_Button->add_procedure(ui_button::SHOW_MODAL_MENU,_Data.value(1));
						else if (string_utility::compare(_Data.value(0),"close_menu"))
							_Button->add_procedure(ui_button::CLOSE_MENU,_Data.value(1));
						else if (string_utility::compare(_Data.value(0),"enable_menu"))
							_Button->add_procedure(ui_button::ENABLE_MENU,_Data.value(1));
						else if (string_utility::compare(_Data.value(0),"disable_menu"))
							_Button->add_procedure(ui_button::DISABLE_MENU,_Data.value(1));
					}
				}
				//Checkbox properties
				else if (_Object=="checkbox" || _Object=="radiobutton") {
				
				ui_checkbox *_Checkbox=static_cast<ui_checkbox*>(_Element);
				
					//Shared properties
					if (_Data=="checked")
						_Checkbox->checked(_Data.value<bool>(0));
					
					//Radiobutton properties
					if (_Object=="radiobutton") {
					
					ui_radiobutton *_Radiobutton=static_cast<ui_radiobutton*>(_Checkbox);
					
						if (_Data=="tag")
							_Radiobutton->tag(_Data.value<unsigned int>(0));
					}
				}
				//Groupbox properties
				else if (_Object=="groupbox") {
				
				ui_groupbox *_Groupbox=static_cast<ui_groupbox*>(_Element);
				
					if (_Data=="element") {
					
					unsigned int _Size=_Data.values();
					
						for (unsigned int i=0;i<_Size;++i) {
						
						ui_element *_Group_element=_Menu->element_pointer(
							_Data.value(i));
						
							if (_Group_element)
								_Groupbox->add_element(_Group_element);
						}
					}
				}
				//Label properties
				else if (_Object=="label") {
					//Empty
				}
				//Progressbar properties
				else if (_Object=="progressbar") {
				
				ui_progressbar *_Progressbar=static_cast<ui_progressbar*>(_Element);
				
					if (_Data=="type") {
					
						if (string_utility::compare(_Data.value(0),"none"))
							_Progressbar->type(ui_progressbar::NONE);
						else if (string_utility::compare(_Data.value(0),"horizontal"))
							_Progressbar->type(ui_progressbar::HORIZONTAL);
						else if (string_utility::compare(_Data.value(0),"vertical"))
							_Progressbar->type(ui_progressbar::VERTICAL);
						else if (string_utility::compare(_Data.value(0),"animation"))
							_Progressbar->type(ui_progressbar::ANIMATION);
					}
					else if (_Data=="range")
						_Progressbar->range(_Data.value<float>(0),_Data.value<float>(1));
					else if (_Data=="progress")
						_Progressbar->progress(_Data.value<float>(0));
					else if (_Data=="percent")
						_Progressbar->percent(_Data.value<float>(0));
					else if (_Data=="flip")
						_Progressbar->flip(_Data.value<bool>(0));
					else if (_Data=="locked")
						_Progressbar->locked(_Data.value<bool>(0));
					else if (_Data=="snap_interval")
						_Progressbar->snap_interval(_Data.value<float>(0));
					else if (_Data=="interpolation") {
					
						if (string_utility::compare(_Data.value(0),"forward"))
							_Progressbar->interpolation(ui_progressbar::FORWARD);
						else if (string_utility::compare(_Data.value(0),"backward"))
							_Progressbar->interpolation(ui_progressbar::BACKWARD);
						else if (string_utility::compare(_Data.value(0),"bidirectional"))
							_Progressbar->interpolation(ui_progressbar::BIDIRECTIONAL);
					}
					else if (_Data=="interpolation_time")
						_Progressbar->interpolation_time(_Data.value<float>(0));
					else if (_Data=="interpolation_latency")
						_Progressbar->interpolation_latency(_Data.value<float>(0));
					else if (_Data=="caption_format")
						_Progressbar->caption_format(_Data.value(0));
				}
				//Scrollbar properties
				else if (_Object=="scrollbar") {
				
				ui_scrollbar *_Scrollbar=static_cast<ui_scrollbar*>(_Element);
				
					if (_Data=="type") {
					
						if (string_utility::compare(_Data.value(0),"vertical"))
							_Scrollbar->type(ui_scrollbar::VERTICAL);
						else if (string_utility::compare(_Data.value(0),"horizontal"))
							_Scrollbar->type(ui_scrollbar::HORIZONTAL);
					}
					else if (_Data=="range")
						_Scrollbar->range(_Data.value<unsigned int>(0),
							_Data.value<unsigned int>(1));
					else if (_Data=="position")
						_Scrollbar->position(_Data.value<unsigned int>(0));
					else if (_Data=="bar_size")
						_Scrollbar->bar_size(_Data.value<float>(0),_Data.value<float>(1));
					else if (_Data=="scrollable") {
					
					ui_scrollable *_Scrollable=dynamic_cast<ui_scrollable*>(
						_Menu->element_pointer(_Data.value(0)));
					
						if (_Scrollable)
							_Scrollbar->attach_scrollable(_Scrollable);
					}
				}
				//Scrollable properties
				else if (_Object=="scrollable" ||
					_Object=="listbox" || _Object=="textbox") {
				
				ui_scrollable *_Scrollable=static_cast<ui_scrollable*>(_Element);
				
					//Shared properties
					if (_Data=="scroll_rate")
						_Scrollable->scroll_rate(_Data.value<unsigned int>(0));
					else if (_Data=="scrollbar") {
					
					ui_scrollbar *_Scrollbar=dynamic_cast<ui_scrollbar*>(
						_Menu->element_pointer(_Data.value(0)));
					
						if (_Scrollbar)
							_Scrollable->attach_scrollbar(_Scrollbar);
					}
					
					//Listbox properties
					else if (_Object=="listbox") {
					
					ui_listbox *_Listbox=static_cast<ui_listbox*>(_Element);
					
						if (_Data=="item") {
						
						ui_listbox::item _Item;
						_Item.data=_Data.value(0);
						
							if (_Data.values()>1)
								_Item.icon=_Data.value(1);
						
						_Listbox->add_item(_Item);
						}
						else if (_Data=="alignment") {
						
							if (string_utility::compare(_Data.value(0),"left"))
								_Listbox->alignment(ui_listbox::LEFT);
							else if (string_utility::compare(_Data.value(0),"center"))
								_Listbox->alignment(ui_listbox::CENTER);
							else if (string_utility::compare(_Data.value(0),"right"))
								_Listbox->alignment(ui_listbox::RIGHT);
						}
						else if (_Data=="text_offset")
							_Listbox->text_offset(_Data.value<vector2>(0));
						else if (_Data=="item_index")
							_Listbox->item_index(_Data.value<unsigned int>(0));
						else if (_Data=="fit_selection")
							_Listbox->fit_selection(_Data.value<bool>(0));
						else if (_Data=="icon_alignment") {
						
							if (string_utility::compare(_Data.value(0),"left_side"))
								_Listbox->icon_alignment(ui_listbox::LEFT_SIDE);
							else if (string_utility::compare(_Data.value(0),"right_side"))
								_Listbox->icon_alignment(ui_listbox::RIGHT_SIDE);
						}
						else if (_Data=="icon_positioning") {
						
							if (string_utility::compare(_Data.value(0),"fixed"))
								_Listbox->icon_positioning(ui_listbox::FIXED);
							else if (string_utility::compare(_Data.value(0),"floating"))
								_Listbox->icon_positioning(ui_listbox::FLOATING);
						}
						else if (_Data=="icon_width")
							_Listbox->icon_width(_Data.value<float>(0));
						else if (_Data=="icon_spacing")
							_Listbox->icon_spacing(_Data.value<float>(0));
						else if (_Data=="show_icons")
							_Listbox->show_icons(_Data.value<bool>(0));
						else if (_Data=="repeat_rate")
							_Listbox->repeat_rate(_Data.value<float>(0));
						else if (_Data=="repeat_delay")
							_Listbox->repeat_delay(_Data.value<float>(0));
					}
					//Textbox properties
					else if (_Object=="textbox") {
					
					ui_textbox *_Textbox=static_cast<ui_textbox*>(_Element);
					
						if (_Data=="mode") {
						
							if (string_utility::compare(_Data.value(0),"alpha_numeric"))
								_Textbox->mode(ui_textbox::ALPHA_NUMERIC);
							else if (string_utility::compare(_Data.value(0),"alpha"))
								_Textbox->mode(ui_textbox::ALPHA);
							else if (string_utility::compare(_Data.value(0),"numeric"))
								_Textbox->mode(ui_textbox::NUMERIC);
						}
						else if (_Data=="text_data")
							_Textbox->text_data(_Data.value(0));
						else if (_Data=="alignment") {
						
							if (string_utility::compare(_Data.value(0),"left"))
								_Textbox->alignment(ui_textbox::LEFT);
							else if (string_utility::compare(_Data.value(0),"center"))
								_Textbox->alignment(ui_textbox::CENTER);
							else if (string_utility::compare(_Data.value(0),"right"))
								_Textbox->alignment(ui_textbox::RIGHT);
						}
						else if (_Data=="off_bound") {
						
							if (string_utility::compare(_Data.value(0),"hide"))
								_Textbox->off_bound(ui_textbox::HIDE);
							else if (string_utility::compare(_Data.value(0),"trim"))
								_Textbox->off_bound(ui_textbox::TRIM);
						}
						else if (_Data=="text_offset")
							_Textbox->text_offset(_Data.value<vector2>(0));
						else if (_Data=="multiple_lines")
							_Textbox->multiple_lines(_Data.value<bool>(0));
						else if (_Data=="max_characters")
							_Textbox->max_characters(_Data.value<unsigned int>(0));
						else if (_Data=="mask")
							_Textbox->mask(!_Data.value(0).empty()?_Data.value(0)[0]:0);
						else if (_Data=="cursor_position")
							_Textbox->cursor_position(_Data.value<unsigned int>(0));
						else if (_Data=="reveal_distance")
							_Textbox->reveal_distance(_Data.value<unsigned int>(0));
						else if (_Data=="repeat_rate")
							_Textbox->repeat_rate(_Data.value<float>(0));
						else if (_Data=="repeat_delay")
							_Textbox->repeat_delay(_Data.value<float>(0));
						else if (_Data=="blink_rate")
							_Textbox->blink_rate(_Data.value<float>(0));
						else if (_Data=="blink_hold")
							_Textbox->blink_hold(_Data.value<float>(0));
					}
				}
			}
		}
	}

return true;
}

ui_menu *ui_manager::init_ui(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {

//Script session
//1. Current menu
//2. Current element
//3. Current hud region
_Script_session=new void*[3];
_Script_session[0]=0;
_Script_session[1]=0;
_Script_session[2]=0;

init_entity(_Script_data,_Draw_order,_Visible);

ui_menu *_Menu=static_cast<ui_menu*>(_Script_session[0]);
delete[] _Script_session;
_Script_session=0;

return _Menu;
}

//States

std::string ui_manager::cursor_statename(const CURSOR_STATE &_Cursor_state) {

std::string _Name;

	switch (_Cursor_state) {
	
		case OVER:
		_Name=cursor::state::OVER;
		break;
		
		case PRESS:
		_Name=cursor::state::PRESS;
		break;
		
		default:
		_Name=cursor::state::NORMAL;
		break;
	};

return _Name;
}

void ui_manager::cursor_state(const CURSOR_STATE &_Cursor_state) {

	//Nothing to do
	if (this->_Cursor_state==_Cursor_state) return;

object *_Object=_Cursor->object_pointer(
	cursor_statename(this->_Cursor_state));

	//Use as default
	if (!_Object) _Object=_Cursor->object_pointer(
		cursor_statename(NORMAL));
	
	//Hide previous state
	if (_Object) _Cursor->visible(_Object,false,true);

_Object=_Cursor->object_pointer(
	cursor_statename(_Cursor_state));

	//Use as default
	if (!_Object) _Object=_Cursor->object_pointer(
		cursor_statename(NORMAL));
	
	//Show new state
	if (_Object) _Cursor->visible(_Object,true,true);

this->_Cursor_state=_Cursor_state;
}

//Public

ui_manager::ui_manager(render_node *_Parent_node,int _Draw_order,
	unsigned int _Client_width,unsigned int _Client_height):
		entity_manager(_Parent_node,_Draw_order),_Client_width(_Client_width),
			_Client_height(_Client_height),_Cursor(0),_Cursor_state(NORMAL),
				_Hint(0),_Update_client(true),_Script_session(0) {
	render_window::instance()->mouse_visible(true); //Show system cursor
}

ui_manager::ui_manager(const ui_manager &_Manager):
	entity_manager(_Manager),_Cursor(0),_Hint(0),_Script_session(0) {

_Cursor_state=NORMAL; //Not copyable
copy(_Manager);

	//Show system cursor
	if (!_Cursor) render_window::instance()->mouse_visible(true);
}

ui_manager::~ui_manager() {

clear_menus();
remove_hint();
remove_cursor();
}

ui_manager &ui_manager::operator=(const ui_manager &_Manager) {

	//Worth doing
	if (this!=&_Manager) {
	
	remove_cursor();
	remove_hint();
	clear_menus();
	
	entity_manager::operator=(_Manager); //Invoke base operator=
	copy(_Manager);
	}

return *this;
}

void ui_manager::update(float _Time) {

	if (_Update_client) {
	
	client_changed(render_window::instance()->ortho_width(),
		render_window::instance()->ortho_height());
	_Update_client=false;
	}
	
	if (_Cursor) _Cursor->update(_Time);
	if (_Hint) _Hint->update(_Time);
	
	for (_Menus::const_iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter)
			(*_Iter)->update(_Time);
}

void ui_manager::post_update(float _Time) {

	for (_Menus::const_iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter)
			(*_Iter)->post_update(_Time);
}

bool ui_manager::message(HWND _Window_handle,UINT _Message,
	WPARAM _Wparam,LPARAM _Lparam) {

	switch (_Message) {
	
		case message_listener::WM_GLSIZE: {
		
		unsigned int _Glwidth=LOWORD(_Lparam);
		unsigned int _Glheight=HIWORD(_Lparam);
		
			//Ignore minimize (zero size)
			if (_Glwidth!=0 && _Glheight!=0) {
			
			client_changed((float)_Glwidth,(float)_Glheight);
			_Update_client=false;
			}
		
		return true;
		}
	}

return false;
}

void ui_manager::key_pressed(unsigned int _Key) {

	if (!_Myforms.empty())
		_Myforms.back()._Active_menu->key_pressed(_Key);
}

void ui_manager::key_released(unsigned int _Key) {

	if (!_Myforms.empty())
		_Myforms.back()._Active_menu->key_released(_Key);
}

void ui_manager::char_pressed(char _Character) {

	if (!_Myforms.empty())
		_Myforms.back()._Active_menu->char_pressed(_Character);
}

void ui_manager::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	if (!_Myforms.empty())
		_Myforms.back()._Active_menu->mouse_pressed(_Button,_X,_Y);
	
	//Change cursor state
	if (_Cursor && _Cursor_state!=PRESS)
		cursor_state(PRESS);
}

void ui_manager::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	if (!_Myforms.empty()) {
	
		if (_Myforms.back()._Active_menu->mouse_released(_Button,_X,_Y)) {
		
			//Change cursor state
			if (_Cursor && _Cursor_state==PRESS)
				cursor_state(OVER);
		}
		else {
		
			//Change cursor state
			if (_Cursor && _Cursor_state==PRESS)
				cursor_state(NORMAL);
		}
	}
}

void ui_manager::mouse_moved(unsigned int _X,unsigned int _Y) {

	if (_Cursor) _Cursor->node()->position(
		vector2((float)_X,(float)_Y));

bool _Result=false;

	if (!_Myforms.empty()) {
	
	_Form &_Myform=_Myforms.back();
	ui_menu *_Menu=_Myform._Active_menu;
	
	_Result=_Menu->mouse_moved(_X,_Y);
	
		if (!_Menu->_Over_element &&
			_Myform._Active_menus.size()>1) {
		
			for (_Menus::iterator _Iter=_Myform._Active_menus.begin(),
				_End=_Myform._Active_menus.end();_Iter!=_End;++_Iter) {
			
			_Menu=(*_Iter); //Increase speed
			
				//Skip active menu
				if (_Menu==_Myform._Active_menu) continue;
			
			_Result=_Menu->mouse_moved(_X,_Y);
			
				//Check if menu can be activated
				if (_Menu->_Over_element) {
				
				_Myform._Active_menu->clean();
				_Myform._Active_menu=_Menu;
				break;
				}
			}
		}
	}
	
	if (_Result) {
	
		//Change cursor state
		if (_Cursor && _Cursor_state==NORMAL)
			cursor_state(OVER);
	}
	else {
	
		//Change cursor state
		if (_Cursor && _Cursor_state==OVER)
			cursor_state(NORMAL);
	}
}

void ui_manager::mouse_scrolled(int _Delta,unsigned int _X,
	unsigned int _Y) {

	if (!_Myforms.empty())
		_Myforms.back()._Active_menu->mouse_scrolled(_Delta,_X,_Y);
}

void ui_manager::client_size(unsigned int _Client_width,
	unsigned int _Client_height) {

	if (this->_Client_width!=_Client_width ||
		this->_Client_height!=_Client_height) {
	
	this->_Client_width=_Client_width;
	this->_Client_height=_Client_height;
	_Update_client=true;
	}
}

unsigned int ui_manager::client_width() const {
	return _Client_width;
}

unsigned int ui_manager::client_height() const {
	return _Client_height;
}

//Cursor

entity *ui_manager::create_cursor(bool _Visible) {

	if (!_Cursor) {
	
	_Cursor=create_entity(std::numeric_limits<int>::max()-
		_Node->draw_order()-1000,_Visible); //Default
	_Cursor->node()->position(vector2((float)input_manager::instance()->mouse_x(),
		(float)input_manager::instance()->mouse_y()));
	
	render_window::instance()->mouse_visible(false); //Hide system cursor
	}

return _Cursor;
}

entity *ui_manager::create_cursor(const entity &_Entity) {

	if (!_Cursor) {
	
	_Cursor=create_entity(std::numeric_limits<int>::max()-
		_Node->draw_order()-1000,_Entity); //Default
	_Cursor->node()->position(vector2((float)input_manager::instance()->mouse_x(),
		(float)input_manager::instance()->mouse_y()));
	
	render_window::instance()->mouse_visible(false); //Hide system cursor
	}

return _Cursor;
}

void ui_manager::remove_cursor() {

	if (_Cursor) {
	
	remove_entity(_Cursor);
	render_window::instance()->mouse_visible(true); //Show system cursor
	}
}

entity *ui_manager::cursor_pointer() const {
	return _Cursor;
}

//Hint

ui_hint *ui_manager::create_hint(const interval &_Draw_order,
	float _Width,float _Height) {

	if (!_Hint)
		_Hint=new ui_hint(this,_Node,_Draw_order,_Width,_Height);

return _Hint;
}

ui_hint *ui_manager::create_hint(const interval &_Draw_order,
	const ui_hint &_Hint) {

	if (!this->_Hint)
		this->_Hint=new ui_hint(this,_Node,_Draw_order,_Hint);

return this->_Hint;
}

void ui_manager::remove_hint() {

	if (_Hint) {
	
	delete _Hint;
	_Hint=0;
	}
}

ui_hint *ui_manager::hint_pointer() const {
	return _Hint;
}

//UI

ui_menu *ui_manager::create_ui(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {

	if (!_Script_data) return 0;

return init_ui(_Script_data,_Draw_order,_Visible);
}

//Menus
//Menu

ui_menu *ui_manager::create_menu(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Menu already exist
	if (menu_pointer(_Name)) return 0;

ui_menu *_Mymenu=new ui_menu(this,_Name,_Node,
	_Draw_order,_Width,_Height,_Visible);
_Mymenus.push_back(_Mymenu);

	if (_Visible) activate(_Mymenu);

return _Mymenu;
}

ui_menu *ui_manager::create_menu(const std::string &_Name,
	const interval &_Draw_order,const ui_menu &_Menu) {

	//Menu already exist
	if (menu_pointer(_Name)) return 0;

ui_menu *_Mymenu=new ui_menu(this,_Name,_Node,
	_Draw_order,_Menu);
_Mymenus.push_back(_Mymenu);

	if (_Menu.visible()) activate(_Mymenu);

return _Mymenu;
}

ui_menu *ui_manager::create_menu(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {
	return create_ui(_Script_data,_Draw_order,_Visible);
}

void ui_manager::create_menus(file_data *_Script_data,
	const interval &_Draw_order,bool _Visible) {
	create_ui(_Script_data,_Draw_order,_Visible);
}

//Hud

ui_hud *ui_manager::create_hud(const std::string &_Name,
	const interval &_Draw_order,float _Width,float _Height,
		bool _Visible) {

	//Menu already exist
	if (menu_pointer(_Name)) return 0;

ui_hud *_Myhud=new ui_hud(this,_Name,_Node,
	_Draw_order,_Width,_Height,_Visible);
_Mymenus.push_back(_Myhud);

	if (_Visible) activate(_Myhud);

return _Myhud;
}

ui_hud *ui_manager::create_hud(const std::string &_Name,
	const interval &_Draw_order,const ui_hud &_Hud) {

	//Menu already exist
	if (menu_pointer(_Name)) return 0;

ui_hud *_Myhud=new ui_hud(this,_Name,_Node,
	_Draw_order,_Hud);
_Mymenus.push_back(_Myhud);

	if (_Hud.visible()) activate(_Myhud);

return _Myhud;
}

void ui_manager::remove_menu(ui_menu *&_Menu) {

	for (_Menus::iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Menu) {
		
		deactivate(_Menu);
		delete _Menu;
		_Menu=0; //Set referenced pointer to null
		
		_Mymenus.erase(_Iter);
		return;
		}
	}
}

void ui_manager::clear_menus() {

deactivate();

	for (_Menus::const_iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter)
			delete (*_Iter);

_Mymenus.clear();
}

ui_manager::menus_iterator ui_manager::menus_begin() const {
	return _Mymenus.begin();
}

ui_manager::menus_iterator ui_manager::menus_end() const {
	return _Mymenus.end();
}

ui_menu *ui_manager::menu_pointer(const std::string &_Name) const {

	for (_Menus::const_iterator _Iter=_Mymenus.begin(),
		_End=_Mymenus.end();_Iter!=_End;++_Iter) {
	
		//Menu exist, return pointer
		if ((*_Iter)->_Name==_Name) return (*_Iter);
	}

return 0;
}

ui_menu *ui_manager::active_menu() const {
	return (!_Myforms.empty()?_Myforms.back()._Active_menu:0);
}