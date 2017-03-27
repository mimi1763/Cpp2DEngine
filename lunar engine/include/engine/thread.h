/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: thread.h
---------------------------------------------------
*/

#ifndef _THREAD_
#define _THREAD_

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

#include "manageable.h"

namespace lunar {

//Forward declarations
class thread_manager;

class thread:public manageable {

	friend thread_manager;

public:

	typedef void* (*entry_point)(void*);

protected:

	pthread_t *_Pthread;
	entry_point _Entry_point;
	void *_Argument;
	bool _Join;
	
	//Protected constructor
	thread(thread_manager *_Owner,entry_point _Entry_point,void *_Argument=0);
	
	//Protected copy constructor
	thread(const thread &_Thread);
	
	//Protected alternative copy constructor
	thread(thread_manager *_Owner,const thread &_Thread);
	
	//Protected virtual destructor
	virtual ~thread();
	
	void copy(const thread &_Thread);

public:

	thread &operator=(const thread &_Thread);
	
	bool operator==(const thread &_Thread) const;
	bool operator!=(const thread &_Thread) const;
	
	bool stop();
	
	void entrypoint(entry_point _Entry_point);
	entry_point entrypoint() const;
	
	void argument(void *_Argument);
	void *argument() const;
	
	void join(bool _Join);
	bool join() const;
};

} //namespace lunar

#endif