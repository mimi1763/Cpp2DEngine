/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: input_manager.h
---------------------------------------------------
*/

#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_

#include "listener_interface.h"

namespace lunar {

//Forward declarations
class core;

class input_manager:protected listener_interface<key_listener*>,
	protected listener_interface<mouse_listener*>,public message_listener {

	friend core;

private:

	static input_manager *_Instance; //Grant core exclusive access

protected:

	struct _Active_code {
	
	unsigned int _Code;
	bool _Mouse;
	};
	
	typedef std::list<_Active_code> _Active_codes;
	
	_Active_codes _Myactive_codes;
	
	unsigned int _Xmouse;
	unsigned int _Ymouse;
	unsigned int _Glheight;
	
	//Protected constructor
	input_manager();
	
	//Protected copy constructor
	input_manager(const input_manager &_Input_manager);
	
	//Protected virtual destructor
	virtual ~input_manager();
	
	input_manager &operator=(const input_manager &_Input_manager);
	
	void copy(const input_manager &_Input_manager);
	
	virtual bool message(HWND _Window_handle,UINT _Message,
		WPARAM _Wparam,LPARAM _Lparam);
	
	void release();
	
	void key_pressed(unsigned int _Key);
	void key_released(unsigned int _Key);
	void char_pressed(unsigned char _Character);
	
	void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	void mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	void mouse_moved(unsigned int _X,unsigned int _Y);
	
	void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);

public:

	//Base class typedefs to solve ambiguousness
	typedef listener_interface<key_listener*> key_listeners; 
	typedef listener_interface<mouse_listener*> mouse_listeners;
	
	//Redefines for public access
	typedef key_listeners::listeners_iter keylisteners_iter;
	typedef mouse_listeners::listeners_iter mouselisteners_iter;
	
	static input_manager *instance();
	
	bool key_active(unsigned int _Key) const;
	bool mouse_active(unsigned int _Button) const;
	
	unsigned int mouse_x() const;
	unsigned int mouse_y() const;
	
	//Listeners
	
	void subscribe_keylistener(key_listener *_Key_listener,
		bool _Secured=false);
	void subscribe_mouselistener(mouse_listener *_Mouse_listener,
		bool _Secured=false);
	
	void unsubscribe_keylistener(key_listener *_Key_listener);
	void unsubscribe_mouselistener(mouse_listener *_Mouse_listener);
	
	void clear_keylisteners();
	void clear_mouselisteners();
	
	keylisteners_iter keylisteners_begin() const;
	keylisteners_iter keylisteners_end() const;
	
	mouselisteners_iter mouselisteners_begin() const;
	mouselisteners_iter mouselisteners_end() const;
};

} //namespace lunar

#endif