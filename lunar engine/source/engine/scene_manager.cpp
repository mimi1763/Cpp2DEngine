/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: scene_manager.cpp
---------------------------------------------------
*/

#include "scene_manager.h"

//Dependencies
#include "render_node.h"

namespace lunar {

//Private

//This instance is allocated when core is initialized
//This instance is deallocated when core is destroyed
scene_manager *scene_manager::_Instance=0;

void scene_manager::copy(const scene_manager &_Scene_manager) {
	//Empty - override default memcpy behaviour
}

//Public

scene_manager::scene_manager() {
	//Empty
}

scene_manager::scene_manager(const scene_manager &_Scene_manager) {
	copy(_Scene_manager);
}

scene_manager::~scene_manager() {
	clear_renderables();
}

scene_manager &scene_manager::operator=(const scene_manager &_Scene_manager) {

	//Worth doing
	if (this!=&_Scene_manager) copy(_Scene_manager);

return *this;
}

scene_manager *scene_manager::instance() {
	return _Instance;
}

bool scene_manager::collision_query(collision_result &_Mycollision_result) const {

//O(n*2) where n is the # of collision objects

_Mycollision_result.clear(); //Clear previous results

	if (_Mycollision_objects.size()<2) return false; //Nothing to do
	
	for (_Collision_objects::const_iterator _Iter=_Mycollision_objects.begin(),
		_End=_Mycollision_objects.end();_Iter!=_End;++_Iter) {
	
	_Collision_object &_Mycollision_object=(_Collision_object&)(*_Iter); //Increase speed
	
	render_node *_Parent_node=_Mycollision_object._Object->parent_node();
	
		if (!_Parent_node) continue; //Not attached to node
		if (!_Parent_node->visible()) continue; //Node not visible
	
	const sphere &_Boundingsphere=_Mycollision_object._Object->
		world_boundingsphere(_Mycollision_object._Derive_sphere);
	_Mycollision_object._Derive_sphere=false; //Sphere derived for object 1
	
	_Collision_objects::const_iterator _Iter2=_Iter;
	
		//Loop in range [iter+1, end()]
		for (++_Iter2;_Iter2!=_End;++_Iter2) {
		
		_Collision_object &_Mycollision_object2=(_Collision_object&)(*_Iter2); //Increase speed
		
			//Only check for collisions where this object type AND'd
			//with the given object mask results in a non-zero value
			if (_Mycollision_object._Type>0 && _Mycollision_object2._Mask>0 &&
				(_Mycollision_object._Type & _Mycollision_object2._Mask)==0)
					continue;
		
		render_node *_Parentnode2=_Mycollision_object2._Object->
			parent_node();
		
			if (!_Parentnode2) continue; //Not attached to node
			if (!_Parentnode2->visible()) continue; //Node not visible
		
		const sphere &_Boundingsphere2=_Mycollision_object2._Object->
			world_boundingsphere(_Mycollision_object2._Derive_sphere);
		_Mycollision_object2._Derive_sphere=false; //Sphere derived for object 2
		
			//Check if bounding spheres intersects
			if (_Boundingsphere.intersects(_Boundingsphere2)) {
			
			bool _Collision=(_Mycollision_object._Mode==SPHERE &&
				_Mycollision_object2._Mode==SPHERE);
			
				//Continue more precise detection?
				if (!_Collision) {
				
				const aabb &_Boundingbox=_Mycollision_object._Object->
					world_boundingbox(_Mycollision_object._Derive_aabb);
				_Mycollision_object._Derive_aabb=false; //Boundingbox derived for object 1
				
				const aabb &_Boundingbox2=_Mycollision_object2._Object->
					world_boundingbox(_Mycollision_object2._Derive_aabb);
				_Mycollision_object2._Derive_aabb=false; //Boundingbox derived for object 2
				
					//Check if bounding boxes intersects
					if (_Boundingbox.intersects(_Boundingbox2)) {
					
					_Collision=(_Parent_node->axis_aligned() && _Parentnode2->
						axis_aligned());
					
						//If not axis aligned, check if oriented bounding boxes intersects
						if (!_Collision) {
						
						const obb &_Oriented_boundingbox=_Mycollision_object.
							_Object->world_orientedboundingbox(_Mycollision_object.
								_Derive_obb);
						_Mycollision_object._Derive_obb=false; //Oriented boundingbox derived for object 1
						
						const obb &_Oriented_boundingbox2=_Mycollision_object2.
							_Object->world_orientedboundingbox(_Mycollision_object2.
								_Derive_obb);
						_Mycollision_object2._Derive_obb=false; //Oriented boundingbox derived for object 2
						
						//Check if oriented bounding boxes intersects
						_Collision=_Oriented_boundingbox.intersects(
							_Oriented_boundingbox2);
						}
					}
				}
				
				if (_Collision) _Mycollision_result.push_back(std::pair<
					attachable*,attachable*>(_Mycollision_object._Object,
						_Mycollision_object2._Object)); //Collision
			}
		}
	
	//Done with object for now, reset
	_Mycollision_object._Derive_sphere=true;
	_Mycollision_object._Derive_aabb=true;
	_Mycollision_object._Derive_obb=true;
	}

return !_Mycollision_result.empty();
}

void scene_manager::add_collisionobject(attachable *_Attachable,
	const COLLISION_MODE &_Mode,unsigned int _Type,
		unsigned int _Mask) {

	for (_Collision_objects::iterator _Iter=_Mycollision_objects.begin(),
		_End=_Mycollision_objects.end();_Iter!=_End;++_Iter) {
	
		//No duplicates allowed
		if ((*_Iter)._Object==_Attachable) return;
	}

_Collision_object _Mycollision_object;
_Mycollision_object._Object=_Attachable;
_Mycollision_object._Mode=_Mode;
_Mycollision_object._Type=_Type;
_Mycollision_object._Mask=_Mask;

//For optimization purposes
_Mycollision_object._Derive_sphere=true;
_Mycollision_object._Derive_aabb=true;
_Mycollision_object._Derive_obb=true;

_Mycollision_objects.push_back(_Mycollision_object);
}

void scene_manager::remove_collisionobject(attachable *_Attachable) {

	for (_Collision_objects::iterator _Iter=_Mycollision_objects.begin(),
		_End=_Mycollision_objects.end();_Iter!=_End;++_Iter) {
	
	_Collision_object &_Mycollision_object=(*_Iter); //Increase speed
	
		if (_Mycollision_object._Object==_Attachable) {
		
		_Mycollision_objects.erase(_Iter);
		return;
		}
	}
}

void scene_manager::clear_collisionobjects() {
	_Mycollision_objects.clear();
}

//Renderable functions

border *scene_manager::create_border(float _Width,
	float _Height,float _Border_size,const color &_Color,
		bool _Visible) {

border *_Myborder=new border(_Width,_Height,_Border_size,_Color,_Visible);
_Myrenderables.push_back(_Myborder);
return _Myborder;
}

border *scene_manager::create_border(const border &_Border) {

border *_Myborder=new border(_Border);
_Myrenderables.push_back(_Myborder);
return _Myborder;
}

curve *scene_manager::create_curve(float _Line_size,
	const color &_Color,bool _Visible) {

curve *_Mycurve=new curve(_Line_size,_Color,_Visible);
_Myrenderables.push_back(_Mycurve);
return _Mycurve;
}

curve *scene_manager::create_curve(const curve &_Curve) {

curve *_Mycurve=new curve(_Curve);
_Myrenderables.push_back(_Mycurve);
return _Mycurve;
}

ellipse *scene_manager::create_ellipse(float _Radius,
	const color &_Color,bool _Visible) {

ellipse *_Myellipse=new ellipse(_Radius,_Color,_Visible);
_Myrenderables.push_back(_Myellipse);
return _Myellipse;
}

ellipse *scene_manager::create_ellipse(float _Width,
	float _Height,const color &_Color,bool _Visible) {

ellipse *_Myellipse=new ellipse(_Width,_Height,_Color,_Visible);
_Myrenderables.push_back(_Myellipse);
return _Myellipse;
}

ellipse *scene_manager::create_ellipse(const ellipse &_Ellipse) {

ellipse *_Myellipse=new ellipse(_Ellipse);
_Myrenderables.push_back(_Myellipse);
return _Myellipse;
}

polygon *scene_manager::create_polygon(const color &_Color,
	bool _Visible) {

polygon *_Mypolygon=new polygon(_Color,_Visible);
_Myrenderables.push_back(_Mypolygon);
return _Mypolygon;
}

polygon *scene_manager::create_polygon(const polygon &_Polygon) {

polygon *_Mypolygon=new polygon(_Polygon);
_Myrenderables.push_back(_Mypolygon);
return _Mypolygon;
}

rectangle *scene_manager::create_rectangle(float _Width,
	float _Height,const color &_Color,bool _Visible) {

rectangle *_Myrectangle=new rectangle(_Width,_Height,_Color,_Visible);
_Myrenderables.push_back(_Myrectangle);
return _Myrectangle;
}

rectangle *scene_manager::create_rectangle(const rectangle &_Rectangle) {

rectangle *_Myrectangle=new rectangle(_Rectangle);
_Myrenderables.push_back(_Myrectangle);
return _Myrectangle;
}

void scene_manager::remove_renderable(renderable *&_Renderable) {

	for (_Renderables::iterator _Iter=_Myrenderables.begin(),
		_End=_Myrenderables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Renderable) {
		
		delete _Renderable;
		_Renderable=0; //Set referenced pointer to null
		
		_Myrenderables.erase(_Iter);
		return;
		}
	}
}

void scene_manager::clear_renderables() {

	for (_Renderables::iterator _Iter=_Myrenderables.begin(),
		_End=_Myrenderables.end();_Iter!=_End;++_Iter) delete (*_Iter);

_Myrenderables.clear();
}

scene_manager::renderables_iter scene_manager::renderable_begin() const {
	return _Myrenderables.begin();
}

scene_manager::renderables_iter scene_manager::renderable_end() const {
	return _Myrenderables.end();
}

} //namespace lunar