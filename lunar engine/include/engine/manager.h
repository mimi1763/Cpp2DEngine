/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: manager.h
---------------------------------------------------
*/

#ifndef _MANAGER_
#define _MANAGER_

#include "manageable.h"

namespace lunar {

//Forward declarations
class manageable;

class manager {

protected:

	typedef std::list<manageable*> _Manageables;
	
	_Manageables _Mymanageables; //All
	_Manageables _Mypublic_manageables; //Public
	_Manageables _Mysecured_manageables; //Secured
	
	void copy(const manager &_Manager);
	void tidy();
	
	virtual void removed(manageable *_Manageable);
	
	void add(manageable *_Manageable,bool _Secured=false);
	static void status(manageable *_Manageable,const manageable::STATUS &_Status);

public:

	typedef _Manageables::const_iterator manageables_iter; //Keep data safe
	
	//Constructor
	manager();
	
	//Copy constructor
	manager(const manager &_Manager);
	
	//Virtual destructor
	virtual ~manager();
	
	manager &operator=(const manager &_Manager);
	
	virtual void update(manageable *_Manageable)=0;
	virtual void update();
	
	void remove(manageable *&_Manageable);
	
	//Support remove by reference for derived types
	template <typename _Ty>
	void remove(_Ty &_Derived) {
	
	manageable *_Manageable=_Derived;
	remove(_Manageable);
	
		//Set referenced pointer to null
		if (!_Manageable) _Derived=0;
	}
	
	void remove(manageable *_Manageable,unsigned int _Count);
	
	void clear();
	
	manageables_iter manageables_begin() const;
	manageables_iter manageables_end() const;
};

} //namespace lunar

#endif