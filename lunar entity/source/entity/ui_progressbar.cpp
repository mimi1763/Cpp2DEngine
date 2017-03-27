/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_progressbar.cpp
---------------------------------------------------
*/

#include "ui_progressbar.h"

//Dependencies
#include <cmath>

//Dependencies (engine)
#include "input_manager.h"
#include "render_node.h"
#include "animation.h"

#include "generic_utility.h"
#include "math_utility.h"
#include "string_utility.h"

//Public

//Additional
//Public

const std::string ui_progressbar::additional::BAR="BAR";
const std::string ui_progressbar::additional::BAR_PRECISE="BAR_PRECISE";

//Progressbar
//Protected

ui_progressbar::ui_progressbar(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,float _Width,
		float _Height,bool _Visible):ui_element(
			_Owner,_Name,_Parent_node,_Draw_order,_Width,_Height,_Visible),
				_Type(NONE),_Min(0.0f),_Max(100.0f),_Progress(0.0f),_Flip(false),
					_Locked(true),_Snap_interval(5.0f),_Interpolation(OFF),
						_Interpolation_time(1.0f),_Interpolation_latency(0.5f),
							_Update_bar(true),_Interpolate_bar(false),
								_Interpolated_percent(0.0f),_Interpolated_time(0.0f) {
	//Empty
}

ui_progressbar::ui_progressbar(const ui_progressbar &_Progressbar):
	ui_element(_Progressbar) {

_Update_bar=true; //Not copyable
_Interpolate_bar=false; //Not copyable
_Interpolated_percent=0.0f; //Not copyable
_Interpolated_time=0.0f; //Not copyable

copy(_Progressbar);
}

ui_progressbar::ui_progressbar(ui_menu *_Owner,const std::string &_Name,
	render_node *_Parent_node,const interval &_Draw_order,
		const ui_progressbar &_Progressbar):ui_element(
			_Owner,_Name,_Parent_node,_Draw_order,_Progressbar) {

_Update_bar=true; //Not copyable
_Interpolate_bar=false; //Not copyable
_Interpolated_percent=0.0f; //Not copyable
_Interpolated_time=0.0f; //Not copyable

copy(_Progressbar);
}

ui_progressbar::~ui_progressbar() {
	//Empty
}

void ui_progressbar::copy(const ui_progressbar &_Progressbar) {

_Type=_Progressbar._Type;
_Min=_Progressbar._Min;
_Max=_Progressbar._Max;
_Progress=_Progressbar._Progress;

_Flip=_Progressbar._Flip;
_Locked=_Progressbar._Locked;
_Snap_interval=_Progressbar._Snap_interval;

_Interpolation=_Progressbar._Interpolation;
_Interpolation_time=_Progressbar._Interpolation_time;
_Interpolation_latency=_Progressbar._Interpolation_latency;
_Caption_format=_Progressbar._Caption_format;
}

texturable *ui_progressbar::bar_component(object *_Object) const {

	//Nothing to do
	if (!_Object) return 0;

texturable *_Texturable=0;
attachable *_Data;

	//Find first texturable component in object
	for (object::components_iter _Iter=_Object->components_begin(),
		_End=_Object->components_end();_Iter!=_End;++_Iter) {
	
	_Data=(*_Iter)->data(); //Increase speed
	
		if (_Data && (_Texturable=dynamic_cast<texturable*>(_Data)))
			break;
	}

return _Texturable;
}

void ui_progressbar::update_bar(object *_Object,float _Percent) {

texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not found
	if (!_Manageable) return;

_Manageable->update();

	//Manageable update failed
	if (_Manageable->status()!=manageable::OKAY)
		return;

//Animation type

	if (_Type==ANIMATION) {
	
	animation *_Animation=dynamic_cast<animation*>(_Texturable);
	
		//Animation found
		if (_Animation) {
		
		_Animation->stop();
		_Animation->frame((unsigned int)(_Animation->frames()*
			(_Flip?1.0f-_Percent:_Percent)));
		}
	
	return;
	}

//Horizontal/vertical type

_Texturable->size(0.0f,0.0f);
_Texturable->auto_size(false);
_Texturable->auto_crop(false);

_Texturable->cut_mode(_Flip?texturable::CUT_BOTTOM_LEFT:texturable::CUT_TOP_RIGHT);
_Texturable->crop_size(_Type==HORIZONTAL?_Texturable->texture_width()*_Percent:
	_Texturable->texture_width(),_Type==VERTICAL?_Texturable->texture_height()*_Percent:
		_Texturable->texture_height());

	if (_Flip) _Texturable->crop_position(vector2((float)_Texturable->gltexture_width()-
		_Texturable->texture_width(),(float)_Texturable->gltexture_height()-
			_Texturable->texture_height()));

_Texturable->size(_Texturable->crop_width(),_Texturable->crop_height());

//Position

vector2 _Position=-vector2(_Texturable->texture_width()-_Texturable->width(),
	_Texturable->texture_height()-_Texturable->height())*0.5f;

	if (_Flip) _Position=-_Position;

_Object->node()->position(utility::position(_Position,_Texturable->width(),
	_Texturable->height(),_Texturable->origin())+utility::position(
		vector2::ZERO,_Area->width(),_Area->height(),rectangle::CENTER,
			_Area->origin()));
}

void ui_progressbar::update_bar() {

float _Percent=percent();

	if (!_Caption_format.empty()) {
	
		//Print out in percentages
		if (_Caption_format[_Caption_format.size()-1]=='%')
			caption(string_utility::format(_Caption_format,_Percent*100.0f)+"%");
		//Print out in units
		else caption(string_utility::format(_Caption_format,_Progress)+"/"+
			string_utility::format(_Caption_format,_Max));
	}
	
	if (!_Interpolate_bar && _Interpolation!=OFF) {
	
	_Interpolate_bar=true;
	_Interpolated_time=0.0f;
	}

object *_Object=object_pointer(additional::BAR);

	//No bar object found
	if (!_Object) return;

_Object->visible(_Type!=NONE);
object *_Object_exact=object_pointer(additional::BAR_PRECISE);

	if (_Object_exact) {
	
	bool _Visible=(_Type!=NONE && _Interpolate_bar);
	_Object_exact->visible(_Visible);
	
		if (!_Visible) _Object_exact=0;
	}
	
	//Nothing more to do
	if (_Type==NONE) return;
	
	//Update bar by percent
	if (!_Interpolate_bar || _Object_exact) update_bar(
		_Object_exact?_Object_exact:_Object,_Percent);
	
	//Update bar by interpolated percent
	if (_Interpolate_bar) update_bar(_Object,_Interpolated_percent);
}

//Public

ui_progressbar &ui_progressbar::operator=(const ui_progressbar &_Progressbar) {

	//Worth doing
	if (this!=&_Progressbar) {
	
	ui_element::operator=(_Progressbar); //Invoke base operator=
	copy(_Progressbar);
	
	_Update_bar=true;
	_Interpolate_bar=false;
	}

return *this;
}

void ui_progressbar::update(float _Time) {

	if (_Update_bar && visible()) {
	
	update_bar();
	_Update_bar=false;
	}
	
	if (_Interpolate_bar) {
	
	_Interpolated_time+=_Time;
	float _Percent=percent();
	
		//Stop interpolation
		if (_Interpolated_percent==_Percent ||
			(_Interpolation==FORWARD && _Interpolated_percent>=_Percent) ||
				(_Interpolation==BACKWARD && _Interpolated_percent<=_Percent)) {
		
			if (visible())
				update_bar(object_pointer(additional::BAR),_Percent);
		
		_Interpolate_bar=false;
		_Interpolated_percent=_Percent;
		}
		//Interpolate
		else if (_Interpolated_time>_Interpolation_latency) {
		
			//Forward
			if (_Interpolated_percent<_Percent) {
			
			_Interpolated_percent+=_Time/_Interpolation_time;
			
				if (_Interpolated_percent>_Percent)
					_Interpolated_percent=_Percent;
			}
			//Backward
			else if (_Interpolated_percent>_Percent) {
			
			_Interpolated_percent-=_Time/_Interpolation_time;
			
				if (_Interpolated_percent<_Percent)
					_Interpolated_percent=_Percent;
			}
			
			if (visible())
				update_bar(object_pointer(additional::BAR),_Interpolated_percent);
		}
	}

ui_element::update(_Time); //Use base functionality
}

bool ui_progressbar::key_released(unsigned int _Key) {

	//Nothing to do
	if (_Locked || !_Focused) return false;
	
	switch (_Key) {
	
		case VK_UP:
		case VK_RIGHT: {
		
		float _Interval=(_Snap_interval>0.0f?_Snap_interval:1.0f);
		float _New_progress=floor(_Progress/_Interval)*_Interval+
			(_Flip?-_Interval:_Interval);
		
		float _Prev_progress=_Progress;
		progress(_New_progress);
		
			//Changed by user
			if (_Progress!=_Prev_progress) change();
		
		return true;
		}
		
		case VK_DOWN:
		case VK_LEFT: {
		
		float _Interval=(_Snap_interval>0.0f?_Snap_interval:1.0f);
		float _New_progress=floor(_Progress/_Interval)*_Interval+
			(_Flip?_Interval:-_Interval);
		
		float _Prev_progress=_Progress;
		progress(_New_progress);
		
			//Changed by user
			if (_Progress!=_Prev_progress) change();
		
		return true;
		}
	}

return false;
}

void ui_progressbar::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Locked || !_Focused || _Type==NONE) return;

object *_Object=object_pointer(additional::BAR);
texturable *_Texturable=bar_component(_Object);
manageable *_Manageable=dynamic_cast<manageable*>(_Texturable);

	//Manageable not updated
	if (!_Manageable || _Manageable->status()!=manageable::OKAY)
		return;

float _Percent=0.0f;

	if (_Type==HORIZONTAL || _Type==ANIMATION) {
	
	_Percent=utility::position(vector2((float)_X,0.0f),
		_Area->width(),_Area->height(),_Area->origin()).x-
			(_Object->node()->world_position().x-
				_Object->node()->position().x);
	
	unsigned int _Width=_Texturable->texture_width();
	_Percent=(_Percent+_Width*0.5f)/_Width;
	}
	else {
	
	_Percent=utility::position(vector2(0.0f,(float)_Y),
		_Area->width(),_Area->height(),_Area->origin()).y-
			(_Object->node()->world_position().y-
				_Object->node()->position().y);
	
	unsigned int _Height=_Texturable->texture_height();
	_Percent=(_Percent+_Height*0.5f)/_Height;
	}
	
	//Clamp percent
	if (_Percent<0.0f) _Percent=0.0f;
	else if (_Percent>1.0f) _Percent=1.0f;
	
	if (_Flip) _Percent=1.0f-_Percent;

float _Prev_progress=_Progress;

	if (_Snap_interval>0.0f)
		progress(math::round(math::lerp(_Min,_Max,_Percent)/
			_Snap_interval)*_Snap_interval);
	else percent(_Percent);
	
	//Changed by user
	if (_Progress!=_Prev_progress) change();
}

void ui_progressbar::mouse_moved(unsigned int _X,unsigned int _Y) {

	//Nothing to do
	if (_Locked || !_Focused || _Type==NONE) return;
	
	//Drag
	if (input_manager::instance()->mouse_active(0) ||
		input_manager::instance()->mouse_active(1))
			ui_progressbar::mouse_pressed(0,_X,_Y);
}

void ui_progressbar::mouse_scrolled(int _Delta,unsigned int _X,
	unsigned int _Y) {

	//Nothing to do
	if (_Locked || !_Focused || _Type==NONE) return;
	
	//Forward/backward
	if (_Delta!=0) key_released(_Delta>0?VK_UP:VK_DOWN);
}

bool ui_progressbar::functional() const {
	return ((!_Locked && _Type!=NONE && _Min!=_Max) ||
		ui_element::functional());
}

void ui_progressbar::type(const TYPE &_Type) {

	if (this->_Type!=_Type) {
	
	this->_Type=_Type;
	_Update_bar=true;
	}
}

ui_progressbar::TYPE ui_progressbar::type() const {
	return _Type;
}

void ui_progressbar::range(float _Min,float _Max) {

float _New_min=(_Min>_Max?_Max:_Min);
float _New_max=(_Max<_Min?_Min:_Max);

	if (this->_Min!=_New_min || this->_Max!=_New_max) {
	
	this->_Min=_New_min;
	this->_Max=_New_max;
	
		if (_Progress<_New_min) _Progress=_New_min;
		else if (_Progress>_New_max) _Progress=_New_max;
	
	_Update_bar=true;
	}
}

float ui_progressbar::min() const {
	return _Min;
}

float ui_progressbar::max() const {
	return _Max;
}

void ui_progressbar::progress(float _Progress) {

float _New_progress=_Progress;

	if (_Progress<_Min) _New_progress=_Min;
	else if (_Progress>_Max) _New_progress=_Max;
	
	if (this->_Progress!=_New_progress) {
	
	this->_Progress=_New_progress;
	_Update_bar=true;
	}
}

float ui_progressbar::progress() const {
	return _Progress;
}

void ui_progressbar::percent(float _Percent) {

float _New_percent=_Percent;

	if (_Percent<0.0f) _New_percent=0.0f;
	else if (_Percent>1.0f) _New_percent=1.0f;
	
	if (_New_percent!=percent())
		progress(math::lerp(_Min,_Max,_New_percent));
}

float ui_progressbar::percent() const {
	return (_Min!=_Max?(_Progress-_Min)/(_Max-_Min):1.0f);
}

void ui_progressbar::flip(bool _Flip) {

	if (this->_Flip!=_Flip) {
	
	this->_Flip=_Flip;
	
		if (_Type!=NONE) _Update_bar=true;
	}
}

bool ui_progressbar::flip() const {
	return _Flip;
}

void ui_progressbar::locked(bool _Locked) {

	if (this->_Locked!=_Locked) {
	
	focusable(!_Locked);
	this->_Locked=_Locked;
	}
}

bool ui_progressbar::locked() const {
	return _Locked;
}

void ui_progressbar::snap_interval(float _Snap_interval) {

	if (_Snap_interval<0.0f) this->_Snap_interval=0.0f;
	else this->_Snap_interval=_Snap_interval;
}

float ui_progressbar::snap_interval() const {
	return _Snap_interval;
}

void ui_progressbar::interpolation(const INTERPOLATION &_Interpolation) {

	if (this->_Interpolation!=_Interpolation) {
	
		if (this->_Interpolation==OFF || _Interpolation==OFF) {
		
			if (_Type!=NONE) _Update_bar=true;
		
		_Interpolate_bar=false;
		}
	
	this->_Interpolation=_Interpolation;
	}
}

ui_progressbar::INTERPOLATION ui_progressbar::interpolation() const {
	return _Interpolation;
}

void ui_progressbar::interpolation_time(float _Interpolation_time) {
	this->_Interpolation_time=_Interpolation_time;
}

float ui_progressbar::interpolation_time() const {
	return _Interpolation_time;
}

void ui_progressbar::interpolation_latency(float _Interpolation_latency) {
	this->_Interpolation_latency=_Interpolation_latency;
}

float ui_progressbar::interpolation_latency() const {
	return _Interpolation_latency;
}

void ui_progressbar::caption_format(const std::string &_Caption_format) {

	if (this->_Caption_format!=_Caption_format) {
	
	this->_Caption_format=_Caption_format;
	_Update_bar=true;
	}
}

const std::string &ui_progressbar::caption_format() const {
	return _Caption_format;
}