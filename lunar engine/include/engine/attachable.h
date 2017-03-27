/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: attachable.h
---------------------------------------------------
*/

#ifndef _ATTACHABLE_
#define _ATTACHABLE_

#include <list>

#include "listener.h"
#include "render_manager.h" //enum BLENDING

#include "aabb.h"
#include "obb.h"
#include "sphere.h"
#include "any_type.h"

namespace lunar {

//Forward declarations
class render_node;
class shaderprogram;

class attachable:public manageable_listener {

	friend render_manager;

public:

	struct geometry_stats {
	
	//Public variables
	unsigned int polygons;
	unsigned int vertices;
	};

protected:

	typedef std::list<shaderprogram*> _Shaderprograms;
	
	//Common data
	
	render_node *_Parent_node;
	render_manager::BLENDING _Blending;
	unsigned int _Passes;
	bool _Visible;
	
	//Bound data
	
	aabb _Boundingbox;
	aabb _World_boundingbox;
	
	obb _Oriented_boundingbox;
	obb _World_orientedboundingbox;
	
	sphere _Boundingsphere;
	sphere _World_boundingsphere;
	
	vector2 _Boundoffset_min;
	vector2 _Boundoffset_max;
	bool _Show_bounds;
	bool _Update_bounds;
	
	//Other data
	
	_Shaderprograms _Myshaderprograms;
	bool _Shaderprograms_enabled;
	
	any_type _Custom_data;
	
	void copy(const attachable &_Attachable);
	
	virtual void unsubscribed(interface_base *_Interface);
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);
	
	virtual void update_bounds()=0;
	
	virtual void pre_draw()=0;
	virtual void draw()=0;
	virtual void post_draw()=0;
	
	void attach_shaderprogram(shaderprogram *_Shaderprogram);
	void detach_shaderprogram(shaderprogram *_Shaderprogram);
	
	void attach_shaderprograms();
	void detach_shaderprograms();
	
	void create_bounds(const vector2 &_Min,const vector2 &_Max);

public:

	typedef _Shaderprograms::const_iterator shaderprograms_iter; //Keep data safe
	
	//Constructor
	explicit attachable(bool _Visible=true);
	
	//Copy constructor
	attachable(const attachable &_Attachable);
	
	//Virtual destructor
	virtual ~attachable();
	
	attachable &operator=(const attachable &_Attachable);
	
	virtual bool prepare();
	virtual geometry_stats statistics() const;
	
	//Common functions
	
	void parent_node(render_node *_Parent_node);
	render_node *parent_node() const;
	
	void blending(const render_manager::BLENDING &_Blending);
	render_manager::BLENDING blending() const;
	
	void passes(unsigned int _Passes);
	unsigned int passes() const;
	
	void visible(bool _Visible);
	bool visible() const;
	
	//Bound functions
	
	const aabb &boundingbox();
	const aabb &world_boundingbox(bool _Derive=true);
	
	const obb &oriented_boundingbox();
	const obb &world_orientedboundingbox(bool _Derive=true);
	
	const sphere &boundingsphere();
	const sphere &world_boundingsphere(bool _Derive=true);
	
	void bound_offset(const vector2 &_Offset_min,const vector2 &_Offset_max);
	const vector2 &boundoffset_min() const;
	const vector2 &boundoffset_max() const;
	
	void show_bounds(bool _Show_bounds);
	bool show_bounds() const;
	
	//Other functions
	
	void add_shaderprogram(shaderprogram *_Shaderprogram);
	
	void remove_shaderprogram(shaderprogram *_Shaderprogram);
	void clear_shaderprograms();
	
	void shaderprograms_enabled(bool _Enabled);
	bool shaderprograms_enabled() const;
	
	shaderprograms_iter shaderprograms_begin() const;
	shaderprograms_iter shaderprograms_end() const;
	
	void custom_data(const any_type &_Custom_data);
	any_type &custom_data();
};

} //namespace lunar

#endif