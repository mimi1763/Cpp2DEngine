/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: scene_manager.h
---------------------------------------------------
*/

#ifndef _SCENE_MANAGER_
#define _SCENE_MANAGER_

#include "border.h"
#include "curve.h"
#include "ellipse.h"
#include "polygon.h"

namespace lunar {

//Forward declarations
class core;

class scene_manager {

	friend core;

private:

	static scene_manager *_Instance; //Grant core exclusive access

public:

	enum COLLISION_MODE { SPHERE, BOX };
	
	typedef std::list<std::pair<attachable*,attachable*>> collision_result;

private:

	struct _Collision_object {
	
	attachable *_Object;
	COLLISION_MODE _Mode;
	unsigned int _Type;
	unsigned int _Mask;
	
	//For optimization purposes
	bool _Derive_sphere;
	bool _Derive_aabb;
	bool _Derive_obb;
	};
	
	typedef std::list<_Collision_object> _Collision_objects;
	typedef std::list<renderable*> _Renderables;
	
	_Collision_objects _Mycollision_objects;
	_Renderables _Myrenderables;
	
	void copy(const scene_manager &_Scene_manager);

public:

	typedef _Renderables::const_iterator renderables_iter; //Keep data safe
	
	//Constructor
	scene_manager();
	
	//Copy constructor
	scene_manager(const scene_manager &_Scene_manager);
	
	//Destructor
	~scene_manager();
	
	scene_manager &operator=(const scene_manager &_Scene_manager);
	
	static scene_manager *instance();
	
	bool collision_query(collision_result &_Mycollision_result) const;
	
	void add_collisionobject(attachable *_Attachable,
		const COLLISION_MODE &_Mode=BOX,unsigned int _Type=0,
			unsigned int _Mask=0);
	
	void remove_collisionobject(attachable *_Attachable);
	
	void clear_collisionobjects();
	
	//Renderable functions
	
	border *create_border(float _Width,float _Height,
		float _Border_size,const color &_Color=color::WHITE,
			bool _Visible=true);
	border *create_border(const border &_Border);
	
	curve *create_curve(float _Line_size,
		const color &_Color=color::WHITE,bool _Visible=true);
	curve *create_curve(const curve &_Curve);
	
	ellipse *create_ellipse(float _Radius,
		const color &_Color=color::WHITE,bool _Visible=true);
	ellipse *create_ellipse(float _Width,float _Height,
		const color &_Color=color::WHITE,bool _Visible=true);
	ellipse *create_ellipse(const ellipse &_Ellipse);
	
	polygon *create_polygon(const color &_Color=color::WHITE,
		bool _Visible=true);
	polygon *create_polygon(const polygon &_Polygon);
	
	rectangle *create_rectangle(float _Width,float _Height,
		const color &_Color=color::WHITE,bool _Visible=true);
	rectangle *create_rectangle(const rectangle &_Rectangle);
	
	void remove_renderable(renderable *&_Renderable);
	
	//Support remove by reference for derived types
	template <typename _Ty>
	void remove_renderable(_Ty &_Derived) {
	
	renderable *_Renderable=_Derived;
	remove_renderable(_Renderable);
	
		//Set referenced pointer to null
		if (!_Renderable) _Derived=0;
	}
	
	void clear_renderables();
	
	renderables_iter renderable_begin() const;
	renderables_iter renderable_end() const;
};

} //namespace lunar

#endif