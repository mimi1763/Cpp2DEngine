/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: light_component.cpp
---------------------------------------------------
*/

#include "light_component.h"

//Dependencies (engine)
#include "render_node.h"
#include "attachable.h"

//Dependencies (entity)
#include "light.h"

//Protected

light_component::light_component(object *_Owner,const std::string &_Name,
	bool _Visible):component(_Owner,_Name,_Visible) {
	//Empty
}

light_component::light_component(const light_component &_Light_component):
	component(_Light_component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();

copy(_Light_component);
}

light_component::light_component(const component &_Component):
	component(_Component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();
}

light_component::light_component(object *_Owner,const std::string &_Name,
	const light_component &_Light_component):component(
		_Owner,_Name,_Light_component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();

copy(_Light_component);
}

light_component::light_component(object *_Owner,const std::string &_Name,
	const component &_Component):component(_Owner,_Name,_Component) {

//Reattach because of virtual calls in component ctor is disabled
detach();
attach();
}

light_component::~light_component() {
	//Empty
}

void light_component::copy(const light_component &_Light_component) {
	//Empty
}

void light_component::attach() {

	if (_Data) {
	
	light *_Light=dynamic_cast<light*>(_Owner);
	
		if (_Light) {
		
		_Light->light_node()->attach(_Data);
		_Data->blending(render_manager::LIGHT_BLEND);
		}
	}
}

//Public

light_component &light_component::operator=(const light_component &_Light_component) {

	//Worth doing
	if (this!=&_Light_component) {
	
	component::operator=(_Light_component); //Invoke base operator=
	copy(_Light_component);
	}

return *this;
}

light_component &light_component::operator=(const component &_Component) {

	//Worth doing
	if (this!=&_Component) component::operator=(_Component); //Invoke base operator=

return *this;
}