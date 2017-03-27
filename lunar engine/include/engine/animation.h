/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: animation.h
---------------------------------------------------
*/

#ifndef _ANIMATION_
#define _ANIMATION_

#include "manageable.h"
#include "texturable.h"
#include "animation_container.h"
#include "callback.h"

namespace lunar {

//Forward declarations
class animation_manager;
class timer;

class animation:public manageable,public texturable {

	friend animation_manager;

protected:

	float _Framerate;
	bool _Loop;
	bool _Reverse;
	
	unsigned int _Frame;
	unsigned int _Frames;
	unsigned int *_Gltexture;
	timer *_Timer;
	
	callback<animation*> *_Oncomplete;
	callback<animation*> *_Onframecomplete;
	
	//Protected constructor
	animation(animation_manager *_Owner,animation_data *_Animation_data,
		timer *_Timer,bool _Visible=true);
	
	//Protected copy constructor
	animation(const animation &_Animation);
	
	//Protected alternative copy constructor
	animation(animation_manager *_Owner,const animation &_Animation,
		timer *_Timer);
	
	//Protected virtual destructor
	virtual ~animation();
	
	void copy(const animation &_Animation);
	
	virtual void manageable_removed(manageable *_Manageable);
	
	virtual void pre_draw();
	virtual void post_draw();

public:

	animation &operator=(const animation &_Animation);
	
	virtual bool prepare();
	
	void data(animation_data *_Animation_data);
	animation_data *data() const;
	
	virtual unsigned int gltexture() const;
	
	void start();
	void stop(bool _Reset=true);
	float elapsed();
	
	void next_frame();
	void previous_frame();
	
	float progress() const;
	float frame_progress() const;
	
	void framerate(float _Framerate);
	float framerate() const;
	
	void duration(float _Duration);
	float duration() const;
	
	void loop(bool _Loop);
	bool loop() const;
	
	void reverse(bool _Reverse);
	bool reverse() const;
	
	void frame(unsigned int _Frame);
	unsigned int frame() const;
	unsigned int frames() const;
	
	timer *animation_timer() const;
	
	//Callbacks
	
	template <typename _Ty>
	void oncomplete(typename member_callback<_Ty,animation*>::
		function _Function,_Ty *_Class) {
	
		if (_Oncomplete) release_oncomplete();
	
	_Oncomplete=new member_callback<_Ty,animation*>(_Function,_Class);
	}
	
	void oncomplete(static_callback<animation*>::function _Function);
	
	template <typename _Ty>
	void onframecomplete(typename member_callback<_Ty,animation*>::
		function _Function,_Ty *_Class) {
	
		if (_Onframecomplete) release_onframecomplete();
	
	_Onframecomplete=new member_callback<_Ty,animation*>(_Function,_Class);
	}
	
	void onframecomplete(static_callback<animation*>::function _Function);
	
	void release_oncomplete();
	void release_onframecomplete();
};

} //namespace lunar

#endif