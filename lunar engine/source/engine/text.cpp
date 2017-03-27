/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: text.cpp
---------------------------------------------------
*/

#include "text.h"

//Dependencies
#include <cmath>
#include "system.h"

#include "text_manager.h"
#include "font.h"

namespace lunar {

//Public

//Style
//Private

void text::style::copy(const style &_Style) {

	if (_Style._Color) color_value(*_Style._Color);
	else if (_Color) {
	
	delete _Color;
	_Color=0;
	}
}

void text::style::color_value(const color &_Color) {

	if (!this->_Color) this->_Color=new color(_Color);
	else *this->_Color=_Color;
}

//Style
//Public

text::style::style():_Color(0) {
	//Empty
}

text::style::style(const style &_Style) {

_Color=0; //Not copyable
copy(_Style);
}

text::style::~style() {

	if (_Color) delete _Color;
}

text::style &text::style::operator=(const style &_Style) {

	//Worth doing
	if (this!=&_Style) copy(_Style);

return *this;
}

color *text::style::color_value() const {
	return _Color;
}

//Element
//Public

text::element::element():_Width(0) {
	//Empty
}

text::element::~element() {
	//Empty
}

const std::string &text::element::data() const {
	return _Data;
}

unsigned int text::element::width() const {
	return _Width;
}

const text::style &text::element::element_style() const {
	return _Style;
}

//Line
//Public

text::line::line():_Width(0) {
	//Constructor
}

text::line::~line() {
	//Destructor
}

std::string text::line::data() const {

std::string _Data;

	for (_Elements::const_iterator _Iter=_Myelements.begin(),
		_End=_Myelements.end();_Iter!=_End;++_Iter)
			_Data+=(*_Iter).data();

return _Data;
}

unsigned int text::line::width() const {
	return _Width;
}

text::line::elements_iter text::line::elements_begin() const {
	return _Myelements.begin();
}

text::line::elements_iter text::line::elements_end() const {
	return _Myelements.end();
}

//Text
//Protected

text::text(text_manager *_Owner,const std::string &_Text_data,font *_Font,
	const color &_Color,bool _Visible):manageable(_Owner,0),
		attachable(_Visible),_Text_data(_Text_data),_Alignment(LEFT),
			_Width(0),_Font(_Font),_Color(_Color),_Wrap_width(0),
				_Styles_enabled(false),_Line_space(0),_Max_lines(0),
					_Skip_lines(0) {
	_Font->subscribe(this,true); //Add secure listener
}

text::text(const text &_Text):manageable(_Text),attachable(_Text) {

_Font=0; //No font
copy(_Text);

	//Update bounds
	if (_Status==OKAY) _Update_bounds=true;
}

text::text(text_manager *_Owner,const text &_Text,font *_Font):
	manageable(_Owner,_Text),attachable(_Text),_Font(_Font) {

_Font->subscribe(this,true); //Add secure listener
copy(_Text);

	//Update bounds
	if (_Status==OKAY) _Update_bounds=true;
}

text::~text() {

_Listening=false; //Ignore notifications

	if (_Font) _Font->owner()->remove(_Font);
}

void text::copy(const text &_Text) {

_Text_data=_Text._Text_data;
_Alignment=_Text._Alignment;
_Width=_Text._Width;

_Color=_Text._Color;
_Wrap_width=_Text._Wrap_width;
_Styles_enabled=_Text._Styles_enabled;

_Line_space=_Text._Line_space;
_Max_lines=_Text._Max_lines;
_Skip_lines=_Text._Skip_lines;
_Mylines=_Text._Mylines;

status(_Text._Status); //Inherit status
}

void text::status_changed() {

manageable::status_changed(); //Use base functionality

	//Update bounds
	if (_Status==OKAY) _Update_bounds=true;
}

void text::manageable_changed(manageable *_Manageable) {

//Font has been changed, notify text

	//Update explicit (not changed)
	if (_Font==_Manageable) status(NEED_UPDATE);
	//Use base functionality
	else attachable::manageable_changed(_Manageable);
}

void text::manageable_statuschanged(manageable *_Manageable) {

//Font status has been changed, notify text
	
	if (_Manageable->status()==NEED_UPDATE) {
	
		//Update explicit (not changed)
		if (_Font==_Manageable) status(NEED_UPDATE);
		//Use base functionality
		else attachable::manageable_statuschanged(_Manageable);
	}
}

void text::manageable_removed(manageable *_Manageable) {

//Font has been destroyed, notify text

	if (_Font==_Manageable) {
	
	_Font=0;
	status(FAILED); //Invalidate text
	}
	//Use base functionality
	else attachable::manageable_removed(_Manageable);
}

void text::update_bounds() {

float _Width=(float)(_Wrap_width>0?_Wrap_width:width());
float _Height=(float)height();

vector2 _Min(0.0f,-_Height);
vector2 _Max(_Width,0.0f);

	if (_Alignment==text::CENTER) {
	
	_Min.x+=floor(_Wrap_width*0.5f-_Width*0.5f);
	_Max.x+=floor(_Wrap_width*0.5f-_Width*0.5f);
	}
	else if (_Alignment==text::RIGHT) {
	
	_Min.x+=_Wrap_width-_Width;
	_Max.x+=_Wrap_width-_Width;
	}

create_bounds(_Min,_Max);
}

void text::pre_draw() {

glEnable(GL_TEXTURE_2D);

glListBase((GLuint)_Font->listbase());
glColor4fv(_Color.ptr);
}

void text::draw() {

	//Nothing to draw
	if (_Skip_lines>=_Mylines.size()) return;

float _Width=(float)_Wrap_width;
float _Line_height=(float)line_height();
float _Line_distance=(float)line_distance();

float _Height=0.0f;
unsigned int _Count=0;

glPushMatrix();

	for (_Lines::const_iterator _Iter=_Mylines.begin(),
		_End=_Mylines.end();_Iter!=_End;++_Iter,++_Count) {
	
		//Skip line
		if (_Count<_Skip_lines) continue;
		
		//Max lines reached
		if (_Max_lines>0 && _Count>=_Max_lines+_Skip_lines) break;
	
	const line &_Line=(*_Iter); //Increase speed
	
	glPushMatrix();
	glTranslatef(0.0f,-_Height,0.0f);
	
		if (_Alignment==text::LEFT) glTranslatef(0.0f,-_Line_height,0.0f);
		else if (_Alignment==text::CENTER) glTranslatef(floor( //Floor value (blur fix)
			_Width*0.5f-(float)_Line._Width*0.5f),-_Line_height,0.0f);
		else if (_Alignment==text::RIGHT) glTranslatef(
			_Width-(float)_Line._Width,-_Line_height,0.0f);
		
		for (_Elements::const_iterator _Eiter=_Line._Myelements.begin(),
			_Eend=_Line._Myelements.end();_Eiter!=_Eend;++_Eiter) {
		
		const element &_Element=(*_Eiter); //Increase speed
		
			if (_Styles_enabled) {
			
				//Use style color if specified
				if (_Element._Style.color_value()) glColor4fv(
					_Element._Style.color_value()->ptr);
				//Use default color
				else glColor4fv(_Color.ptr);
			}
		
		glCallLists((GLsizei)_Element._Data.size(),GL_UNSIGNED_BYTE,
			_Element._Data.c_str());
		}
	
	glPopMatrix();
	_Height+=_Line_distance;
	}

glPopMatrix();
}

void text::post_draw() {
	glDisable(GL_TEXTURE_2D);
}

//Public

text &text::operator=(const text &_Text) {

	//Worth doing
	if (this!=&_Text) {
	
	manageable::operator=(_Text); //Invoke base operator=
	attachable::operator=(_Text); //Invoke base operator=
	
	bool _Changed=(_Text_data!=_Text._Text_data);
	copy(_Text);
	
		//Copy text font
		if (_Font && _Text._Font)
			(*_Font)=(*_Text._Font);
		
		//Update explicit
		if (_Changed) changed();
		
		//Update bounds
		if (_Status==OKAY) _Update_bounds=true;
	}

return *this;
}

bool text::operator==(const text &_Text) const {
	return (_Text_data==_Text._Text_data); //Redefine equality
}

bool text::operator!=(const text &_Text) const {
	return !(*this==_Text); //Redefine inequality
}

bool text::prepare() {

	//Update if needed (increase speed)
	if (_Status==NEED_UPDATE) update();

//Don't draw if update failed
return (_Status==OKAY?attachable::prepare():false);
}

attachable::geometry_stats text::statistics() const {

geometry_stats _Geometry_stats={0};
unsigned int _Count=0;

	for (_Lines::const_iterator _Iter=_Mylines.begin(),
		_End=_Mylines.end();_Iter!=_End;++_Iter,++_Count) {
	
		//Skip line
		if (_Count<_Skip_lines) continue;
		
		//Max lines reached
		if (_Max_lines>0 && _Count>=_Max_lines+_Skip_lines) break;
	
	const line &_Line=(*_Iter); //Increase speed
	
		for (_Elements::const_iterator _Eiter=_Line._Myelements.begin(),
			_Eend=_Line._Myelements.end();_Eiter!=_Eend;++_Eiter)
				_Geometry_stats.polygons+=(unsigned int)
					(*_Eiter)._Data.size();
	}

_Geometry_stats.vertices=_Geometry_stats.polygons*4;
return _Geometry_stats;
}

//Common functions

void text::text_data(const std::string &_Text_data) {

	if (this->_Text_data!=_Text_data) {
	
	this->_Text_data=_Text_data;
	changed(); //Update explicit
	}
}

const std::string &text::text_data() const {
	return _Text_data;
}

void text::alignment(const ALIGNMENT &_Alignment) {

	if (this->_Alignment!=_Alignment) {
	
	this->_Alignment=_Alignment;
	
		//Update bounds
		if (_Status==OKAY) _Update_bounds=true;
	}
}

text::ALIGNMENT text::alignment() const {
	return _Alignment;
}

unsigned int text::width() const {

unsigned int _Width=0;

	for (_Lines::const_iterator _Iter=_Mylines.begin(),
		_End=_Mylines.end();_Iter!=_End;++_Iter) {
	
	unsigned int _Line_width=(*_Iter).width();
	
		if (_Line_width>_Width) _Width=_Line_width;
	}

return (_Width>0?_Width:this->_Width);
}

unsigned int text::height() const {

unsigned int _Line_height=line_height();
unsigned int _Line_count=line_count(true);
int _Line_space=(this->_Line_space<-(int)_Line_height?
	-(int)_Line_height:this->_Line_space);

return (_Line_count>1?_Line_height*_Line_count+
	_Line_space*(_Line_count-1):_Line_height);
}

unsigned int text::line_height() const {
	return (_Font?_Font->height():0);
}

unsigned int text::line_count(bool _Only_visible) const {

unsigned int _Size=(unsigned int)_Mylines.size();

	if (_Only_visible) {
	
	_Size=(_Skip_lines<_Size?_Size-_Skip_lines:0);
	_Size=(_Max_lines==0?_Size:(_Size>_Max_lines?_Max_lines:_Size));
	}

return _Size;
}

//Format functions

font *text::text_font() const {
	return _Font;
}

void text::color_value(const color &_Color) {
	this->_Color=_Color;
}

const color &text::color_value() const {
	return _Color;
}

void text::wrap_width(unsigned int _Wrap_width) {

	if (this->_Wrap_width!=_Wrap_width) {
	
	this->_Wrap_width=_Wrap_width;
	status(NEED_UPDATE); //Update explicit (not changed)
	}
}

unsigned int text::wrap_width() const {
	return _Wrap_width;
}

void text::styles_enabled(bool _Enabled) {

	if (_Styles_enabled!=_Enabled) {
	
	_Styles_enabled=_Enabled;
	status(NEED_UPDATE); //Update explicit (not changed)
	}
}

bool text::styles_enabled() const {
	return _Styles_enabled;
}

//Line functions

void text::line_space(int _Line_space) {

	if (this->_Line_space!=_Line_space) {
	
	this->_Line_space=_Line_space;
	
		//Update bounds
		if (_Status==OKAY) _Update_bounds=true;
	}
}

int text::line_space() const {
	return _Line_space;
}

void text::max_lines(unsigned int _Max_lines) {

	if (this->_Max_lines!=_Max_lines) {
	
	this->_Max_lines=_Max_lines;
	
		//Update bounds
		if (_Status==OKAY) _Update_bounds=true;
	}
}

unsigned int text::max_lines() const {
	return _Max_lines;
}

void text::skip_lines(unsigned int _Skip_lines) {

	if (this->_Skip_lines!=_Skip_lines) {
	
	this->_Skip_lines=_Skip_lines;
	
		//Update bounds
		if (_Status==OKAY) _Update_bounds=true;
	}
}

unsigned int text::skip_lines() const {
	return _Skip_lines;
}

unsigned int text::line_capacity(unsigned int _Height) const {

unsigned int _Line_height=line_height();

	if (_Line_height==0) return 0;

unsigned int _Line_distance=line_distance();

	if (_Line_distance==0) {
	
		if (_Height>=_Line_height) return -1;
	
	return 0;
	}

return (unsigned int)((float)_Height/_Line_distance+(float)
	((int)_Line_distance-(int)_Line_height)/_Line_distance);
}

unsigned int text::line_distance() const {

unsigned int _Line_height=line_height();
return (_Line_space<-(int)_Line_height?0:_Line_height+_Line_space);
}

text::lines_iter text::lines_begin() const {
	return _Mylines.begin();
}

text::lines_iter text::lines_end() const {
	return _Mylines.end();
}

} //namespace lunar