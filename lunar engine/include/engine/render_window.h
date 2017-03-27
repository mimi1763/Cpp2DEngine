/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_window.h
---------------------------------------------------
*/

#ifndef _RENDER_WINDOW_
#define _RENDER_WINDOW_

#include <string>
#include "listener_interface.h"

namespace lunar {

//Forward declarations
class core;

class render_window:protected listener_interface<message_listener*> {

	friend core;
	friend system;

private:

	static render_window *_Instance; //Grant core exclusive access

protected:

	HINSTANCE _Instance_handle;
	HWND _Window_handle;
	HDC _Device_context;
	HGLRC _Rendering_context;
	DEVMODE _Device_mode;
	
	std::string _Title;
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _Color_depth;
	bool _Fullscreen;
	
	int _Left;
	int _Top;
	bool _Active;
	
	float _Stretch_width;
	float _Stretch_height;
	unsigned int _Min_width;
	unsigned int _Min_height;
	
	bool _Resizable;
	bool _Stretch;
	bool _Mouse_visible;
	bool _Mode_lock;
	
	int _Windowed_left;
	int _Windowed_top;
	unsigned int _Windowed_width;
	unsigned int _Windowed_height;
	
	//Protected constructor
	explicit render_window(const std::string &_Title,unsigned int _Width=800,
		unsigned int _Height=600,unsigned int _Color_depth=32,
			bool _Fullscreen=true);
	
	//Protected copy constructor
	render_window(const render_window &_Render_window);
	
	//Protected virtual destructor
	virtual ~render_window();
	
	render_window &operator=(const render_window &_Render_window);
	
	void copy(const render_window &_Render_window);
	
	//Static window procedure function, message handler
	static LRESULT CALLBACK static_windowproc(HWND _Window_handle,
		UINT _Message,WPARAM _Wparam,LPARAM _Lparam);
	
	//Member window procedure function, message handler
	LRESULT CALLBACK windowproc(HWND _Window_handle,UINT _Message,
		WPARAM _Wparam,LPARAM _Lparam);
	
	bool create_glwindow();
	void close_glwindow();
	
	void resize_opengl(unsigned int _Width,unsigned int _Height);
	
	bool init();
	
	//Window style specific
	
	inline unsigned int windowed_style() const {
		return (_Resizable?WS_OVERLAPPEDWINDOW:WS_OVERLAPPEDWINDOW &
			~(WS_THICKFRAME | WS_MAXIMIZEBOX));
	}
	
	static inline unsigned int windowed_exstyle() {
		return (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	}
	
	static inline unsigned int fullscreen_style() {
		return WS_POPUP;
	}
	
	static inline unsigned int fullscreen_exstyle() {
		return WS_EX_APPWINDOW;
	}

public:

	typedef listeners_iter messagelisteners_iter; //Redefine for public access
	
	static render_window *instance();
	
	void move(unsigned int _Left,unsigned int _Top);
	void resize(unsigned int _Width,unsigned int _Height);
	
	void title(const std::string &_Title);
	const std::string &title() const;
	
	unsigned int width() const;
	unsigned int height() const;
	
	unsigned int color_depth() const;
	
	int left() const;
	int top() const;
	
	bool active() const;
	
	void fullscreen(bool _Fullscreen);
	bool fullscreen() const;
	
	void fullscreen_size(unsigned int _Width,unsigned int _Height);
	unsigned int fullscreen_width() const;
	unsigned int fullsceen_height() const;
	
	void stretch_size(float _Stretch_width,float _Stretch_height);
	float stretch_width() const;
	float stretch_height() const;
	
	void min_size(unsigned int _Min_width,unsigned int _Min_height);
	unsigned int min_width() const;
	unsigned int min_height() const;
	
	void resizable(bool _Resizable);
	bool resizable() const;
	
	void stretch(bool _Stretch);
	bool stretch() const;
	
	void mouse_visible(bool _Visible);
	bool mouse_visible() const;
	
	void mode_lock(bool _Mode_lock);
	bool mode_lock() const;
	
	float ortho_width() const;
	float ortho_height() const;
	
	void subscribe_messagelistener(message_listener *_Message_listener,
		bool _Secured=false);
	
	void unsubscribe_messagelistener(message_listener *_Message_listener);
	void clear_messagelisteners();
	
	messagelisteners_iter messagelistener_begin() const;
	messagelisteners_iter messagelistener_end() const;
};

} //namespace lunar

#endif