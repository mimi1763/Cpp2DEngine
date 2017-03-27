/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: framebuffer_manager.h
---------------------------------------------------
*/

#ifndef _FRAMEBUFFER_MANAGER_
#define _FRAMEBUFFER_MANAGER_

#include "manager.h"
#include "framebuffer.h"

namespace lunar {

//Forward declarations
class core;
class texture_manager;

class framebuffer_manager:public manager {

	friend core;

private:

	static framebuffer_manager *_Instance; //Grant core exclusive access

protected:

	texture_manager *_Texture_manager;
	
	void copy(const framebuffer_manager &_Framebuffer_manager);
	
	virtual void removed(manageable *_Manageable);
	
	//GL framebuffer specific
	bool init_framebuffer(framebuffer *_Framebuffer);

public:

	//Constructor
	framebuffer_manager();
	
	//Alternative constructor
	explicit framebuffer_manager(texture_manager *_Texture_manager);
	
	//Copy constructor
	framebuffer_manager(const framebuffer_manager &_Framebuffer_manager);
	
	//Virtual destructor
	virtual ~framebuffer_manager();
	
	framebuffer_manager &operator=(const framebuffer_manager &_Framebuffer_manager);
	
	static framebuffer_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	//Framebuffer functions
	
	framebuffer *create_framebuffer(unsigned int _Width,
		unsigned int _Height,bool _Visible=true,
			bool _Secured=false);
	
	framebuffer *create_framebuffer(const framebuffer &_Framebuffer,
		bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif