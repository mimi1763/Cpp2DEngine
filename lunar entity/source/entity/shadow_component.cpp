/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: shadow_component.cpp
---------------------------------------------------
*/

#include "shadow_component.h"

//Dependencies (engine)
#include "render_node.h"
#include "attachable.h"

//Dependencies (entity)
#include "object.h"

//Protected

shadow_component::shadow_component(object *_Owner,const std::string &_Name,
	bool _Visible):component(_Owner,_Name,_Visible) {
	//Empty
}

shadow_component::shadow_component(const shadow_component &_Shadow_component):
	component(_Shadow_component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();

copy(_Shadow_component);
}

shadow_component::shadow_component(const component &_Component):
	component(_Component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();
}

shadow_component::shadow_component(object *_Owner,const std::string &_Name,
	const shadow_component &_Shadow_component):component(
		_Owner,_Name,_Shadow_component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();

copy(_Shadow_component);
}

shadow_component::shadow_component(object *_Owner,const std::string &_Name,
	const component &_Component):component(_Owner,_Name,_Component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();
}

shadow_component::~shadow_component() {
	//Empty
}

void shadow_component::copy(const shadow_component &_Shadow_component) {
	//Empty
}

void shadow_component::attach() {

	if (_Data) _Owner->shadow_node()->attach(_Data);
}

//Public

shadow_component &shadow_component::operator=(const shadow_component &_Shadow_component) {

	//Worth doing
	if (this!=&_Shadow_component) {
	
	component::operator=(_Shadow_component); //Invoke base operator=
	copy(_Shadow_component);
	}

return *this;
}

shadow_component &shadow_component::operator=(const component &_Component) {

	//Worth doing
	if (this!=&_Component) component::operator=(_Component); //Invoke base operator=

return *this;
}