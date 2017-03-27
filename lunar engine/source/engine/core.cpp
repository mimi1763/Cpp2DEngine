/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: core.cpp
---------------------------------------------------
*/

#include "core.h"

//Dependencies
#include "system.h"

#include "file_manager.h"
#include "random_manager.h"

#include "thread_manager.h"
#include "resource_manager.h"

#include "timer_manager.h"
#include "sound_manager.h"
#include "font_manager.h"
#include "text_manager.h"

#include "texture_manager.h"
#include "animation_manager.h"
#include "framebuffer_manager.h"
#include "particlesystem_manager.h"

#include "shader_manager.h"
#include "shaderprogram_manager.h"

#include "scene_manager.h"
#include "render_manager.h"
#include "input_manager.h"

#include "support.h"
#include "math_utility.h"

namespace lunar {

//Protected

bool core::_Engine_initialized=false;

core::core(const core &_Core) {

//Not copyable
_Initialized=false;
_Frame_limit=0.0f;
_Syncronize=true;
_Frame_time=0.0f;
copy(_Core);
}

core &core::operator=(const core &_Core) {

	//Worth doing
	if (this!=&_Core) copy(_Core);

return *this;
}

void core::copy(const core &_Core) {
	//Empty - override default memcpy behaviour
}

void core::extract_programparam() {

std::string _Param=GetCommandLine();

//Set default working directory for Lunar engine
file_manager::working_directory(_Param.substr(_Param.
	find_first_not_of("\""),_Param.find_last_of("\\")));
}

void core::init_opengl() {

glShadeModel(GL_SMOOTH);

glDepthFunc(GL_LEQUAL);
glEnable(GL_DEPTH_TEST);

glAlphaFunc(GL_GREATER,0.0f); //Only draw pixels greater than 0% alpha
glEnable(GL_ALPHA_TEST);

glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
glEnable(GL_BLEND);

glClearColor(0.0f,0.0f,0.0f,0.0f);
}

bool core::update() {

float _Update_interval=_Stopwatch.elapsed().interval; //Time since last update

	if (!_Stopwatch.enabled()) _Stopwatch.start();

	//Use frame limit
	if (_Frame_limit>0.0f) {

	static float _Time_elapsed=0.0f;

		if (1.0f/_Frame_limit>=(_Time_elapsed+=_Update_interval))
			return true;

	_Frame_time=_Time_elapsed;
	_Time_elapsed=0.0f;
	}
	else
		_Frame_time=_Update_interval;

thread_manager::_Instance->update(); //Start and update pending threads
sound_manager::_Instance->update(); //Start and update pending sounds
timer_manager::_Instance->update(); //Update timers

particlesystem_manager::_Instance->update(_Frame_time); //Evolve particles

	for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
		_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
	
		if (!(*_Iter)->update(_Frame_time)) return false; //Call user update
	}

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();

render_manager::_Instance->render(); //Render scene

	if (_Syncronize) glFinish();
	else glFlush(); //Explicit flush

SwapBuffers(render_window::_Instance->_Device_context);

	for (_Listeners::const_iterator _Iter=_Mylisteners.begin(),
		_End=_Mylisteners.end();_Iter!=_End;++_Iter) {
	
		if (!(*_Iter)->post_update(_Frame_time)) return false; //Call user post update
	}

return true;
}

//Public

core::core():_Initialized(false),_Frame_limit(0.0f),
	_Syncronize(true),_Frame_time(0.0f) {
	extract_programparam();
}

core::~core() {

	//Only the initial initializer instance can cleanup instance data
	if (!_Initialized) return;
	
	if (render_window::_Instance) {
	
	delete render_window::_Instance;
	render_window::_Instance=0;
	}
	
	if (input_manager::_Instance) {
	
	delete input_manager::_Instance;
	input_manager::_Instance=0;
	}
	
	if (render_manager::_Instance) {
	
	delete render_manager::_Instance;
	render_manager::_Instance=0;
	}
	
	if (scene_manager::_Instance) {
	
	delete scene_manager::_Instance;
	scene_manager::_Instance=0;
	}
	
	if (shaderprogram_manager::_Instance) {
	
	delete shaderprogram_manager::_Instance;
	shaderprogram_manager::_Instance=0;
	}
	
	if (shader_manager::_Instance) {
	
	delete shader_manager::_Instance;
	shader_manager::_Instance=0;
	}
	
	if (particlesystem_manager::_Instance) {
	
	delete particlesystem_manager::_Instance;
	particlesystem_manager::_Instance=0;
	}
	
	if (framebuffer_manager::_Instance) {
	
	delete framebuffer_manager::_Instance;
	framebuffer_manager::_Instance=0;
	}
	
	if (animation_manager::_Instance) {
	
	delete animation_manager::_Instance;
	animation_manager::_Instance=0;
	}
	
	if (texture_manager::_Instance) {
	
	delete texture_manager::_Instance;
	texture_manager::_Instance=0;
	}
	
	if (text_manager::_Instance) {
	
	delete text_manager::_Instance;
	text_manager::_Instance=0;
	}
	
	if (font_manager::_Instance) {
	
	delete font_manager::_Instance;
	font_manager::_Instance=0;
	}
	
	if (sound_manager::_Instance) {
	
	delete sound_manager::_Instance;
	sound_manager::_Instance=0;
	}
	
	if (timer_manager::_Instance) {
	
	delete timer_manager::_Instance;
	timer_manager::_Instance=0;
	}
	
	if (resource_manager::_Instance) {
	
	delete resource_manager::_Instance;
	resource_manager::_Instance=0;
	}
	
	if (thread_manager::_Instance) {
	
	delete thread_manager::_Instance;
	thread_manager::_Instance=0;
	}

_Engine_initialized=_Initialized=false;
}

bool core::init() {

	//Already initialized
	if (_Initialized) return true;
	
	//Another core instance is initialized
	if (_Engine_initialized) return false;
	
	//Render window is not created
	if (!render_window::_Instance) return false;

//Init static data (pre initialize)
system _System;
timeline _Timeline;
random_manager _Random_manager;
math _Math;

//Create default managers
thread_manager::_Instance=new thread_manager();
resource_manager::_Instance=new resource_manager();

timer_manager::_Instance=new timer_manager();
sound_manager::_Instance=new sound_manager();
font_manager::_Instance=new font_manager();
text_manager::_Instance=new text_manager();

texture_manager::_Instance=new texture_manager();
animation_manager::_Instance=new animation_manager();
framebuffer_manager::_Instance=new framebuffer_manager();
particlesystem_manager::_Instance=new particlesystem_manager();

shader_manager::_Instance=new shader_manager();
shaderprogram_manager::_Instance=new shaderprogram_manager();

scene_manager::_Instance=new scene_manager();
render_manager::_Instance=new render_manager();
input_manager::_Instance=new input_manager();

render_window::_Instance->subscribe_messagelistener(
	input_manager::_Instance,true); //Add secure message listener

	//Initialize render window
	if (!render_window::_Instance->init()) {
	
	_Initialized=true;
	this->~core(); //Call destructor to cleanup
	return false;
	}
	
	//Init gl extension wrangler library (GLEW)
	if (glewInit()!=GLEW_OK) {
	
	_Initialized=true;
	this->~core(); //Call destructor to cleanup
	return false;
	}

//Init static data (post initialize)
support _Support; //gl functions
system::init_extensions(); //system specific extensions (wgl)

//Update timeline elapsed minimum every 60 seconds, to make sure
//elapsed is correct when not used over a very long time period
timer_manager::_Instance->create_timer(60.0f,true,true)->
	ontick(&timeline::update);

	//In case of variable CPU frequency, add secure timer
	if (timeline::high_resolution()) timer_manager::_Instance->
		create_timer(10.0f,true,true)->ontick(&timeline::frequency);

return (_Engine_initialized=_Initialized=true);
}

int core::start() {

	//Not initialized
	if (!_Initialized) return 1;
	
	//Try to force vsync on if high resolution timer is not supported
	//This will smooth out the frame time interpolation.
	if (!timeline::high_resolution()) vertical_sync(true);

//Syncronize if vsync is enabled or high resolution timer is not supported
_Syncronize=(vertical_sync() || !timeline::high_resolution());
_Stopwatch.stop();

init_opengl();

MSG _Message;

	//Main loop
	while (true) {
	
		if (PeekMessage(&_Message,0,0,0,PM_REMOVE)!=0) {
		
			if (_Message.message==WM_QUIT) break;
		
		TranslateMessage(&_Message);
		DispatchMessage(&_Message);
		}
		else if (!update()) break;
	}

return (int)_Message.wParam;
}

void core::frame_limit(float _Frame_limit) {
	this->_Frame_limit = _Frame_limit;
}

float core::frame_limit() const {
	return _Frame_limit;
}

void core::vertical_sync(bool _Vertical_sync) {

	//Not initialized
	if (!_Initialized) return;

system::swap_control(_Vertical_sync);
}

bool core::vertical_sync() const {

	//Not initialized
	if (!_Initialized) return true;

return system::swap_control();
}

void core::sync() {
	_Stopwatch.elapsed();
}

float core::frame_time() const {
	return _Frame_time;
}

float core::fps() const {
	return (1.0f/_Frame_time);
}

void core::clear_cache() const {

	//Not initialized
	if (!_Initialized) return;

//All objects that requires cached data are
//automatically updated next time they are used

sound_manager::_Instance->clear_cache();
font_manager::_Instance->clear_cache();
texture_manager::_Instance->clear_cache();
animation_manager::_Instance->clear_cache();
framebuffer_manager::_Instance->clear_cache();
shader_manager::_Instance->clear_cache();
shaderprogram_manager::_Instance->clear_cache();
}

render_window *core::create_renderwindow(const std::string &_Title,
	unsigned int _Width,unsigned int _Height,
		unsigned int _Color_depth,bool _Fullscreen) {

	//Create render window
	if (!render_window::_Instance) render_window::_Instance=new
		render_window(_Title,_Width,_Height,_Color_depth,_Fullscreen);

return render_window::_Instance;
}

void core::subscribe_framelistener(frame_listener *_Frame_listener,
	bool _Secured) {
	subscribe(_Frame_listener,_Secured);
}

void core::unsubscribe_framelistener(frame_listener *_Frame_listener) {
	unsubscribe(_Frame_listener);
}

void core::clear_framelisteners() {
	clear_listeners();
}

core::framelisteners_iter core::framelisteners_begin() const {
	return _Mylisteners.begin();
}

core::framelisteners_iter core::framelisteners_end() const {
	return _Mylisteners.end();
}

} //namespace lunar