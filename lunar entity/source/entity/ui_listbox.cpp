/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_listbox.cpp
---------------------------------------------------
*/

#include "ui_listbox.h"

//Dependencies
#include <cmath>

//Dependencies (engine)
#include "input_manager.h"
#include "render_node.h"
#include "text_manager.h"
#include "font.h"
#include "texture.h"

#include "math_utility.h"
#include "generic_utility.h"
#include "any_cast.h"

//Public

//Additional
//Public

const std::string ui_listbox::additional::TEXT="TEXT";
const std::string ui_listbox::additional::SELECTION="SELECTION";

//Listbox
//Protected

ui_listbox::ui_listbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_scrollable(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Alignment(LEFT),_Item_index(-1),
					_Fit_selection(false),_Icon_alignment(LEFT_SIDE),
						_Icon_positioning(FIXED),_Icon_width(16.0f),
							_Icon_spacing(4.0f),_Show_icons(false),
								_Repeat_rate(0.03f),_Repeat_delay(0.5f),
									_Update_items(true),_Update_selection(true),
										_Repeat_key(0),_Repeat_time(0.0f) {

//Default listbox set up
_Caption_origin=ui_element::OUTSIDE_TOP_LEFT;
}

ui_listbox::ui_listbox(const ui_listbox &_Listbox):
	ui_scrollable(_Listbox) {

_Update_items=true; //Not copyable
_Update_selection=true; //Not copyable

_Repeat_key=0; //Not copyable
_Repeat_time=0.0f; //Not copyable

copy(_Listbox);
}

ui_listbox::ui_listbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_listbox &_Listbox):ui_scrollable(_Owner,_Name,
			_Parent_node,_Draw_order,_Listbox) {

_Update_items=true; //Not copyable
_Update_selection=true; //Not copyable

_Repeat_key=0; //Not copyable
_Repeat_time=0.0f; //Not copyable

copy(_Listbox);
}

ui_listbox::~ui_listbox() {
	//Empty
}

void ui_listbox::copy(const ui_listbox &_Listbox) {

_Alignment=_Listbox._Alignment;
_Text_offset=_Listbox._Text_offset;
_Item_index=_Listbox._Item_index;
_Fit_selection=_Listbox._Fit_selection;

_Icon_alignment=_Listbox._Icon_alignment;
_Icon_positioning=_Listbox._Icon_positioning;
_Icon_width=_Listbox._Icon_width;
_Icon_spacing=_Listbox._Icon_spacing;
_Show_icons=_Listbox._Show_icons;

_Repeat_rate=_Listbox._Repeat_rate;
_Repeat_delay=_Listbox._Repeat_delay;
}

//Events

void ui_listbox::scroll(unsigned int _Units,bool _Up) {

	//Nothing to do
	if (_Update_items) return;

text *_Text=text_component(object_pointer(additional::TEXT));

	//Text not updated
	if (!_Text || _Text->status()!=manageable::OKAY)
		return;

unsigned int _Skip_lines=_Text->skip_lines();

	//Up
	if (_Up) {
	
		//Scroll to top
		if (_Units==0) _Text->skip_lines(0);
		else _Text->skip_lines(_Skip_lines>_Units?
			_Skip_lines-_Units:0);
	}
	//Down
	else {
	
	unsigned int _Line_count=_Text->line_count();
	unsigned int _Max_lines=_Text->max_lines();
	unsigned int _Max_skiplines=(_Line_count>_Max_lines?
		_Line_count-_Max_lines:0);
	
		//Scroll to bottom
		if (_Units==0) _Text->skip_lines(_Max_skiplines);
		else _Text->skip_lines(_Skip_lines+_Units<_Max_skiplines?
			_Skip_lines+_Units:_Max_skiplines);
	}

_Scrolled=true; //Received event
_Update_selection=true;
}

unsigned int ui_listbox::scrollbar_count() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->line_count():
	ui_scrollable::scrollbar_count());
}

unsigned int ui_listbox::scrollbar_viewcount() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->max_lines():
	ui_scrollable::scrollbar_viewcount());
}

unsigned int ui_listbox::scrollbar_position() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->skip_lines():
	ui_scrollable::scrollbar_position());
}

//Utility

void ui_listbox::clean_data(std::string &_Data) {

	//Clean up non-printable characters
	for (size_t i=0;i<_Data.size();) {
	
		if (_Data[i]>=0 && _Data[i]<32)
			_Data.erase(i,1);
		else ++i;
	}
}

//Helper

unsigned int ui_listbox::item_location(const vector2 &_Position) const {

text *_Text=text_component(object_pointer(additional::TEXT));

	//Nothing to do
	if (!_Text || _Text->line_count()==0) return -1;

float _Pos=fabs(_Position.y-_Text_offset.y)-(float)_Text->line_height()*0.5f;
unsigned int _Line_distance=_Text->line_distance();
unsigned int _Selected_line=(_Line_distance>0?(unsigned int)(_Pos>0.0f?
	math::round(_Pos/_Line_distance):0.0f)+_Text->skip_lines():0);

	//Make sure selected line is in view
	if (_Selected_line>=_Text->skip_lines()+_Text->line_count(true))
		_Selected_line=_Text->skip_lines()+_Text->line_count(true)-1;

return _Selected_line;
}

text *ui_listbox::text_component(object *_Object) const {

	//Nothing to do
	if (!_Object) return 0;

text *_Text=0;
attachable *_Data;

	//Find first text component in object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Text=dynamic_cast<text*>(_Data)))
			break;
	}

return _Text;
}

void ui_listbox::update_items() {

object *_Object=object_pointer(additional::TEXT);

	//No text object found
	if (!_Object) return;

text *_Text=text_component(_Object);

	//Text not found
	if (!_Text) return;

//Update items

_Text->text_font()->update(); //Update font

	//Font update failed
	if (_Text->text_font()->status()!=manageable::OKAY)
		return;

unsigned int _Max_lines=_Text->line_capacity((unsigned int)_Area->height());
unsigned int _Width=(unsigned int)_Area->width();

	if (_Show_icons) {
	
		//Adjust text width based on icon column width
		if (_Icon_positioning==FIXED ||
			((_Alignment==LEFT && _Icon_alignment==LEFT_SIDE) ||
			(_Alignment==RIGHT && _Icon_alignment==RIGHT_SIDE))) {
		
		unsigned int _Icon_column=(unsigned int)ceil(_Icon_width+_Icon_spacing);
		_Width=(_Icon_column<=_Width?_Width-_Icon_column:0);
		}
	}
	
	if (_Max_lines==0) _Text->text_data("");
	else {
	
	std::string _Text_data;
	
		for (_Items::iterator _Iter=_Myitems.begin(),
			_End=_Myitems.end();_Iter!=_End;) {
		
		_Text_data+=text_manager::trim((*_Iter).data,
			_Text->text_font(),_Width);
		
			if (++_Iter!=_End) _Text_data+="\n";
		}
	
	_Text->text_data(_Text_data);
	}

_Text->wrap_width(_Width);
_Text->max_lines(_Max_lines);
_Text->styles_enabled(true);
_Text->update();

	//Text update failed
	if (_Text->status()!=manageable::OKAY)
		return;
	
	//Too small width for a single character
	if (_Text->width()>_Area->width()) {
	
	_Text->text_data("");
	_Text->update(); //Undo
	}

_Text->alignment((text::ALIGNMENT)_Alignment); //Update alignment

vector2 _Position(-_Area->width()*0.5f,_Area->height()*0.5f);
_Position=utility::position(_Position,_Area->width(),
	_Area->height(),rectangle::CENTER,_Area->origin());

vector2 _Icon_offset;

	if (_Show_icons) {
	
		//Add icon offset
		if ((_Icon_positioning==FIXED && _Icon_alignment==LEFT_SIDE) ||
			(_Alignment==LEFT && _Icon_alignment==LEFT_SIDE))
				_Icon_offset.x=_Icon_width+_Icon_spacing;
	}

_Object->node()->position(vector2(ceil(_Position.x),
	floor(_Position.y)+2.0f)+_Icon_offset+_Text_offset);
}

void ui_listbox::update_selection() {

object *_Object=object_pointer(additional::SELECTION);

	//No selection object found
	if (!_Object) return;

text *_Text=text_component(object_pointer(additional::TEXT));

	//Text not found/invalid or not enough room to show text
	if (!_Text || _Text->status()!=manageable::OKAY ||
		(_Text->text_data().empty() && !_Myitems.empty())) {
	
	_Object->visible(false);
	return;
	}

unsigned int _Skip_lines=_Text->skip_lines();
unsigned int _Max_lines=_Text->max_lines();
unsigned int _Line_count=_Text->line_count();

	//Selection
	if (_Item_index!=-1) {
	
		//Hide/show selection
		if (_Scrolled) {
		
		_Object->visible(_Item_index>=_Skip_lines &&
			_Item_index<_Skip_lines+_Max_lines);
		_Scrolled=false; //Event handled
		}
		//Follow cursor position
		else {
		
			//Update skip lines
			if (_Item_index<_Skip_lines || _Item_index>=_Skip_lines+_Max_lines ||
				(_Skip_lines>0 && _Line_count-_Skip_lines<_Max_lines)) {
			
			unsigned int _Max_skiplines=(_Line_count>_Max_lines?
				_Line_count-_Max_lines:0);
			
				if (_Skip_lines>0 && _Line_count-_Skip_lines<_Max_lines)
					--_Skip_lines;
				else _Skip_lines=(_Item_index<_Skip_lines?
					_Item_index:_Item_index+1-_Max_lines);
				
				if (_Skip_lines>_Max_skiplines)
					_Skip_lines=_Max_skiplines;
			
			_Text->skip_lines(_Skip_lines);
			}
		
		_Object->visible(true);
		}
	}
	
	//Hide/show icons
	if (_Show_icons) {
	
	unsigned int _Width=_Text->wrap_width();
	text::lines_iter _Iter=_Text->lines_begin();
	text::lines_iter _End=_Text->lines_end();
	
		for (size_t i=0;i<_Myitems.size() &&
			_Iter!=_End;++i,++_Iter) {
		
			//Check if icon can be shown
			if (_Icon_width<=_Width) {
			
				//Item is on screen
				if (i>=_Skip_lines && i<_Skip_lines+_Max_lines) {
				
					//Floating
					//Check if icon is outside text area
					if (_Icon_positioning==FLOATING) {
					
						if ((_Alignment==LEFT && _Icon_alignment==RIGHT_SIDE) ||
							(_Alignment==RIGHT && _Icon_alignment==LEFT_SIDE) ||
								_Alignment==CENTER) {
						
						unsigned int _Icon_column=(unsigned int)ceil(_Icon_width+_Icon_spacing);
						
							//Hide icon
							if ((*_Iter).width()+_Icon_column>_Width) {
							
							hide_icon(i);
							continue;
							}
						}
					}
				
				show_icon(i);
				continue;
				}
			}
		
		hide_icon(i);
		}
	}
	
	//No selection
	if (_Item_index==-1) {

	_Object->visible(false);
	return;
	}

texturable *_Texturable=0;
attachable *_Data;

	//Find first texturable component in object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Texturable=dynamic_cast<texturable*>(_Data)))
			break;
	}

manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Update manageable
	if (_Manageable) {
	
	_Manageable->update();
	
		//Make sure texturable size is correct
		if (_Manageable->status()==manageable::OKAY)
			_Texturable->prepare();
		else _Texturable=0;
	}

unsigned int _Line=_Item_index-_Skip_lines;

vector2 _Position(_Area->width()*0.5f,-(float)_Line*
	_Text->line_distance()-(float)_Text->line_height()*0.5f);
_Position=utility::position(_Position,_Area->width(),
	_Area->height(),rectangle::TOP_LEFT,_Area->origin());

	if (_Texturable) {
	
	_Texturable->auto_size(!_Fit_selection);
	
		if (_Fit_selection)
			_Texturable->size(_Area->width(),
				(float)_Text->line_height());
	}

_Object->node()->position(_Position+_Text_offset);
}

void ui_listbox::show_icon(unsigned int _Item_index) {

	//Nothing to do
	if (_Item_index>=_Myitems.size()) return;
	
	//No icon
	if (_Myitems[_Item_index].icon.empty()) return;

object *_Object=object_pointer(additional::TEXT);

	//No text object found
	if (!_Object) return;

text *_Text=text_component(_Object);

	//Text not found
	if (!_Text) return;

std::string _Name="item"+any_cast(_Item_index);
_Object=object_pointer(_Name);

	//No icon object found, create object
	if (!_Object) _Object=create_object(_Name,2);

component *_Component=_Object->component_pointer(_Name);

	//No icon component found, create component
	if (!_Component) _Component=_Object->create_component(_Name);

texture *_Icon=static_cast<texture*>(_Component->data());

	if (!_Icon || _Icon->data()->name()!=_Myitems[_Item_index].icon) {
	
	_Icon=static_cast<texture*>(_Component->data(
		texture_container::instance()->data(_Myitems[_Item_index].icon)));
	}
	
	if (_Icon) {
	
	_Icon->prepare();
	_Icon->auto_size(false);
	
		//Adjust size
		if (_Icon->width()!=_Icon_width) {
		
		float _Off=_Icon->width()-_Icon_width;
		_Icon->size(_Icon_width,_Icon->height()-_Off);
		}
		
		//Too high, adjust down height
		if (_Icon->height()>_Text->line_height()) {
		
		float _Off=_Icon->height()-(float)_Text->line_height();
		_Icon->size(_Icon->width()-_Off,_Icon->height()-_Off);
		}
	
	unsigned int _Line=_Item_index-_Text->skip_lines();
	vector2 _Position(0.0f,-(float)_Line*_Text->line_distance()-
		(float)_Text->line_height()*0.5f);
	
		if (_Icon_positioning==FIXED ||
			((_Alignment==LEFT && _Icon_alignment==LEFT_SIDE) ||
			(_Alignment==RIGHT && _Icon_alignment==RIGHT_SIDE))) {
		
			if (_Icon_alignment==LEFT_SIDE)
				_Position.x=_Icon->width()*0.5f;
			else if (_Icon_alignment==RIGHT_SIDE)
				_Position.x=_Area->width()-_Icon->width()*0.5f;
		}
		else {
		
		text::lines_iter _Iter=_Text->lines_begin();
		std::advance(_Iter,_Item_index);
		float _Width=_Icon_spacing+_Icon->width()*0.5f;
		
			if (_Alignment==LEFT)
				_Position.x=(*_Iter).width()+_Width;
			else if (_Alignment==RIGHT)
				_Position.x=_Area->width()-((*_Iter).width()+_Width);
			else if (_Alignment==CENTER) {
			
				if (_Icon_alignment==LEFT_SIDE)
					_Position.x=_Area->width()*0.5f-((*_Iter).width()*0.5f+_Width);
				else if (_Icon_alignment==RIGHT_SIDE)
					_Position.x=_Area->width()*0.5f+((*_Iter).width()*0.5f+_Width);
			}
		}
	
	_Position=utility::position(_Position,_Area->width(),
		_Area->height(),rectangle::TOP_LEFT,_Area->origin());
	_Object->node()->position(_Position);
	}

_Object->visible(!!_Icon);
}

void ui_listbox::hide_icon(unsigned int _Item_index) {

	//Nothing to do
	if (_Item_index>=_Myitems.size()) return;

object *_Object=object_pointer("item"+any_cast(_Item_index));

	if (_Object) _Object->visible(false);
}

void ui_listbox::remove_icon(unsigned int _Item_index) {

	//Nothing to do
	if (_Item_index>=_Myitems.size()) return;
	
	if (!_Myitems[_Item_index].icon.empty()) {
	
	object *_Object=object_pointer("item"+any_cast(_Item_index));
	
		if (_Object) remove_object(_Object);
	}
}

//Public

ui_listbox &ui_listbox::operator=(const ui_listbox &_Listbox) {

	//Worth doing
	if (this!=&_Listbox) {
	
	ui_scrollable::operator=(_Listbox); //Invoke base operator=
	copy(_Listbox);
	
	_Update_items=true;
	_Update_selection=true;
	}

return *this;
}

void ui_listbox::update(float _Time) {

	if (visible()) {
	
		if (_Update_items || _Update_caption) {
		
		update_items();
		_Update_items=false;
		_Update_selection=true;
		}
		
		if (_Update_selection) {
		
		update_selection();
		_Update_selection=false;
		update_scrollbar(); //Update attached scrollbar
		}
	}
	
	if (_Repeat_key>0) {
	
	_Repeat_time+=_Time;
	
		if (_Repeat_time>_Repeat_delay+_Repeat_rate) {
		
		key_pressed(_Repeat_key);
		_Repeat_time=_Repeat_delay;
		}
	}

ui_scrollable::update(_Time); //Use base functionality
}

bool ui_listbox::key_pressed(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;

	switch (_Key) {
		
		case VK_UP:
		case VK_LEFT:{
		
			if (_Item_index!=-1 &&
				_Item_index>0) {
			
			--_Item_index;
			_Update_selection=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_DOWN:
		case VK_RIGHT: {
		
			if (_Item_index!=-1 &&
				_Item_index+1<_Myitems.size()) {
			
			++_Item_index;
			_Update_selection=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_HOME: {
		
			if (_Item_index!=-1 &&
				_Item_index>0) {
			
			_Item_index=0;
			_Update_selection=true;
			}
		
		return true;
		}
		
		case VK_END: {
		
			if (_Item_index!=-1 &&
				_Item_index+1<_Myitems.size()) {
			
			_Item_index=(unsigned int)_Myitems.size()-1;
			_Update_selection=true;
			}
		
		return true;
		}
	}

return ui_scrollable::key_pressed(_Key);
}

bool ui_listbox::key_released(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;
	
	switch (_Key) {
	
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
		_Repeat_key=0;
		_Repeat_time=0.0f;
		
		case VK_HOME:
		case VK_END:
		return true;
	}

return ui_scrollable::key_released(_Key);
}

void ui_listbox::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (!_Focused || _Update_items) return;

object *_Object=object_pointer(additional::TEXT);

	//No text object found
	if (!_Object) return;

_Item_index=item_location(utility::position(
	vector2((float)_X,(float)_Y),_Area->width(),_Area->height(),
		_Area->origin(),rectangle::TOP_LEFT)-(_Object->node()->
			world_position()-_Object->node()->position()));
_Update_selection=true;
}

void ui_listbox::mouse_moved(unsigned int _X,
	unsigned int _Y) {

	//Nothing to do
	if (!_Focused || _Update_items) return;
	
	//Drag
	if (input_manager::instance()->mouse_active(0) ||
		input_manager::instance()->mouse_active(1))
			ui_listbox::mouse_pressed(0,_X,_Y);
}

bool ui_listbox::functional() const {
	return ((_Focusable && !_Myitems.empty()) || ui_element::functional());
}

void ui_listbox::alignment(const ALIGNMENT &_Alignment) {

	if (this->_Alignment!=_Alignment) {
	
	this->_Alignment=_Alignment;
	_Update_items=true;
	}
}

ui_listbox::ALIGNMENT ui_listbox::alignment() const {
	return _Alignment;
}

void ui_listbox::text_offset(const vector2 &_Offset) {

	if (_Text_offset!=_Offset) {
	
	_Text_offset=_Offset;
	_Update_items=true;
	}
}

const vector2 &ui_listbox::text_offset() const {
	return _Text_offset;
}

void ui_listbox::item_index(unsigned int _Item_index) {

	//Item index out of bounds
	if (_Item_index!=-1 &&
		_Item_index>=_Myitems.size()) return;
	
	if (this->_Item_index!=_Item_index) {
	
	this->_Item_index=_Item_index;
	_Update_selection=true;
	}
}

unsigned int ui_listbox::item_index() const {
	return _Item_index;
}

void ui_listbox::fit_selection(bool _Fit_selection) {

	if (this->_Fit_selection!=_Fit_selection) {
	
	this->_Fit_selection=_Fit_selection;
	_Update_selection=true;
	}
}

bool ui_listbox::fit_selection() const {
	return _Fit_selection;
}

//Icons

void ui_listbox::icon_alignment(const ICON_ALIGNMENT &_Icon_alignment) {

	if (this->_Icon_alignment!=_Icon_alignment) {
	
	this->_Icon_alignment=_Icon_alignment;
	_Update_items=true;
	}
}

ui_listbox::ICON_ALIGNMENT ui_listbox::icon_alignment() const {
	return _Icon_alignment;
}

void ui_listbox::icon_positioning(const ICON_POSITIONING &_Icon_positioning) {

	if (this->_Icon_positioning!=_Icon_positioning) {
	
	this->_Icon_positioning=_Icon_positioning;
	_Update_items=true;
	}
}

ui_listbox::ICON_POSITIONING ui_listbox::icon_positioning() const {
	return _Icon_positioning;
}

void ui_listbox::icon_width(float _Icon_width) {

float _Width=(_Icon_width<0.0f?0.0f:_Icon_width);

	if (this->_Icon_width!=_Width) {
	
	this->_Icon_width=_Width;
	_Update_items=true;
	}
}

float ui_listbox::icon_width() const {
	return _Icon_width;
}

void ui_listbox::icon_spacing(float _Icon_spacing) {

float _Spacing=(_Icon_spacing<0.0f?0.0f:_Icon_spacing);

	if (this->_Icon_spacing!=_Spacing) {
	
	this->_Icon_spacing=_Spacing;
	_Update_items=true;
	}
}

float ui_listbox::icon_spacing() const {
	return _Icon_spacing;
}

void ui_listbox::show_icons(bool _Show_icons) {

	if (this->_Show_icons!=_Show_icons) {
	
	this->_Show_icons=_Show_icons;
	_Update_items=true;
	}
}

bool ui_listbox::show_icons() const {
	return _Show_icons;
}

//Selection

void ui_listbox::select(unsigned int _Off) {
	item_index(_Off);
}

void ui_listbox::deselect() {
	item_index(-1);
}

void ui_listbox::repeat_rate(float _Repeat_rate) {
	this->_Repeat_rate=_Repeat_rate;
}

float ui_listbox::repeat_rate() const {
	return _Repeat_rate;
}

void ui_listbox::repeat_delay(float _Repeat_delay) {
	this->_Repeat_delay=_Repeat_delay;
}

float ui_listbox::repeat_delay() const {
	return _Repeat_delay;
}

//Items

void ui_listbox::add_item(const std::string &_Data) {
	insert_item(_Data,-1);
}

void ui_listbox::add_item(const item &_Item) {
	insert_item(_Item,-1);
}

void ui_listbox::insert_item(const std::string &_Data,unsigned int _Off) {

item _Item;
_Item.data=_Data;
insert_item(_Item,_Off);
}

void ui_listbox::insert_item(const item &_Item,unsigned int _Off) {

	if (_Off<_Myitems.size()) {
	
		//Remove previous icon
		if (_Item.icon.empty())
			remove_icon(_Off);
		
		//Remove icons that mismatch after insert
		for (unsigned int i=_Off;i<_Myitems.size()-1;++i) {
		
			if (_Myitems[i].icon.empty())
				remove_icon(i+1);
		}
	
	_Myitems.insert(_Myitems.begin()+_Off,_Item);
	clean_data(_Myitems[_Off].data);
	
		//Move selection down
		if (_Item_index!=-1 && _Item_index>=_Off)
			++_Item_index;
	}
	else {
	
	_Myitems.push_back(_Item);
	clean_data(_Myitems.back().data);
	}

_Update_items=true;
}

void ui_listbox::change_item(const std::string &_Data,unsigned int _Off) {

	if (_Off<_Myitems.size()) {
	
	_Myitems[_Off].data=_Data;
	clean_data(_Myitems[_Off].data);
	_Update_items=true;
	}
}

void ui_listbox::change_item(const item &_Item,unsigned int _Off) {

	if (_Off<_Myitems.size()) {
	
		//Remove previous icon
		if (_Item.icon.empty())
			remove_icon(_Off);
	
	_Myitems[_Off]=_Item;
	clean_data(_Myitems[_Off].data);
	_Update_items=true;
	}
}

void ui_listbox::remove_item(unsigned int _Off) {

	if (!_Myitems.empty()) {
	
		if (_Off<_Myitems.size()) {
		
			//Remove icons that mismatch after remove
			for (unsigned int i=_Off;i<_Myitems.size()-1;++i) {
			
				if (_Myitems[i+1].icon.empty())
					remove_icon(i);
			}
		
		remove_icon((unsigned int)_Myitems.size()-1);
		_Myitems.erase(_Myitems.begin()+_Off);
		
			//Deselect
			if (_Item_index==_Off) _Item_index=-1;
			//Move selection up
			else if (_Item_index!=-1 && _Item_index>_Off)
				--_Item_index;
		}
		else {
		
		remove_icon((unsigned int)_Myitems.size()-1);
		_Myitems.pop_back();
		
			//Deselect
			if (_Item_index==_Myitems.size())
				_Item_index=-1;
		}
	
	_Update_items=true;
	}
}

void ui_listbox::clear_items() {

	if (_Myitems.empty()) return;
	
	//Remove icons
	for (unsigned int i=0;i<_Myitems.size();++i)
		remove_icon(i);

_Myitems.clear();
_Item_index=-1; //Deselect
_Update_items=true;
}

ui_listbox::items_iter ui_listbox::items_begin() const {
	return _Myitems.begin();
}

ui_listbox::items_iter ui_listbox::items_end() const {
	return _Myitems.end();
}