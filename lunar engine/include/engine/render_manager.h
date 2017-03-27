/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_manager.h
---------------------------------------------------
*/

#ifndef _RENDER_MANAGER_
#define _RENDER_MANAGER_

#include <list>

#include "vector2.h"
#include "color.h"

namespace lunar {

//Forward declarations
class core;
class render_node;
class attachable;

class render_manager {

	friend core;

private:

	static render_manager *_Instance; //Grant core exclusive access

public:

	enum BLENDING { DEFAULT_BLEND, DEFAULT_ALPHA_BLEND, ADDITIVE_BLEND,
		ADDITIVE_ALPHA_BLEND, LIGHT_BLEND };
	
	struct render_stats {
	
	//Public variables
	unsigned int objects;
	unsigned int polygons;
	unsigned int vertices;
	};

protected:

	render_node *_Root_node;
	
	//Currently rendering
	
	render_node *_Active_rendernode;
	attachable *_Active_attachable;
	
	//Protected constructor
	render_manager();
	
	//Protected copy constructor
	render_manager(const render_manager &_Render_manager);
	
	//Protected virtual destructor
	virtual ~render_manager();
	
	render_manager &operator=(const render_manager &_Render_manager);
	
	void copy(const render_manager &_Render_manager);
	
	void render(attachable *_Attachable) const;
	void blend(const BLENDING &_Blending) const;

public:

	static render_manager *instance();
	
	void render();
	
	render_node *root_node() const;
	
	render_node *active_rendernode() const;
	attachable *active_attachable() const;
	
	render_stats statistics() const;
	
	//Utility functions
	
	static void copy_screen(void *_Data,unsigned int _Width,
		unsigned int _Height,const vector2 &_Position=vector2::ZERO);
	
	static void clear_screen();
	static void clear_screen(const color &_Color);
};

} //namespace lunar

#endif