/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: component.h
---------------------------------------------------
*/

#ifndef _COMPONENT_
#define _COMPONENT_

#include <string>

//Dependencies (engine)
#include "color.h"

//Forward declarations (engine)
namespace lunar {

class attachable;
class resource_data;
} //namespace lunar

//Forward declarations (entity)
class object;

using namespace lunar;

class component {

	friend object;

public:

	enum TYPE { NONE, TEXT, TEXTURE, ANIMATION, PARTICLE_SYSTEM };

protected:

	object *_Owner;
	std::string _Name;
	TYPE _Type;
	
	color _Color;
	bool _Visible;
	
	attachable *_Data;
	
	//Protected constructor
	component(object *_Owner,const std::string &_Name,
		bool _Visible=true);
	
	//Protected copy constructor
	component(const component &_Component);
	
	//Protected alternative copy constructor
	component(object *_Owner,const std::string &_Name,
		const component &_Component);
	
	//Protected virtual destructor
	virtual ~component();
	
	void copy(const component &_Component);
	
	virtual void attach();
	virtual void detach();

public:

	component &operator=(const component &_Component);
	
	object *owner() const;
	const std::string &name() const;
	
	TYPE type() const;
	
	void color_channel(const color &_Color,bool _Base=true);
	const color &color_channel(bool _Base=false) const;
	
	void opacity(float _Opacity,bool _Base=true);
	float opacity(bool _Base=false) const;
	
	void visible(bool _Visible);
	bool visible() const;
	
	//Data
	
	attachable *data(resource_data *_Resource_data);
	attachable *data() const;
	
	void clear_data();
};

#endif