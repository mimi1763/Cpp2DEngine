/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation_manager.h
---------------------------------------------------
*/

#ifndef _ANIMATION_MANAGER_
#define _ANIMATION_MANAGER_

#include "manager.h"
#include "animation.h"

namespace lunar {

//Forward declarations
class core;
class timer_manager;

class animation_manager:public manager {

	friend core;

private:

	static animation_manager *_Instance; //Grant core exclusive access

protected:

	struct _Created_animation {
	
	animation_data *_Animation_data;
	float _Framerate;
	unsigned int _Frames;
	unsigned int *_Gltexture;
	unsigned int _Texture_width;
	unsigned int _Texture_height;
	unsigned int _Gltexture_width; //Power of 2
	unsigned int _Gltexture_height; //Power of 2
	};
	
	typedef std::list<_Created_animation> _Created_animations;
	
	_Created_animations _Mycreated_animations;
	animation_container *_Animation_container;
	
	timer_manager *_Timer_manager;
	
	void copy(const animation_manager &_Animation_manager);
	
	//Ffmpeg specific
	void init_ffmpeg();
	
	bool create_ffmpeganimation(animation_data *_Animation_data);
	
	static void init_animation(animation *_Animation,
		const _Created_animation &_Mycreated_animation);
	
	static void update_frame(animation *_Animation);

public:

	//Constructor
	animation_manager();
	
	//Alternative constructor
	explicit animation_manager(timer_manager *_Timer_manager);
	
	//Copy constructor
	animation_manager(const animation_manager &_Animation_manager);
	
	//Virtual destructor
	virtual ~animation_manager();
	
	animation_manager &operator=(const animation_manager &_Animation_manager);
	
	static animation_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void animation_source(animation_container *_Animation_container);
	animation_container *animation_source() const;
	
	//Animation functions
	
	animation *create_animation(const std::string &_Name,bool _Enabled=true,
		bool _Visible=true,bool _Secured=false);
	
	animation *create_animation(const animation_data &_Animation_data,
		bool _Enabled=true,bool _Visible=true,bool _Secured=false);
	
	animation *create_animation(animation_data *_Animation_data,
		bool _Enabled=true,bool _Visible=true,bool _Secured=false);
	
	animation *create_animation(const animation &_Animation,bool _Secured=false);
	
	void clear_cache();
};

} //namespace

#endif