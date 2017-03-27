/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: manageable.h
---------------------------------------------------
*/

#ifndef _MANAGEABLE_
#define _MANAGEABLE_

#include "listener_interface.h"
#include "resource_container.h"

namespace lunar {

//Forward declarations
class manager;

class manageable:public listener_interface<manageable_listener*> {

	friend manager;

public:

	enum STATUS { OKAY, NEED_UPDATE, FAILED };

protected:

	manager *_Owner;
	STATUS _Status;
	
	resource_data *_Data;
	resource_data *_Recent_data;
	
	//Protected constructor
	manageable(manager *_Owner,resource_data *_Data);
	
	//Protected copy constructor
	manageable(const manageable &_Manageable);
	
	//Protected alternative copy constructor
	manageable(manager *_Owner,const manageable &_Manageable);
	
	//Protected virtual destructor
	virtual ~manageable();
	
	manageable &operator=(const manageable &_Manageable);
	
	void copy(const manageable &_Manageable);
	
	virtual void changed();
	virtual void status_changed();
	
	void status(const STATUS &_Status);
	void data(resource_data *_Data);

public:

	bool operator==(const manageable &_Manageable) const;
	bool operator!=(const manageable &_Manageable) const;
	
	virtual void update();
	
	void refresh();
	
	manager *owner() const;
	STATUS status() const;
	
	resource_data *data() const;
	resource_data *recent_data() const;
};

} //namespace lunar

#endif