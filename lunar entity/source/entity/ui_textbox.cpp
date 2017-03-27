/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_textbox.cpp
---------------------------------------------------
*/

#include "ui_textbox.h"

//Dependencies
#include <cmath>

//Dependencies (engine)
#include "input_manager.h"
#include "render_node.h"
#include "text_manager.h"
#include "font.h"
#include "texturable.h"

#include "math_utility.h"
#include "generic_utility.h"
#include "system.h"

//Public

//Additional
//Public

const std::string ui_textbox::additional::TEXT="TEXT";
const std::string ui_textbox::additional::CURSOR="CURSOR";

//Textbox
//Protected

ui_textbox::ui_textbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		float _Width,float _Height,bool _Visible):
			ui_scrollable(_Owner,_Name,_Parent_node,_Draw_order,
				_Width,_Height,_Visible),_Mode(ALPHA_NUMERIC),_Alignment(LEFT),
					_Off_bound(HIDE),_Multiple_lines(false),_Max_characters(0),_Mask(0),
						_Cursor_position(0),_Reveal_distance(0),_Repeat_rate(0.03f),
							_Repeat_delay(0.5f),_Blink_rate(1.0f),_Blink_hold(0.7f),
								_Update_text(true),_Update_cursor(true),_Left_bound(0),
									_Right_bound(0),_Repeat_key(0),_Blink_out(true),
										_Repeat_time(0.0f),_Blink_time(0.0f) {

//Default textbox set up
_Caption_origin=OUTSIDE_LEFT;
}

ui_textbox::ui_textbox(const ui_textbox &_Textbox):
	ui_scrollable(_Textbox) {

_Update_text=true; //Not copyable
_Update_cursor=true; //Not copyable

_Repeat_key=0; //Not copyable
_Blink_out=true; //Not copyable
_Repeat_time=0.0f; //Not copyable
_Blink_time=0.0f; //Not copyable

copy(_Textbox);
}

ui_textbox::ui_textbox(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_textbox &_Textbox):ui_scrollable(_Owner,_Name,
			_Parent_node,_Draw_order,_Textbox) {

_Update_text=true; //Not copyable
_Update_cursor=true; //Not copyable

_Repeat_key=0; //Not copyable
_Blink_out=true; //Not copyable
_Repeat_time=0.0f; //Not copyable
_Blink_time=0.0f; //Not copyable

copy(_Textbox);
}

ui_textbox::~ui_textbox() {
	//Empty
}

void ui_textbox::copy(const ui_textbox &_Textbox) {

_Mode=_Textbox._Mode;
_Text_data=_Textbox._Text_data;
_Alignment=_Textbox._Alignment;
_Off_bound=_Textbox._Off_bound;
_Text_offset=_Textbox._Text_offset;
_Multiple_lines=_Textbox._Multiple_lines;
_Max_characters=_Textbox._Max_characters;
_Mask=_Textbox._Mask;

_Cursor_position=_Textbox._Cursor_position;
_Reveal_distance=_Textbox._Reveal_distance;
_Repeat_rate=_Textbox._Repeat_rate;
_Repeat_delay=_Textbox._Repeat_delay;
_Blink_rate=_Textbox._Blink_rate;
_Blink_hold=_Textbox._Blink_hold;

_Left_bound=_Textbox._Left_bound;
_Right_bound=_Textbox._Right_bound;
}

//Events

void ui_textbox::focus() {

_Update_cursor=true;
ui_scrollable::focus();
}

void ui_textbox::defocus() {

object *_Object=object_pointer(additional::CURSOR);

	if (_Object) _Object->visible(false);

ui_scrollable::defocus();
}

void ui_textbox::scroll(unsigned int _Units,bool _Up) {

	//Nothing to do
	if (!_Multiple_lines || _Update_text) return;

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
_Update_cursor=true;
}

unsigned int ui_textbox::scrollbar_count() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->line_count():
	ui_scrollable::scrollbar_count());
}

unsigned int ui_textbox::scrollbar_viewcount() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->max_lines():
	ui_scrollable::scrollbar_viewcount());
}

unsigned int ui_textbox::scrollbar_position() const {

text *_Text=text_component(object_pointer(additional::TEXT));
return (_Text?_Text->skip_lines():
	ui_scrollable::scrollbar_position());
}

//Utility

bool ui_textbox::clean_data(std::string &_Data,bool _Multiple_lines) {

bool _Changed=false;

	//Clean up non-printable characters
	for (size_t i=0;i<_Data.size();) {
	
		if (_Data[i]>=0 && _Data[i]<32 &&
			(!_Multiple_lines || _Data[i]!='\n')) {
		
		_Data.erase(i,1);
		_Changed=true;
		}
		else ++i;
	}

return _Changed;
}

bool ui_textbox::trim_data(std::string &_Data,const MODE &_Mode) {

bool _Changed=false;

	if (_Mode==ALPHA) {
	
		for (size_t i=0;i<_Data.size();) {
		
			//Remove numeric characters
			if (_Data[i]>='0' && _Data[i]<='9') {
			
			_Data.erase(i,1);
			_Changed=true;
			}
			else ++i;
		}
	}
	else if (_Mode==NUMERIC) {
	
		for (size_t i=0;i<_Data.size();) {
		
			//Remove non-numeric characters
			if ((_Data[i]<'0' || _Data[i]>'9')
				&& _Data[i]!='\n') { //Exception, keep new line
			
			_Data.erase(i,1);
			_Changed=true;
			}
			else ++i;
		}
	}

return _Changed;
}

bool ui_textbox::trim_data(std::string &_Data,unsigned int _Max_characters) {

	if (_Data.size()>_Max_characters) {
	
		//Clear
		if (_Max_characters==0) _Data.clear();
		//Trim
		else _Data.erase(_Max_characters);
	
	return true;
	}

return false;
}

std::string ui_textbox::mask_data(const std::string &_Data,char _Mask) {

std::string _Masked_data(_Data.size(),_Mask);

	for (size_t i=0;i<_Data.size();++i) {
	
		//Keep new line
		if (_Data[i]=='\n') _Masked_data[i]='\n';
	}

return _Masked_data;
}

void ui_textbox::hide_text(text *_Text,unsigned int _Width) {

font *_Font=_Text->text_font();
unsigned int _Text_width=text_manager::width(
	_Text->text_data(),_Font);

	//Nothing to hide
	if (_Text_width<=_Width) {
	
	_Left_bound=_Right_bound=0;
	return;
	}

std::string _Data=_Text->text_data();
bool _Move_right=(_Left_bound==0 && _Right_bound==0); //Default

	if (!_Move_right) {
	
		//Out of bounds, clamp bounds
		if (_Right_bound>_Text_data.size()) {
		
			if (_Left_bound>_Right_bound-(unsigned int)_Text_data.size())
				_Left_bound-=_Right_bound-(unsigned int)_Text_data.size();
			else _Left_bound=0;
		
		_Right_bound=(unsigned int)_Text_data.size();
		}
		
		//Reveal right
		if (_Cursor_position>_Right_bound) {
		
		_Right_bound=(_Cursor_position+_Reveal_distance<_Text_data.size()?
			_Cursor_position+_Reveal_distance:(unsigned int)_Text_data.size());
		_Move_right=true;
		}
		//Reveal left
		else if (_Cursor_position<_Left_bound)
			_Left_bound=(_Cursor_position>_Reveal_distance?
				_Cursor_position-_Reveal_distance:0);
	}
	
	//Hide leftmost characters
	if (_Move_right || _Left_bound>0) {
	
	unsigned int _Off_width=text_manager::width(_Text->text_data().
		substr(0,_Move_right?_Right_bound>0?_Right_bound:
			_Cursor_position:_Left_bound),_Font);
	
		if (!_Move_right) _Off_width+=_Width;
	
	_Left_bound=0;
	
		while (_Off_width>_Width) {
		
		unsigned int _Char_width=text_manager::width(_Data[0],_Font);
		_Off_width-=_Char_width;
		_Text_width-=_Char_width;
		
		_Data.erase(0,1);
		++_Left_bound;
		}
	}

_Right_bound=0;

	//Hide rightmost characters
	while (_Text_width>_Width) {
	
	_Text_width-=text_manager::width(_Data[_Data.size()-1],_Font);
	_Data.erase(_Data.size()-1,1);
	++_Right_bound;
	}

_Right_bound=(unsigned int)_Text->text_data().size()-_Right_bound;

	//Cursor out of bound (because of variable character width)
	if (_Cursor_position>_Right_bound) {
	
	unsigned int _Off=_Cursor_position-_Right_bound;
	_Left_bound+=_Off;
	_Right_bound+=_Off;
	
	hide_text(_Text,_Width);
	return;
	}

_Text->text_data(_Data);
}

void ui_textbox::trim_text(text *_Text,unsigned int _Width) {

font *_Font=_Text->text_font();
unsigned int _Text_width=text_manager::width(
	_Text->text_data(),_Font);

	if (_Text_width<=_Width) return;

std::string _Data=_Text->text_data();

	//Text too long for line, trim one or more characters
	while (_Text_width>_Width) {
	
	unsigned char _Character=(_Cursor_position>0?
		_Data[_Cursor_position-1]:_Data[_Cursor_position]);
	_Text_width-=text_manager::width(_Character,_Font);
	
	_Text_data.erase(_Cursor_position>0?
		_Cursor_position-1:_Cursor_position,1);
	_Data.erase(_Cursor_position>0?
		_Cursor_position-1:_Cursor_position,1);
	
		if (_Cursor_position>0) --_Cursor_position;
	}

_Text->text_data(_Data);
}

//Helper

std::string ui_textbox::cursor_location(unsigned int _Cursor_position,
	unsigned int &_Line,unsigned int &_Column) const {

text *_Text=text_component(object_pointer(additional::TEXT));

	//Nothing to do
	if (!_Text) return 0;

const std::string &_Text_data=_Text->text_data();

	//Invalid cursor position
	if (_Cursor_position>_Text_data.size())
		return _Text_data;

_Line=_Column=0;
std::string _Line_data;

	//Find cursor line and column
	for (text::lines_iter _Iter=_Text->lines_begin(),
		_End=_Text->lines_end();_Iter!=_End;++_Iter,++_Line) {
	
	_Line_data=(*_Iter).data(); //Increase speed
	
		//Add offset
		if (_Line>0 && (_Text_data[_Column]=='\n' ||
			_Text_data[_Column]==' ')) ++_Column;
		
		//Check if cursor is located on this line
		if (_Cursor_position<=_Column+_Line_data.size()) {
		
		_Column=_Cursor_position-_Column;
		break;
		}
	
	_Column+=(unsigned int)_Line_data.size();
	}

return _Line_data;
}

unsigned int ui_textbox::cursor_location(unsigned int _Line,
	unsigned int _Column) const {

text *_Text=text_component(object_pointer(additional::TEXT));

	//Nothing to do
	if (!_Text) return 0;

const std::string &_Text_data=_Text->text_data();

unsigned int _Current_line=0;
unsigned int _Cursor_position=0;
std::string _Line_data;

	//Find cursor line and column
	for (text::lines_iter _Iter=_Text->lines_begin(),
		_End=_Text->lines_end();_Iter!=_End;++_Iter,++_Current_line) {
	
	_Line_data=(*_Iter).data(); //Increase speed
	
		//Add offset
		if (_Current_line>0 && (_Text_data[_Cursor_position]=='\n' ||
			_Text_data[_Cursor_position]==' ')) ++_Cursor_position;
		
		//Check if cursor is located on this line
		if (_Current_line>=_Line) {
		
		_Cursor_position+=(_Column>_Line_data.size()?
			(unsigned int)_Line_data.size():_Column);
		break;
		}
	
	_Cursor_position+=(unsigned int)_Line_data.size();
	}

return _Cursor_position;
}

unsigned int ui_textbox::cursor_location(const vector2 &_Position) const {

text *_Text=text_component(object_pointer(additional::TEXT));

	//Nothing to do
	if (!_Text || _Text->line_count()==0) return 0;

float _Pos=fabs(_Position.y-_Text_offset.y)-(float)_Text->line_height()*0.5f;
unsigned int _Line_distance=_Text->line_distance();
unsigned int _Selected_line=(_Line_distance>0?(unsigned int)(_Pos>0.0f?
	math::round(_Pos/_Line_distance):0.0f)+_Text->skip_lines():0);

	//Make sure selected line is in view
	if (_Selected_line>=_Text->skip_lines()+_Text->line_count(true))
		_Selected_line=_Text->skip_lines()+_Text->line_count(true)-1;

unsigned int _Line=0;
unsigned int _Column=0;

	for (text::lines_iter _Iter=_Text->lines_begin(),
		_End=_Text->lines_end();_Iter!=_End;++_Iter,++_Line) {
	
		//Found selected line
		if (_Line==_Selected_line) {
		
		std::string _Line_data=(*_Iter).data();
		unsigned int _Line_width=(*_Iter).width();
		unsigned int _Off=(unsigned int)_Text_offset.x;
		
			switch (_Alignment) {
			
				case CENTER:
				_Off+=(unsigned int)(_Area->width()*0.5f-_Line_width*0.5f);
				break;
				
				case RIGHT:
				_Off+=(unsigned int)_Area->width()-_Line_width;
				break;
			}
		
		font *_Font=_Text->text_font();
		
			for (unsigned int _Width=_Off;_Column<_Line_data.size();++_Column) {
			
			_Width+=text_manager::width(_Line_data[_Column],_Font);
			
				//Found selected column
				if (_Width>=_Position.x+1.0f) break;
			}
		
		break;
		}
	}

return _Left_bound+cursor_location(_Line,_Column);
}

text *ui_textbox::text_component(object *_Object) const {

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

void ui_textbox::update_text() {

object *_Object=object_pointer(additional::TEXT);

	//No text object found
	if (!_Object) return;

text *_Text=text_component(_Object);

	//Text not found
	if (!_Text) return;

//Update text data

_Text->text_font()->update(); //Update font

	//Font update failed
	if (_Text->text_font()->status()!=manageable::OKAY)
		return;

unsigned int _Max_lines=_Text->line_capacity((unsigned int)_Area->height());
unsigned int _Width=(unsigned int)_Area->width();

	if (_Max_lines==0) _Text->text_data("");
	else {
	
	_Text->text_data(_Mask==0?_Text_data:
		mask_data(_Text_data,_Mask)); //Apply mask to output text
	
		if (!_Multiple_lines) {
		
			if (_Off_bound==TRIM) trim_text(_Text,_Width);
			else hide_text(_Text,_Width);
		}
	}

_Text->wrap_width(_Width);
_Text->max_lines(_Multiple_lines?_Max_lines:1);
_Text->styles_enabled(false); //Not allowed for textbox, use label
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

vector2 _Position(-_Area->width()*0.5f,_Multiple_lines?
	_Area->height()*0.5f:(float)_Text->height()*0.5f);

_Position=utility::position(_Position,_Area->width(),
	_Area->height(),rectangle::CENTER,_Area->origin());
_Object->node()->position(vector2(ceil(_Position.x),
	floor(_Position.y)+2.0f)+_Text_offset);
}

void ui_textbox::update_cursor() {

object *_Object=object_pointer(additional::CURSOR);

	//No cursor object found
	if (!_Object) return;

text *_Text=text_component(object_pointer(additional::TEXT));

	//Text not found/invalid or not enough room to show text
	if (!_Text || _Text->status()!=manageable::OKAY ||
		(_Text->text_data().empty() && !_Text_data.empty())) {
	
	_Object->visible(false);
	return;
	}

unsigned int _Line=0;
unsigned int _Column=0;
unsigned int _Width=0;
unsigned int _Line_width=0;
std::string _Line_data=cursor_location(
	_Cursor_position-_Left_bound,_Line,_Column);

font *_Font=_Text->text_font();

	if (_Column>0) {
	
		//Calculate width to cursor
		for (size_t i=0;i<_Column;++i)
			_Width+=text_manager::width(_Line_data[i],_Font);
	
	_Line_width=_Width;
	}
	
	//Calculate total line width
	for (size_t i=_Column;i<_Line_data.size();++i)
		_Line_width+=text_manager::width(_Line_data[i],_Font);

unsigned int _Skip_lines=_Text->skip_lines();
unsigned int _Max_lines=_Text->max_lines();
unsigned int _Line_count=_Text->line_count();

	//Hide/show cursor
	if (_Scrolled) {
	
	_Object->visible(_Line>=_Skip_lines &&
		_Line<_Skip_lines+_Max_lines);
	_Scrolled=false; //Event handled
	}
	//Follow cursor position
	else {
	
		//Update skip lines
		if (_Line<_Skip_lines || _Line>=_Skip_lines+_Max_lines ||
			(_Skip_lines>0 && _Line_count-_Skip_lines<_Max_lines)) {
		
		unsigned int _Max_skiplines=(_Line_count>_Max_lines?
			_Line_count-_Max_lines:0);
		
			if (_Skip_lines>0 && _Line_count-_Skip_lines<_Max_lines)
				--_Skip_lines;
			else _Skip_lines=(_Line<_Skip_lines?
				_Line:_Line+1-_Max_lines);
			
			if (_Skip_lines>_Max_skiplines)
				_Skip_lines=_Max_skiplines;
		
		_Text->skip_lines(_Skip_lines);
		}
	
	_Object->visible(true);
	}

_Line-=_Skip_lines; //Convert to visible line

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

vector2 _Position((float)_Width,_Multiple_lines?-(float)_Line*
	_Text->line_distance()-(float)_Text->line_height()*0.5f:
		-_Area->height()*0.5f);

	switch (_Alignment) {
	
		case CENTER:
		_Position.x=_Area->width()*0.5f+(float)_Width-_Line_width*0.5f;
		break;
		
		case RIGHT:
		_Position.x=_Area->width()+(float)_Width-_Line_width;
		break;
	}

_Position=utility::position(_Position,_Area->width(),
	_Area->height(),rectangle::TOP_LEFT,_Area->origin());

	if (_Texturable) {
	
	_Position+=utility::position(vector2::ZERO,_Texturable->width(),
		_Texturable->height(),_Texturable->origin());
	
	_Position.x=ceil(_Position.x);
	_Position.y=floor(_Position.y);
	rectangle::ORIGIN _Origin=_Texturable->origin(); 
	
		//Blur fix
		switch (_Origin) {
		
			case rectangle::TOP_CENTER:
			case rectangle::CENTER:
			case rectangle::BOTTOM_CENTER: {
			
				if (_Texturable->width()*0.5f-
					floor(_Texturable->width()*0.5f)>0.0f)
						_Position.x+=0.5f;
			
			break;
			}
		}
	}

_Object->node()->position(_Position+_Text_offset);
}

//Public

ui_textbox &ui_textbox::operator=(const ui_textbox &_Textbox) {

	//Worth doing
	if (this!=&_Textbox) {
	
	ui_scrollable::operator=(_Textbox); //Invoke base operator=
	copy(_Textbox);
	
	_Update_text=true;
	_Update_cursor=true;
	}

return *this;
}

void ui_textbox::update(float _Time) {

	if (visible()) {
	
		if (_Update_text || _Update_caption) {
		
		update_text();
		_Update_text=false;
		_Update_cursor=true;
		}
		
		//Stop cursor blink while typing
		if ((_Update_cursor || _Repeat_key>0) && _Focused)
			_Blink_time=-1.0f;
		
		if (_Update_cursor && _Focused) {
		
		update_cursor();
		_Update_cursor=false;
		update_scrollbar(); //Update attached scrollbar
		}
		
		//Update cursor blink
		if (_Focused) {
		
		object *_Object=object_pointer(additional::CURSOR);
		
			if (_Object) {
			
			float _Opacity=_Object->opacity();
			float _Previous_opacity=_Opacity;
			
				if (_Blink_rate==0.0f || _Blink_time<0.0f) {
				
				_Opacity=1.0f;
				_Blink_out=true;
				_Blink_time=0.0f;
				}
				else {
				
				_Blink_time+=_Time;
				
					if (_Blink_time>_Blink_rate*0.5f*_Blink_hold) {
					
						if (_Blink_out) {
						
							if ((_Opacity-=_Time/(_Blink_rate*0.5f*
								(1.0f-_Blink_hold)))<=0.0f) {
							
							_Opacity=0.0f;
							_Blink_out=false;
							_Blink_time=0.0f;
							}
						}
						else {
						
							if ((_Opacity+=_Time/(_Blink_rate*0.5f*
								(1.0f-_Blink_hold)))>=1.0f) {
							
							_Opacity=1.0f;
							_Blink_out=true;
							_Blink_time=0.0f;
							}
						}
					}
				}
				
				//Update opacity for cursor and all child objects
				if (_Opacity!=_Previous_opacity)
					opacity(_Object,_Opacity,true);
			}
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

bool ui_textbox::key_pressed(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;
	
	if (input_manager::instance()->key_active(VK_CONTROL)) {
	
		switch (_Key) {
		
			case 'X':
			cut_text(0);
			return true;
			
			case 'C':
			copy_text(0);
			return true;
			
			case 'V':
			paste_text(_Cursor_position,_Cursor_position);
			_Repeat_key=_Key;
			return true;
		}
	}
	
	switch (_Key) {
	
		case VK_RETURN: {
		
			if (_Multiple_lines && (_Max_characters==0 ||
				_Text_data.size()<_Max_characters)) {
			
			_Text_data.insert(_Cursor_position++,1,'\n');
			_Update_text=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_UP: {
		
			if (_Multiple_lines) {
			
			unsigned int _Line=0;
			unsigned int _Column=0;
			cursor_location(_Cursor_position,_Line,_Column);
			_Cursor_position=cursor_location(_Line>0?
				_Line-1:_Line,_Line>0?_Column:0);
			_Update_cursor=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_DOWN: {
		
			if (_Multiple_lines) {
			
			unsigned int _Line=0;
			unsigned int _Column=0;
			cursor_location(_Cursor_position,_Line,_Column);
			_Cursor_position=cursor_location(_Line+1,_Column);
			_Update_cursor=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_LEFT: {
		
			if (_Cursor_position>0) {
			
				if (_Left_bound>0 && _Cursor_position==_Left_bound)
					_Update_text=true;
				else _Update_cursor=true;
			
			--_Cursor_position;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_RIGHT: {
		
			if (_Cursor_position<_Text_data.size()) {
			
				if (_Right_bound>0 && _Cursor_position==_Right_bound)
					_Update_text=true;
				else _Update_cursor=true;
			
			++_Cursor_position;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_BACK: {
		
			if (_Cursor_position>0) {
			
				if (_Left_bound>0 && _Right_bound<_Text_data.size())
					--_Left_bound;
			
			_Text_data.erase(--_Cursor_position,1);
			_Update_text=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_DELETE: {
		
			if (_Cursor_position<_Text_data.size()) {
			
			_Text_data.erase(_Cursor_position,1);
			_Update_text=true;
			}
		
		_Repeat_key=_Key;
		return true;
		}
		
		case VK_HOME: {
		
			if (_Multiple_lines) {
			
			unsigned int _Line=0;
			unsigned int _Column=0;
			cursor_location(_Cursor_position,_Line,_Column);
			_Cursor_position=cursor_location(_Line,0);
			_Update_cursor=true;
			}
			else {
			
				if (_Cursor_position>0) {
				
				_Cursor_position=0;
				
					if (_Left_bound>0) _Update_text=true;
					else _Update_cursor=true;
				}
			}
		
		return true;
		}
		
		case VK_END: {
		
			if (_Multiple_lines) {
			
			unsigned int _Line=0;
			unsigned int _Column=0;
			unsigned int _Size=(unsigned int)cursor_location(
				_Cursor_position,_Line,_Column).size();
			_Cursor_position=cursor_location(_Line,_Size);
			_Update_cursor=true;
			}
			else {
			
				if (_Cursor_position<_Text_data.size()) {
				
				_Cursor_position=(unsigned int)_Text_data.size();
				
					if (_Right_bound>0 && _Cursor_position>_Right_bound)
						_Update_text=true;
					else _Update_cursor=true;
				}
			}
		
		return true;
		}
	}

return ui_scrollable::key_pressed(_Key);
}

bool ui_textbox::key_released(unsigned int _Key) {

	//Nothing to do
	if (!_Focused) return false;
	
	switch (_Key) {
	
		case 'V':
		case VK_RETURN:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE:
		_Repeat_key=0;
		_Repeat_time=0.0f;
		
		case VK_HOME:
		case VK_END:
		return true;
	}

return ui_scrollable::key_released(_Key);
}

bool ui_textbox::char_pressed(char _Character) {

	//Nothing to do
	if (!_Focused) return false;
	
	if (_Max_characters==0 || _Text_data.size()<_Max_characters) {
	
		//Compatible characters
		if (_Mode==ALPHA_NUMERIC ||
			(_Mode==ALPHA && (_Character<'0' || _Character>'9')) ||
			(_Mode==NUMERIC && _Character>='0' && _Character<='9')) {
		
		_Text_data.insert(_Cursor_position++,1,_Character);
		_Update_text=true;
		}
	}

return true;
}

void ui_textbox::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (!_Focused || _Update_text) return;

object *_Object=object_pointer(additional::TEXT);

	//No text object found
	if (!_Object) return;

_Cursor_position=cursor_location(utility::position(
	vector2((float)_X,(float)_Y),_Area->width(),_Area->height(),
		_Area->origin(),rectangle::TOP_LEFT)-(_Object->node()->
			world_position()-_Object->node()->position()));
_Update_cursor=true;
}

void ui_textbox::mouse_moved(unsigned int _X,
	unsigned int _Y) {

	//Nothing to do
	if (!_Focused || _Update_text) return;
	
	//Drag
	if (input_manager::instance()->mouse_active(0) ||
		input_manager::instance()->mouse_active(1))
			ui_textbox::mouse_pressed(0,_X,_Y);
}

bool ui_textbox::functional() const {
	return (_Focusable || ui_element::functional());
}

void ui_textbox::mode(const MODE &_Mode) {

	if (this->_Mode!=_Mode) {
	
		//Partial compatible modes
		if (this->_Mode==ALPHA_NUMERIC) {
		
			if (trim_data(this->_Text_data,_Mode))
				_Update_text=true;
		}
		//Incompatible modes
		else if (_Mode!=ALPHA_NUMERIC) {
		
			if (!_Text_data.empty()) {
			
			unsigned int _New_lines=0;
			
				//Exception, keep new line
				for (size_t i=0;i<_Text_data.size();++i) {
				
					if (_Text_data[i]=='\n') ++_New_lines;
				}
			
			_Text_data.clear();
			
				if (_New_lines>0)
					_Text_data.assign(_New_lines,'\n');
			
			_Update_text=true;
			}
		}
		
		if (_Update_text) {
		
			//Update cursor position
			if (_Cursor_position>_Text_data.size())
				_Cursor_position=(unsigned int)_Text_data.size();
		}
	
	this->_Mode=_Mode;
	}
}

ui_textbox::MODE ui_textbox::mode() const {
	return _Mode;
}

void ui_textbox::text_data(const std::string &_Text_data) {

	if (this->_Text_data!=_Text_data) {
	
	this->_Text_data=_Text_data;
	clean_data(this->_Text_data,_Multiple_lines);
	trim_data(this->_Text_data,_Mode);
	
		if (_Max_characters>0)
			trim_data(this->_Text_data,_Max_characters);
	
	_Cursor_position=(unsigned int)_Text_data.size(); //Update cursor position
	_Update_text=true;
	}
}

void ui_textbox::insert_data(const std::string &_Text_data,
	unsigned int _From_position,unsigned int _To_position) {

	//Invalid range
	if (_From_position>_To_position) return;
	
	//Something to insert
	if (!_Text_data.empty()) {
	
	std::string _Data=_Text_data;
	clean_data(_Data,_Multiple_lines);
	trim_data(_Data,_Mode);
	
		//Still something to insert
		if (!_Data.empty()) {
		
			//Append
			if (_From_position>=this->_Text_data.size()) {
			
				if (_Max_characters>0 &&
					_Max_characters>=this->_Text_data.size()) {
				
				unsigned int _Capacity=_Max_characters-this->_Text_data.size();
				
					//Not enough room
					if (_Capacity==0) return;
				
				trim_data(_Data,_Capacity);
				}
			
			this->_Text_data.append(_Data);
			_Cursor_position=(unsigned int)this->_Text_data.size(); //Update cursor position
			}
			else {
			
				//Insert
				if (_From_position==_To_position ||
					_To_position>this->_Text_data.size())
						this->_Text_data.insert(_From_position,_Data);
				//Replace
				else this->_Text_data.replace(_From_position,
					_To_position-_From_position,_Data);
				
				if (_Max_characters>0)
					trim_data(this->_Text_data,_Max_characters);
			
			_Cursor_position=(unsigned int)_From_position+_Data.size(); //Update cursor position
			}
		
		_Update_text=true;
		}
	}
}

const std::string &ui_textbox::text_data() const {
	return _Text_data;
}

void ui_textbox::alignment(const ALIGNMENT &_Alignment) {

	if (this->_Alignment!=_Alignment) {
	
	this->_Alignment=_Alignment;
	_Update_text=true;
	}
}

ui_textbox::ALIGNMENT ui_textbox::alignment() const {
	return _Alignment;
}

void ui_textbox::off_bound(const OFF_BOUND &_Off_bound) {

	if (this->_Off_bound!=_Off_bound) {
	
	this->_Off_bound=_Off_bound;
	_Update_text=true;
	_Left_bound=_Right_bound=0;
	}
}

ui_textbox::OFF_BOUND ui_textbox::off_bound() const {
	return _Off_bound;
}

void ui_textbox::text_offset(const vector2 &_Offset) {

	if (_Text_offset!=_Offset) {
	
	_Text_offset=_Offset;
	_Update_text=true;
	}
}

const vector2 &ui_textbox::text_offset() const {
	return _Text_offset;
}

void ui_textbox::multiple_lines(bool _Multiple_lines) {

	if (this->_Multiple_lines!=_Multiple_lines) {
	
		if (!_Multiple_lines) {
		
			//Remove new line
			for (size_t i=0;i<_Text_data.size();) {
			
				if (_Text_data[i]=='\n')
					_Text_data.erase(i,1);
				else ++i;
			}
			
			//Update cursor position
			if (_Cursor_position>_Text_data.size())
				_Cursor_position=(unsigned int)_Text_data.size();
		}
	
	this->_Multiple_lines=_Multiple_lines;
	_Update_text=true;
	_Left_bound=_Right_bound=0;
	}
}

bool ui_textbox::multiple_lines() const {
	return _Multiple_lines;
}

void ui_textbox::max_characters(unsigned int _Max_characters) {

	if (this->_Max_characters!=_Max_characters) {
	
		//Trim
		if (_Max_characters>0) {
		
			if (trim_data(this->_Text_data,_Max_characters))
				_Update_text=true;
		}
	
	this->_Max_characters=_Max_characters;
	}
}

unsigned int ui_textbox::max_characters() const {
	return _Max_characters;
}

void ui_textbox::mask(char _Mask) {

	if (this->_Mask!=_Mask) {
	
		//Not allowed
		if (_Mask=='\n') return;
	
	this->_Mask=_Mask;
	_Update_text=true;
	}
}

char ui_textbox::mask() const {
	return _Mask;
}

//Cursor

void ui_textbox::cursor_position(unsigned int _Cursor_position) {

	if (this->_Cursor_position!=_Cursor_position) {
	
		if (_Cursor_position>_Text_data.size())
			this->_Cursor_position=(unsigned int)_Text_data.size();
		else this->_Cursor_position=_Cursor_position;
		
		if ((_Left_bound>0 && this->_Cursor_position<_Left_bound) ||
			(_Right_bound>0 && this->_Cursor_position>_Right_bound))
				_Update_text=true;
		else _Update_cursor=true;
	}
}

unsigned int ui_textbox::cursor_position() const {
	return _Cursor_position;
}

void ui_textbox::reveal_distance(unsigned int _Reveal_distance) {
	this->_Reveal_distance=_Reveal_distance;
}

unsigned int ui_textbox::reveal_distance() const {
	return _Reveal_distance;
}

void ui_textbox::repeat_rate(float _Repeat_rate) {
	this->_Repeat_rate=_Repeat_rate;
}

float ui_textbox::repeat_rate() const {
	return _Repeat_rate;
}

void ui_textbox::repeat_delay(float _Repeat_delay) {
	this->_Repeat_delay=_Repeat_delay;
}

float ui_textbox::repeat_delay() const {
	return _Repeat_delay;
}

void ui_textbox::blink_rate(float _Blink_rate) {
	this->_Blink_rate=_Blink_rate;
}

float ui_textbox::blink_rate() const {
	return _Blink_rate;
}

void ui_textbox::blink_hold(float _Blink_hold) {

	//Percentage of blink rate
	if (_Blink_hold>1.0f) this->_Blink_hold=1.0f;
	else if (_Blink_hold<0.0f) this->_Blink_hold=0.0f;
	else this->_Blink_hold=_Blink_hold;
}

float ui_textbox::blink_hold() const {
	return _Blink_hold;
}

//Clipboard

std::string ui_textbox::cut_text(unsigned int _From_position,
	unsigned int _To_position) {

	//Nothing to cut
	if (_From_position>=_To_position ||
		_From_position>=_Text_data.size()) return "";

std::string _Clipboard=copy_text(_From_position,_To_position);
_Text_data.erase(_From_position,_To_position-_From_position);

_Cursor_position=_From_position;
_Update_text=true;
return _Clipboard;
}

std::string ui_textbox::copy_text(unsigned int _From_position,
	unsigned int _To_position) const {

	//Nothing to copy
	if (_From_position>=_To_position ||
		_From_position>=_Text_data.size()) return "";

std::string _Data=_Text_data.substr(_From_position,
	_To_position-_From_position);
std::string _Clipboard=(_Mask==0?_Data:mask_data(_Data,_Mask));
system::clipboard(_Clipboard);
return _Clipboard;
}

void ui_textbox::paste_text(unsigned int _From_position,
	unsigned int _To_position) {
	insert_data(system::clipboard(),_From_position,_To_position);
}