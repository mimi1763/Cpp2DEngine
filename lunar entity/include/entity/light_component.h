/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: light_component.h
---------------------------------------------------
*/

#ifndef _LIGHT_COMPONENT_
#define _LIGHT_COMPONENT_

//Dependencies (engine)
#include "component.h"

//Forward declarations (entity)
class light;

class light_component:public component {

	friend light;

protected:

	//Protected constructor
	light_component(object *_Owner,const std::string &_Name,
		bool _Visible=true);
	
	//Protected copy constructor
	light_component(const light_component &_Light_component);
	
	//Protected alternative copy constructor
	light_component(const component &_Component);
	
	//Protected alternative copy constructor
	light_component(object *_Owner,const std::string &_Name,
		const light_component &_Light_component);
	
	//Protected alternative copy constructor
	light_component(object *_Owner,const std::string &_Name,
		const component &_Component);
	
	//Protected virtual destructor
	virtual ~light_component();
	
	void copy(const light_component &_Light_component);
	
	virtual void attach();

public:

	light_component &operator=(const light_component &_Light_component);
	light_component &operator=(const component &_Component);
};

#endif