/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: thread_manager.h
---------------------------------------------------
*/

#ifndef _THREAD_MANAGER_
#define _THREAD_MANAGER_

#include "manager.h"
#include "thread.h"

namespace lunar {

//Forward declarations
class core;

class thread_manager:public manager {

	friend core;
	
private:

	static thread_manager *_Instance; //Grant core exclusive access

protected:

	pthread_attr_t _Thread_attribute;
	pthread_mutex_t _Thread_mutex;
	bool _Supported;
	
	void copy(const thread_manager &_Thread_manager);
	
	//Pthread specific
	bool init_pthread();
	bool init_thread(thread *_Thread) const;

public:

	//Constructor
	thread_manager();
	
	//Copy constructor
	thread_manager(const thread_manager &_Thread_manager);
	
	//Virtual destructor
	~thread_manager();
	
	thread_manager &operator=(const thread_manager &_Thread_manager);
	
	static thread_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	bool mutex_lock();
	bool mutex_trylock();
	bool mutex_unlock();
	
	//Thread functions
	
	thread *create_thread(thread::entry_point _Entry_point,void *_Argument=0,
		bool _Secured=false);
	
	thread *create_thread(const thread &_Thread,bool _Secured=false);
};

} //namespace lunar

#endif