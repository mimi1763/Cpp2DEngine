/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: sampler.h
---------------------------------------------------
*/

#ifndef _SAMPLER_
#define _SAMPLER_

#include "uniform.h"
#include "listener.h"

namespace lunar {

//Forward declarations
class texture;
class animation;

class sampler:public uniform,public manageable_listener {

	friend shaderprogram;
	friend shaderprogram_manager;

protected:

	int _Unit;
	texture *_Texture;
	animation *_Animation;
	
	//Protected constructor
	explicit sampler(const std::string &_Name,unsigned int _Size=1);
	
	//Protected constructor (texture)
	sampler(const std::string &_Name,texture *_Texture,
		unsigned int _Size=1);
	
	//Protected constructor (animation)
	sampler(const std::string &_Name,animation *_Animation,
		unsigned int _Size=1);
	
	//Protected copy constructor
	sampler(const sampler &_Sampler);
	
	//Protected virtual destructor
	virtual ~sampler();
	
	void copy(const sampler &_Sampler);
	
	virtual void unsubscribed(interface_base *_Interface);
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);
	
	void attach_source();
	void detach_source();

public:

	sampler &operator=(const sampler &_Sampler);
	
	void texture_data(texture *_Texture);
	texture *texture_data() const;
	
	void animation_data(animation *_Animation);
	animation *animation_data() const;
};

} //namespace lunar

#endif