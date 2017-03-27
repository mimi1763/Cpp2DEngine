/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: timer_manager.h
---------------------------------------------------
*/

#ifndef _TIMER_MANAGER_
#define _TIMER_MANAGER_

#include "manager.h"
#include "timeline.h"
#include "timer.h"

namespace lunar {

//Forward declarations
class core;

class timer_manager:public manager {

	friend core;

private:

	static timer_manager *_Instance; //Grant core exclusive access

protected:

	stopwatch _Stopwatch;
	
	void copy(const timer_manager &_Timer_manager);

public:

	//Constructor
	timer_manager();
	
	//Copy constructor
	timer_manager(const timer_manager &_Timer_manager);
	
	//Virtual destructor
	virtual ~timer_manager();
	
	timer_manager &operator=(const timer_manager &_Timer_manager);
	
	//Singleton
	static timer_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void sync();
	
	//Timer functions
	
	timer *create_timer(float _Interval,bool _Enabled=true,
		bool _Secured=false);
	
	timer *create_timer(const timer &_Timer,bool _Secured=false);
};

} //namespace lunar

#endif