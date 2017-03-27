/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: listener.h
---------------------------------------------------
*/

#ifndef _LISTENER_
#define _LISTENER_

#include "system.h"

namespace lunar {

//Forward declarations
class interface_base;

class listener {

protected:

	bool _Listening;

public:

	//Constructor
	listener():_Listening(true) {
		//Empty
	}
	
	//Virtual destructor
	virtual ~listener() {
		//Empty
	}
	
	virtual void unsubscribed(interface_base *_Interface) {
		//Optional
	}
	
	bool listening() const {
		return _Listening;
	}
};

//Message

class message_listener:public listener {

protected:

public:

	//User defined messages
	static const unsigned int WM_GLSIZE=WM_USER+0;
	
	//Constructor
	message_listener() {
		//Empty
	}
	
	//Virtual destructor
	virtual ~message_listener() {
		//Empty
	}
	
	virtual bool message(HWND _Window_handle,UINT _Message,
		WPARAM _Wparam,LPARAM _Lparam)=0;
};

//Frame

class frame_listener:public listener {

protected:

public:

	//Constructor
	frame_listener() {
		//Empty
	}
	
	//Virtual destructor
	virtual ~frame_listener() {
		//Empty
	}
	
	virtual bool update(float _Time)=0;
	
	virtual bool post_update(float _Time) {
		return true; //Optional
	}
};

//Key

class key_listener:public listener {

protected:

public:

	//Constructor
	key_listener() {
		//Empty
	}
	
	//Virtual destructor
	virtual ~key_listener() {
		//Empty
	}
	
	virtual void key_pressed(unsigned int _Key)=0;
	virtual void key_released(unsigned int _Key)=0;
	
	virtual void char_pressed(char _Character) {
		//Optional
	}
};

//Mouse

class mouse_listener:public listener {

protected:

public:

	//Constructor
	mouse_listener() {
		//Empty
	}
	
	//Virtual destructor
	virtual ~mouse_listener() {
		//Empty
	}
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y)=0;
	virtual void mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y)=0;
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y)=0;
	
	//Positive delta indicates forward rotation (away from the user)
	//Negative delta indicates backward rotation (toward the user)
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y) {
		//Optional
	}
};

//Forward declarations
class manageable;

//Manageable

class manageable_listener:public listener {

protected:

public:

	//Constuctor
	manageable_listener() {
		//Empty
	}
	
	//Virtual destructor
	virtual ~manageable_listener() {
		//Empty
	}
	
	virtual void manageable_changed(manageable *_Manageable)=0;
	virtual void manageable_statuschanged(manageable *_Manageable)=0;
	virtual void manageable_removed(manageable *_Manageable)=0;
};

} //namespace lunar

#endif