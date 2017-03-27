/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: shadow_component.h
---------------------------------------------------
*/

#ifndef _SHADOW_COMPONENT_
#define _SHADOW_COMPONENT_

//Dependencies (engine)
#include "component.h"

//Forward declarations (entity)
class light;

class shadow_component:public component {

	friend object;

protected:

	//Protected constructor
	shadow_component(object *_Owner,const std::string &_Name,
		bool _Visible=true);
	
	//Protected copy constructor
	shadow_component(const shadow_component &_Shadow_component);
	
	//Protected alternative copy constructor
	shadow_component(const component &_Component);
	
	//Protected alternative copy constructor
	shadow_component(object *_Owner,const std::string &_Name,
		const shadow_component &_Shadow_component);
	
	//Protected alternative copy constructor
	shadow_component(object *_Owner,const std::string &_Name,
		const component &_Component);
	
	//Protected virtual destructor
	virtual ~shadow_component();
	
	void copy(const shadow_component &_Shadow_component);
	
	virtual void attach();

public:

	shadow_component &operator=(const shadow_component &_Shadow_component);
	shadow_component &operator=(const component &_Component);
};

#endif