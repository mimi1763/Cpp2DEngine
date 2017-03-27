/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: attachable.cpp
---------------------------------------------------
*/

#include "attachable.h"

//Dependencies
#include <cmath>
#include "system.h"

#include "render_node.h"
#include "shaderprogram.h"
#include "support.h"

namespace lunar {

//Protected

void attachable::copy(const attachable &_Attachable) {

_Blending=_Attachable._Blending;
_Passes=_Attachable._Passes;
_Visible=_Attachable._Visible;

_Boundoffset_min=_Attachable._Boundoffset_min;
_Boundoffset_max=_Attachable._Boundoffset_max;
_Show_bounds=_Attachable._Show_bounds;

_Myshaderprograms=_Attachable._Myshaderprograms;
_Shaderprograms_enabled=_Attachable._Shaderprograms_enabled;
_Custom_data=_Attachable._Custom_data;
}

void attachable::unsubscribed(interface_base *_Interface) {

//Shaderprogram has been unsubscribed, notify attachable

	for (_Shaderprograms::iterator _Iter=_Myshaderprograms.begin(),
		_End=_Myshaderprograms.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Interface) {
		
		_Myshaderprograms.erase(_Iter); //Remove shaderprogram
		break;
		}
	}
	
	if (_Myshaderprograms.empty()) _Shaderprograms_enabled=false; //Auto disable
}

void attachable::manageable_changed(manageable *_Manageable) {
	//Empty - override if needed
}

void attachable::manageable_statuschanged(manageable *_Manageable) {
	//Empty - override if needed
}

void attachable::manageable_removed(manageable *_Manageable) {

//Shaderprogram has been destroyed, notify attachable

attachable::unsubscribed(_Manageable); //Reuse functionality
}

void attachable::attach_shaderprogram(shaderprogram *_Shaderprogram) {

//Can't add secure listeners because attachable doesn't own shaderprogram
//Secure listeners will not be broken before shaderprogram is out of scope
//If public link is boken the attachable will remove shaderprogram
_Shaderprogram->subscribe(this); //Add listener
}

void attachable::detach_shaderprogram(shaderprogram *_Shaderprogram) {

_Listening=false; //Ignore notifications
_Shaderprogram->unsubscribe(this); //Remove listener
_Listening=true;
}

void attachable::attach_shaderprograms() {

	if (_Myshaderprograms.empty()) return;
	
	for (_Shaderprograms::iterator _Iter=_Myshaderprograms.begin(),
		_End=_Myshaderprograms.end();_Iter!=_End;++_Iter)
			attach_shaderprogram(*_Iter);
}

void attachable::detach_shaderprograms() {

	if (_Myshaderprograms.empty()) return;
	
	for (_Shaderprograms::iterator _Iter=_Myshaderprograms.begin(),
		_End=_Myshaderprograms.end();_Iter!=_End;++_Iter)
			detach_shaderprogram(*_Iter);
}

void attachable::create_bounds(const vector2 &_Min,const vector2 &_Max) {

//Update boundingbox
_Boundingbox.extents(_Min,_Max);
vector2 _Half_size=_Boundingbox.half_size();

//Apply bound offset
_Boundingbox.extents(_Min+(_Half_size*_Boundoffset_min),
	_Max-(_Half_size*_Boundoffset_max));
_Half_size=_Boundingbox.half_size();

//Update bounding sphere from original extents
_Boundingsphere.radius(_Half_size.length());
_Boundingsphere.center(_Boundingbox.center());

//Update oriented boundingbox from offset extents
_Oriented_boundingbox=_Boundingbox;
}

//Public

attachable::attachable(bool _Visible):_Parent_node(0),
	_Blending(render_manager::DEFAULT_ALPHA_BLEND),_Passes(1),
		_Visible(_Visible),_Show_bounds(false),_Update_bounds(false),
			_Shaderprograms_enabled(false) {
	//Empty
}

attachable::attachable(const attachable &_Attachable) {

//Not copyable
_Parent_node=0;
_Update_bounds=false;

copy(_Attachable);
attach_shaderprograms();
}

attachable::~attachable() {

	//Check if this is attached to a render node
	if (_Parent_node) _Parent_node->detach(this);

clear_shaderprograms();
}

attachable &attachable::operator=(const attachable &_Attachable) {

	//Worth doing
	if (this!=&_Attachable) {
	
	detach_shaderprograms();
	copy(_Attachable);
	attach_shaderprograms();
	}

return *this;
}

bool attachable::prepare() {

//This function is automatically called before rendering
//Return false from this function to cancel rendering
return true;
}

attachable::geometry_stats attachable::statistics() const {

geometry_stats _Geometry_stats={0};
return _Geometry_stats;
}

//Common functions

void attachable::parent_node(render_node *_Parent_node) {
	this->_Parent_node=_Parent_node;
}

render_node *attachable::parent_node() const {
	return _Parent_node;
}

void attachable::blending(const render_manager::BLENDING &_Blending) {
	this->_Blending=_Blending;
}

render_manager::BLENDING attachable::blending() const {
	return _Blending;
}

void attachable::passes(unsigned int _Passes) {

	if (_Passes==0) _Visible=false;
	else this->_Passes=_Passes; //Non zero value only
}

unsigned int attachable::passes() const {
	return _Passes;
}

void attachable::visible(bool _Visible) {
	this->_Visible=_Visible;
}

bool attachable::visible() const {
	return _Visible;
}

//Bound functions

const aabb &attachable::boundingbox() {

	//Need update
	if (_Update_bounds) {
	
	update_bounds();
	_Update_bounds=false;
	}

return _Boundingbox;
}

const aabb &attachable::world_boundingbox(bool _Derive) {

	//Not attached to node, return local bounds
	if (!_Parent_node) return boundingbox();
	
	//Update world bounds
	if (_Derive) {
	
	_World_boundingbox=boundingbox();
	_World_boundingbox.transform_affine(_Parent_node->
		world_transformation());
	}

return _World_boundingbox;
}

const obb &attachable::oriented_boundingbox() {

	//Need update
	if (_Update_bounds) {
	
	update_bounds();
	_Update_bounds=false;
	}

return _Oriented_boundingbox;
}

const obb &attachable::world_orientedboundingbox(bool _Derive) {

	//Not attached to node, return local bounds
	if (!_Parent_node) return oriented_boundingbox();
	
	//Update world bounds
	if (_Derive) {
	
	_World_orientedboundingbox=oriented_boundingbox();
	_World_orientedboundingbox.transform_affine(_Parent_node->
		world_transformation());
	}

return _World_orientedboundingbox;
}

const sphere &attachable::boundingsphere() {

	//Need update
	if (_Update_bounds) {
	
	update_bounds();
	_Update_bounds=false;
	}

return _Boundingsphere;
}

const sphere &attachable::world_boundingsphere(bool _Derive) {

	//Not attached to node, return local bounds
	if (!_Parent_node) return boundingsphere();
	
	//Update world bounds
	//Warning: This is slow if sphere is not centered
	if (_Derive) {
	
	const sphere &_Boundingsphere=boundingsphere();
	const vector2 &_Center=_Boundingsphere.center();
	_World_boundingsphere.radius(_Boundingsphere.radius()*_Parent_node->
		world_scaling().maximum());
	
		if (_Center==vector2::ZERO) _World_boundingsphere.center(
			_Parent_node->world_position()); //No offset
		else _World_boundingsphere.center(_Parent_node->
			world_transformation().transform_affine(_Center)); //Use local center as offset
	}

return _World_boundingsphere;
}

void attachable::bound_offset(const vector2 &_Offset_min,const vector2 &_Offset_max) {

_Boundoffset_min=vector2(fabs(_Offset_min.x),fabs(_Offset_min.y)); //Left-bottom
_Boundoffset_max=vector2(fabs(_Offset_max.x),fabs(_Offset_max.y)); //Right-top

	//Adjust max offset to 95%, because we don't want empty bounds
	//Left-bottom
	if (_Boundoffset_min.x>0.95f) _Boundoffset_min.x=0.95f;
	if (_Boundoffset_min.y>0.95f) _Boundoffset_min.y=0.95f;
	
	//Right-top
	if (_Boundoffset_max.x>0.95f) _Boundoffset_max.x=0.95f;
	if (_Boundoffset_max.y>0.95f) _Boundoffset_max.y=0.95f;
}

const vector2 &attachable::boundoffset_min() const {
	return _Boundoffset_min;
}

const vector2 &attachable::boundoffset_max() const {
	return _Boundoffset_max;
}

void attachable::show_bounds(bool _Show_bounds) {
	this->_Show_bounds=_Show_bounds;
}

bool attachable::show_bounds() const {
	return _Show_bounds;
}

//Other functions

void attachable::add_shaderprogram(shaderprogram *_Shaderprogram) {

	for (_Shaderprograms::const_iterator _Iter=_Myshaderprograms.begin(),
		_End=_Myshaderprograms.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Shaderprogram) return; //No duplicates allowed
	}
	
	if (_Myshaderprograms.empty()) _Shaderprograms_enabled=true; //Auto enable

_Myshaderprograms.push_back(_Shaderprogram);
attach_shaderprogram(_Shaderprogram);
}

void attachable::remove_shaderprogram(shaderprogram *_Shaderprogram) {

	for (_Shaderprograms::iterator _Iter=_Myshaderprograms.begin(),
		_End=_Myshaderprograms.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Shaderprogram) {
		
		_Myshaderprograms.erase(_Iter); //Only erase node
		detach_shaderprogram(_Shaderprogram);
		break;
		}
	}
	
	if (_Myshaderprograms.empty()) _Shaderprograms_enabled=false; //Auto disable
}

void attachable::clear_shaderprograms() {

detach_shaderprograms();
_Myshaderprograms.clear(); //Only erase nodes
_Shaderprograms_enabled=false; //Auto disable
}

void attachable::shaderprograms_enabled(bool _Enabled) {
	_Shaderprograms_enabled=_Enabled;
}

bool attachable::shaderprograms_enabled() const {
	return _Shaderprograms_enabled;
}

attachable::shaderprograms_iter attachable::shaderprograms_begin() const {
	return _Myshaderprograms.begin();
}

attachable::shaderprograms_iter attachable::shaderprograms_end() const {
	return _Myshaderprograms.end();
}

void attachable::custom_data(const any_type &_Custom_data) {
	this->_Custom_data=_Custom_data;
}

any_type &attachable::custom_data() {
	return _Custom_data;
}

} //namespace lunar