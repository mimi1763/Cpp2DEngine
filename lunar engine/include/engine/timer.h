/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timer.h
---------------------------------------------------
*/

#ifndef _TIMER_
#define _TIMER_

#include "manageable.h"
#include "callback.h"

namespace lunar {

//Forward declarations
class timer_manager;

class timer:public manageable {

	friend timer_manager;

protected:

	float _Interval;
	float _Time;
	bool _Enabled;
	bool _Ticked;
	
	callback<timer*> *_Ontick;
	
	//Protected constructor
	timer(timer_manager *_Owner,float _Interval,bool _Enabled=true);
	
	//Protected copy constructor
	timer(const timer &_Timer);
	
	//Protected alternative copy constructor
	timer(timer_manager *_Owner,const timer &_Timer);
	
	//Protected virtual destructor
	virtual ~timer();
	
	void copy(const timer &_Timer);

public:

	timer &operator=(const timer &_Timer);
	
	bool operator==(const timer &_Timer) const;
	bool operator!=(const timer &_Timer) const;
	
	void start();
	void stop(bool _Reset=true);
	
	void interval(float _Interval);
	float interval() const;
	
	float time() const;
	
	bool enabled() const;
	bool ticked();
	
	//Callbacks
	
	template <typename _Ty>
	void ontick(typename member_callback<_Ty,timer*>::function _Function,
		_Ty *_Class) {
	
		if (_Ontick) release_ontick();
	
	_Ontick=new member_callback<_Ty,timer*>(_Function,_Class);
	}
	
	void ontick(static_callback<timer*>::function _Function);
	
	void release_ontick();
};

} //namespace lunar

#endif