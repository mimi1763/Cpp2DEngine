/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: input_manager.cpp
---------------------------------------------------
*/

#include "input_manager.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
input_manager *input_manager::_Instance=0;

//Protected

input_manager::input_manager():_Xmouse(0),_Ymouse(0),_Glheight(0) {
	//Empty
}

input_manager::input_manager(const input_manager &_Input_manager) {
	copy(_Input_manager);
}

input_manager::~input_manager() {
	//Empty
}

input_manager &input_manager::operator=(const input_manager &_Input_manager) {

	//Worth doing
	if (this!=&_Input_manager) copy(_Input_manager);

return *this;
}

void input_manager::copy(const input_manager &_Input_manager) {

_Xmouse=_Xmouse;
_Ymouse=_Ymouse;
_Glheight=_Glheight;
}

bool input_manager::message(HWND _Window_handle,UINT _Message,
	WPARAM _Wparam,LPARAM _Lparam) {

	switch (_Message) {
	
		case WM_ACTIVATE:
		
			//Release when deactivated
			if (LOWORD(_Wparam)==0) release();
		
		return true;
		
		case WM_ENTERSIZEMOVE:
		release();
		return true;
		
		//Use custom message instead of WM_SIZE
		case WM_GLSIZE:
		_Glheight=HIWORD(_Lparam); //Need only height to flip y axis
		return true;
		
		case WM_KEYDOWN:
		
			//Don't register repeated keydowns
			if ((_Lparam & 0x40000000)==0) key_pressed(_Wparam);
		
		return true;
		
		case WM_KEYUP:
		key_released(_Wparam);
		return true;
		
		case WM_CHAR:
		char_pressed((const unsigned char)_Wparam);
		return true;
		
		case WM_LBUTTONDOWN:
		mouse_pressed(0,LOWORD(_Lparam),_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
		
		case WM_RBUTTONDOWN:
		mouse_pressed(1,LOWORD(_Lparam),_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
		
		case WM_LBUTTONUP:
		mouse_released(0,LOWORD(_Lparam),_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
		
		case WM_RBUTTONUP:
		mouse_released(1,LOWORD(_Lparam),_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
		
		case WM_MOUSEMOVE:
		mouse_moved(LOWORD(_Lparam),_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
		
		case WM_MOUSEWHEEL:
		mouse_scrolled((short)HIWORD(_Wparam),LOWORD(_Lparam),
			_Glheight-HIWORD(_Lparam)); //Adjust y axis
		return true;
	}

return false;
}

void input_manager::release() {

	for (_Active_codes::iterator _Iter=_Myactive_codes.begin(),
		_End=_Myactive_codes.end();_Iter!=_End;++_Iter) {
	
	_Active_code &_Myactive_code=(*_Iter); //Increase speed
	
		if (_Myactive_code._Mouse) {
		
			//Release at last registered coordinates
			for (mouse_listeners::_Listeners::const_iterator _Miter=
				mouse_listeners::_Mylisteners.begin(),_Mend=mouse_listeners::
					_Mylisteners.end();_Miter!=_Mend;++_Miter) (*_Miter)->
						mouse_released(_Myactive_code._Code,_Xmouse,_Ymouse);
		}
		else {
		
			for (key_listeners::_Listeners::const_iterator _Kiter=
				key_listeners::_Mylisteners.begin(),_Kend=key_listeners::
					_Mylisteners.end();_Kiter!=_Kend;++_Kiter) (*_Kiter)->
						key_released(_Myactive_code._Code);
		}
	}

_Myactive_codes.clear();
}

void input_manager::key_pressed(unsigned int _Key) {

_Active_code _Myactive_code;
_Myactive_code._Code=_Key;
_Myactive_code._Mouse=false;
_Myactive_codes.push_front(_Myactive_code); //Is last pressed first released?

	for (key_listeners::_Listeners::const_iterator _Iter=key_listeners::
		_Mylisteners.begin(),_End=key_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->key_pressed(_Key);
}

void input_manager::key_released(unsigned int _Key) {

bool _Pressed=false;

	for (_Active_codes::iterator _Iter=_Myactive_codes.begin(),
		_End=_Myactive_codes.end();_Iter!=_End;++_Iter) {
	
	_Active_code &_Myactive_code=(*_Iter); //Increase speed
	
		if (_Myactive_code._Code==_Key && !_Myactive_code._Mouse) {
		
		_Myactive_codes.erase(_Iter);
		_Pressed=true;
		break;
		}
	}
	
	//Only release a key that has been pressed, else discard
	if (!_Pressed) return;
	
	for (key_listeners::_Listeners::const_iterator _Iter=key_listeners::
		_Mylisteners.begin(),_End=key_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->key_released(_Key);
}

void input_manager::char_pressed(unsigned char _Character) {

	//Accept only printable ASCII character codes
	if (_Character>=32 && _Character!=127) {
	
		for (key_listeners::_Listeners::const_iterator _Iter=key_listeners::
			_Mylisteners.begin(),_End=key_listeners::_Mylisteners.end();
				_Iter!=_End;++_Iter) (*_Iter)->char_pressed(_Character);
	}
}

void input_manager::mouse_pressed(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

_Active_code _Myactive_code;
_Myactive_code._Code=_Button;
_Myactive_code._Mouse=true;
_Myactive_codes.push_front(_Myactive_code); //Is last pressed first released?

	for (mouse_listeners::_Listeners::const_iterator _Iter=mouse_listeners::
		_Mylisteners.begin(),_End=mouse_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->mouse_pressed(_Button,_X,_Y);
}

void input_manager::mouse_released(unsigned int _Button,
	unsigned int _X,unsigned int _Y) {

bool _Pressed=false;

	for (_Active_codes::iterator _Iter=_Myactive_codes.begin(),
		_End=_Myactive_codes.end();_Iter!=_End;++_Iter) {
	
	_Active_code &_Myactive_code=(*_Iter); //Increase speed
	
		if (_Myactive_code._Code==_Button && _Myactive_code._Mouse) {
		
		_Myactive_codes.erase(_Iter);
		_Pressed=true;
		break;
		}
	}
	
	//Only release a button that has been pressed, else discard
	if (!_Pressed) return;
	
	for (mouse_listeners::_Listeners::const_iterator _Iter=mouse_listeners::
		_Mylisteners.begin(),_End=mouse_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->mouse_released(_Button,_X,_Y);
}

void input_manager::mouse_moved(unsigned int _X,
	unsigned int _Y) {

_Xmouse=_X;
_Ymouse=_Y;

	for (mouse_listeners::_Listeners::const_iterator _Iter=mouse_listeners::
		_Mylisteners.begin(),_End=mouse_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->mouse_moved(_X,_Y);
}

void input_manager::mouse_scrolled(int _Delta,
	unsigned int _X,unsigned int _Y) {

	for (mouse_listeners::_Listeners::const_iterator _Iter=mouse_listeners::
		_Mylisteners.begin(),_End=mouse_listeners::_Mylisteners.end();
			_Iter!=_End;++_Iter) (*_Iter)->mouse_scrolled(_Delta,_X,_Y);
}

//Public

input_manager *input_manager::instance() {
	return _Instance;
}

bool input_manager::key_active(unsigned int _Key) const {

	for (_Active_codes::const_iterator _Iter=_Myactive_codes.begin(),
		_End=_Myactive_codes.end();_Iter!=_End;++_Iter) {
	
	const _Active_code &_Myactive_code=(*_Iter); //Increase speed
	
		if (_Myactive_code._Code==_Key && !_Myactive_code._Mouse)
			return true;
	}

return false;
}

bool input_manager::mouse_active(unsigned int _Button) const {

	for (_Active_codes::const_iterator _Iter=_Myactive_codes.begin(),
		_End=_Myactive_codes.end();_Iter!=_End;++_Iter) {
	
	const _Active_code &_Myactive_code=(*_Iter); //Increase speed
	
		if (_Myactive_code._Code==_Button && _Myactive_code._Mouse)
			return true;
	}

return false;
}

unsigned int input_manager::mouse_x() const {
	return _Xmouse;
}

unsigned int input_manager::mouse_y() const {
	return _Ymouse;
}

//Listeners

void input_manager::subscribe_keylistener(key_listener *_Key_listener,
	bool _Secured) {
	key_listeners::subscribe(_Key_listener,_Secured);
}

void input_manager::subscribe_mouselistener(mouse_listener *_Mouse_listener,
	bool _Secured) {
	mouse_listeners::subscribe(_Mouse_listener,_Secured);
}

void input_manager::unsubscribe_keylistener(key_listener *_Key_listener) {
	key_listeners::unsubscribe(_Key_listener);
}

void input_manager::unsubscribe_mouselistener(mouse_listener *_Mouse_listener) {
	mouse_listeners::unsubscribe(_Mouse_listener);
}

void input_manager::clear_keylisteners() {
	key_listeners::clear_listeners();
}

void input_manager::clear_mouselisteners() {
	mouse_listeners::clear_listeners();
}

input_manager::keylisteners_iter input_manager::keylisteners_begin() const {
	return key_listeners::listeners_begin();
}

input_manager::keylisteners_iter input_manager::keylisteners_end() const {
	return key_listeners::listeners_end();
}

input_manager::mouselisteners_iter input_manager::mouselisteners_begin() const {
	return mouse_listeners::listeners_begin();
}

input_manager::mouselisteners_iter input_manager::mouselisteners_end() const {
	return mouse_listeners::listeners_end();
}

} //namespace lunar