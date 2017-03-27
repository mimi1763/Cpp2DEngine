/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: core.h
---------------------------------------------------
*/

#ifndef _CORE_
#define _CORE_

#include <string>

#include "timeline.h"
#include "listener_interface.h"
#include "render_window.h"

namespace lunar {

//Forward declarations
class render_window;

class core:protected listener_interface<frame_listener*> {

protected:

	static bool _Engine_initialized;
	
	bool _Initialized;
	float _Frame_limit;
	bool _Syncronize;
	
	float _Frame_time;
	stopwatch _Stopwatch;
	
	//Protected copy constructor
	core(const core &_Core);
	
	core &operator=(const core &_Core);
	
	void copy(const core &_Core);
	
	static void extract_programparam();
	static void init_opengl();
	
	bool update();

public:

	typedef listeners_iter framelisteners_iter; //Keep data safe
	
	//Constructor
	core();
	
	//Virtual destructor
	virtual ~core();
	
	bool init();
	int start();
	
	void frame_limit(float _Frame_limit);
	float frame_limit() const;

	void vertical_sync(bool _Vertical_sync);
	bool vertical_sync() const;
	
	void sync();

	float frame_time() const;
	float fps() const;
	
	void clear_cache() const;
	
	render_window *create_renderwindow(const std::string &_Title,
		unsigned int _Width=800,unsigned int _Height=600,
			unsigned int _Color_depth=32,bool _Fullscreen=true);
	
	void subscribe_framelistener(frame_listener *_Frame_listener,
		bool _Secured=false);
	
	void unsubscribe_framelistener(frame_listener *_Frame_listener);
	void clear_framelisteners();
	
	framelisteners_iter framelisteners_begin() const;
	framelisteners_iter framelisteners_end() const;
};

} //namespace lunar

#endif