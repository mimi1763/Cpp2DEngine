/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: static_entity.cpp
---------------------------------------------------
*/

#include "static_entity.h"

//Dependencies (engine)
#include "render_node.h"

//Dependencies (entity)
#include "entity_manager.h"
#include "node_manager.h"

//Protected

static_entity::static_entity(entity_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,bool _Visible):_Owner(_Owner),
		_Draw_order(_Draw_order),_Elevation(0.0f),_Sound_enabled(true),
			_Sound_volume(1.0f),_Light_enabled(true),_Light_brightness(1.0f) {

_Node=_Owner->node_manager_ptr()->create_node(_Parent_node,
	_Draw_order+interval(_Owner->node()->draw_order()),_Visible);
_Root_node=_Owner->node_manager_ptr()->create_node(_Node,
	_Node->draw_order(),_Node->visible());
}

static_entity::static_entity(const static_entity &_Static_entity):
	audible(_Static_entity),_Owner(_Static_entity._Owner),
		_Draw_order(_Static_entity._Draw_order) {

_Node=_Owner->node_manager_ptr()->create_node(_Static_entity._Node->
	parent_node(),*_Static_entity._Node,_Static_entity._Node->draw_order(),
		_Static_entity._Node->visible());
_Root_node=_Owner->node_manager_ptr()->create_node(_Node,
	_Node->draw_order(),_Node->visible());
copy(_Static_entity);
}

static_entity::static_entity(entity_manager *_Owner,render_node *_Parent_node,
	const interval &_Draw_order,const static_entity &_Static_entity):
		audible(_Static_entity),_Owner(_Owner),_Draw_order(_Draw_order) {

_Node=_Owner->node_manager_ptr()->create_node(_Parent_node,
	*_Static_entity._Node,_Draw_order+interval(_Owner->node()->draw_order()),
		_Static_entity._Node->visible());
_Root_node=_Owner->node_manager_ptr()->create_node(_Node,
	_Node->draw_order(),_Node->visible());
copy(_Static_entity);
}

static_entity::~static_entity() {

clear_objects();
_Owner->node_manager_ptr()->recycle_node(_Node);
}

void static_entity::copy(const static_entity &_Static_entity) {

_Elevation=_Static_entity._Elevation;
_Color=_Static_entity._Color;

_Sound_enabled=_Static_entity._Sound_enabled;
_Sound_volume=_Static_entity._Sound_volume;
_Light_enabled=_Static_entity._Light_enabled;
_Light_brightness=_Static_entity._Light_brightness;

	//Copy object structure
	if (!_Static_entity._Mybranches.empty()) {
	
	_Branches _Global_branches;
	
		for (_Branches::const_iterator _Iter=_Static_entity._Mybranches.begin(),
			_End=_Static_entity._Mybranches.end();_Iter!=_End;++_Iter) {
		
			//No parent
			if (!(*_Iter)->_Parent_object) _Global_branches.push_back(*_Iter);
		}
	
	_Branches _Mybranches;
	_Branch *_Mybranch;
	
		//Need to add objects in correct order (least depth object first)
		for (_Branches::const_iterator _Iter=_Global_branches.begin(),
			_End=_Global_branches.end();_Iter!=_End;++_Iter) {
		
		_Static_entity.traverse(*_Iter,_Mybranches);
		
			for (_Branches::const_iterator _Biter=_Mybranches.begin(),
				_Bend=_Mybranches.end();_Biter!=_Bend;++_Biter) {
			
			_Mybranch=(*_Biter); //Increase speed
			
			light *_Light=dynamic_cast<light*>(_Mybranch->_Object);
			
				if (_Light) create_light(_Mybranch->_Object->name(),
					_Mybranch->_Parent_object?object_pointer(
						_Mybranch->_Parent_object->name()):0,
							_Mybranch->_Object->draw_order(),*_Light);
				else create_object(_Mybranch->_Object->name(),
					_Mybranch->_Parent_object?object_pointer(
						_Mybranch->_Parent_object->name()):0,
							_Mybranch->_Object->draw_order(),
								*_Mybranch->_Object);
			}
		
		_Mybranches.clear();
		}
	}
}

void static_entity::object_created(object *_Object) {
	//Empty
}

void static_entity::object_removed(object *_Object) {
	//Empty
}

//Audible

sound_manager *static_entity::sound_owner() const {
	return _Owner->external().sound_manager_ptr();
}

render_node *static_entity::sound_node() const {
	return _Node;
}

//Branches

void static_entity::create_branch(object *_Object,object *_Parent_object) {

_Branch *_Mybranch=branch_pointer(_Object->name());

	//Object branch already exist
	if (_Mybranch) {
	
	_Branch *_Branch_iter;
	
		for (_Branches::const_iterator _Iter=_Mybranches.begin(),
			_End=_Mybranches.end();_Iter!=_End;++_Iter) {
		
		_Branch_iter=(*_Iter); //Increase speed
		
			if (_Branch_iter->_Parent_object==_Mybranch->_Object) {
			
			_Object->node()->adopt(_Branch_iter->_Object->node()); //Change node parent
			_Branch_iter->_Parent_object=_Object; //Change object parent
			}
		}
	
	object_removed(_Mybranch->_Object);
	delete _Mybranch->_Object;
	}
	else {
	
	_Mybranch=new _Branch;
	_Mybranches.push_back(_Mybranch);
	}

_Mybranch->_Object=_Object;
_Mybranch->_Parent_object=_Parent_object;
}

static_entity::_Branch *static_entity::branch_pointer(const std::string &_Name) const {

	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
		//Object exist, return branch pointer
		if ((*_Iter)->_Object->name()==_Name) return (*_Iter);
	}

return 0;
}

void static_entity::traverse(_Branch *_Mybranch,_Branches &_Mybranches) const {

//Depth first search traverse
_Mybranches.push_back(_Mybranch);

	//Traverse child objects recursively
	for (_Branches::const_iterator _Iter=this->_Mybranches.begin(),
		_End=this->_Mybranches.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)->_Parent_object==_Mybranch->_Object)
			traverse(*_Iter,_Mybranches);
	}
}

void static_entity::traverse(object *_Object,objects &_Objects) const {

//Depth first search traverse

	if (_Object) _Objects.push_back(_Object);

	//Traverse child objects recursively
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)->_Parent_object==_Object)
			traverse((*_Iter)->_Object,_Objects);
	}
}

//Public

static_entity &static_entity::operator=(const static_entity &_Static_entity) {

	//Worth doing
	if (this!=&_Static_entity) {
	
	clear_objects();
	
	audible::operator=(_Static_entity); //Invoke base operator=
	copy(_Static_entity);
	
	node_manager::copy_node(*_Node,*_Static_entity._Node);
	node_manager::copy_node(*_Root_node,*_Static_entity._Root_node);
	}

return *this;
}

void static_entity::update(float _Time) {

	//Only update if entity is visible
	if (_Node->visible()) {
	
		//Update all objects
		for (_Branches::const_iterator _Iter=_Mybranches.begin(),
			_End=_Mybranches.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Object->update(_Time);
	}

audible::update(_Time);
}

entity_manager *static_entity::owner() const {
	return _Owner;
}

const interval &static_entity::draw_order() const {
	return _Draw_order;
}

render_node *static_entity::node() const {
	return _Node;
}

void static_entity::append(const static_entity &_Static_entity) {

	//Append sound groups
	if (!_Static_entity._Mysound_groups.empty()) {
	
	sound_group *_Sound_group;
	
		for (_Sound_groups::const_iterator _Iter=_Static_entity._Mysound_groups.begin(),
			_End=_Static_entity._Mysound_groups.end();_Iter!=_End;++_Iter) {
		
		_Sound_group=(*_Iter); //Increase speed
		create_soundgroup(_Sound_group->name(),*_Sound_group);
		}
	}

	//Append object structure 
	if (!_Static_entity._Mybranches.empty()) {
	
	_Branches _Global_branches;
	_Branches _Mybranches;

		for (_Branches::const_iterator _Iter=_Static_entity._Mybranches.begin(),
			_End=_Static_entity._Mybranches.end();_Iter!=_End;++_Iter) {
		
			//No parent
			if (!(*_Iter)->_Parent_object &&
				!object_pointer((*_Iter)->_Object->name())) {

			_Static_entity.traverse(*_Iter,_Mybranches);
			_Mybranches.pop_front();

			bool branch_appendable=true;

				for (_Branches::iterator _Biter=_Mybranches.begin(),
					_Bend=_Mybranches.end();_Biter!=_Bend;++_Biter) {

					if (object_pointer((*_Biter)->_Object->name())) {

					branch_appendable=false;
					break;
					}
				}

				if (branch_appendable)
					_Global_branches.push_back(*_Iter);

			_Mybranches.clear();
			}
		}

	_Branch *_Mybranch;
	
		//Need to add objects in correct order (least depth object first)
		for (_Branches::const_iterator _Iter=_Global_branches.begin(),
			_End=_Global_branches.end();_Iter!=_End;++_Iter) {
		
		_Static_entity.traverse(*_Iter,_Mybranches);

			for (_Branches::iterator _Biter=_Mybranches.begin(),
				_Bend=_Mybranches.end();_Biter!=_Bend;++_Biter) {
			
			_Mybranch=(*_Biter); //Increase speed
			
			light *_Light=dynamic_cast<light*>(_Mybranch->_Object);
			
				if (_Light) create_light(_Mybranch->_Object->name(),
					_Mybranch->_Parent_object?object_pointer(
						_Mybranch->_Parent_object->name()):0,
							_Mybranch->_Object->draw_order(),*_Light);
				else create_object(_Mybranch->_Object->name(),
					_Mybranch->_Parent_object?object_pointer(
						_Mybranch->_Parent_object->name()):0,
							_Mybranch->_Object->draw_order(),
								*_Mybranch->_Object);
			}
		
		_Mybranches.clear();
		}
	}
}

void static_entity::elevation(float _Elevation) {

	//Change relative elevation for all objects
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter)
			(*_Iter)->_Object->elevation(_Elevation,false);

this->_Elevation=_Elevation;
}

void static_entity::elevation(object *_Parent_object,float _Elevation,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	if (_Forced) {
	
		//Force new parent base elevation
		if (_Parent_object) {

		_Parent_object->elevation(_Elevation);
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				_Root_object->elevation(_Elevation);
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change relative elevation for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter)
			(*_Iter)->elevation(_Elevation,false);
}

float static_entity::elevation() const {
	return _Elevation;
}

void static_entity::color_channel(const color &_Color) {

	//Change color for all objects
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter)
			(*_Iter)->_Object->color_channel(_Color,false);

this->_Color=_Color;
}

void static_entity::color_channel(object *_Parent_object,const color &_Color,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base color
	if (_Forced) {
	
		if (_Parent_object) {

		_Parent_object->color_channel(_Color);
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				_Root_object->color_channel(_Color);
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change color for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter)
			(*_Iter)->color_channel(_Color,false);
}

const color &static_entity::color_channel() const {
	return _Color;
}

void static_entity::opacity(float _Opacity) {

	//Change opacity for all objects
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter)
			(*_Iter)->_Object->opacity(_Opacity,false);

_Color.a=_Opacity;
}

void static_entity::opacity(object *_Parent_object,float _Opacity,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base opacity
	if (_Forced) {
	
		if (_Parent_object) {
		
		_Parent_object->opacity(_Opacity);
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				_Root_object->opacity(_Opacity);
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change opacity for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter)
			(*_Iter)->opacity(_Opacity,false);
}

float static_entity::opacity() const {
	return _Color.a;
}

void static_entity::visible(bool _Visible) {

_Node->visible(_Visible,!_Visible); //Cascade if visible false

	if (_Visible) {
	
		//Change visibility for all objects
		for (_Branches::const_iterator _Iter=_Mybranches.begin(),
			_End=_Mybranches.end();_Iter!=_End;++_Iter)
				(*_Iter)->_Object->visible(_Visible,false);
	}
}

void static_entity::visible(object *_Parent_object,bool _Visible,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base visibility
	if (_Forced) {
	
		if (_Parent_object) {

		_Parent_object->visible(_Visible);
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				_Root_object->visible(_Visible);
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change visibility for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter)
			(*_Iter)->visible(_Visible,false);
}

bool static_entity::visible() const {
	return _Node->visible();
}

//Sound specific

void static_entity::sound_enabled(bool _Enabled) {

	//Change audibility for all entity sounds
	if (_Enabled) play(false);
	else stop(false,false);
	
	//Change audibility for all object sounds
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
		if (_Enabled) (*_Iter)->_Object->play(false);
		else (*_Iter)->_Object->stop(false,false);
	}

_Sound_enabled=_Enabled;
}

void static_entity::sound_enabled(object *_Parent_object,bool _Enabled,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base audibility
	if (_Forced) {
	
		if (_Parent_object) {

			if (_Enabled) _Parent_object->play();
			else _Parent_object->stop();
		
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
					if (_Enabled) _Root_object->play();
					else _Root_object->stop();
				
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change audibility for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter) {
	
		if (_Enabled) (*_Iter)->play(false);
		else (*_Iter)->stop(false,false);
	}
}

bool static_entity::sound_enabled() const {
	return _Sound_enabled;
}

void static_entity::sound_volume(float _Volume) {

//Change sound volume for all entity sounds
volume(_Volume,false);

	//Change sound volume for all object sounds
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter)
			(*_Iter)->_Object->volume(_Volume,false);

_Sound_volume=_Volume;
}

void static_entity::sound_volume(object *_Parent_object,float _Volume,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base sound volume
	if (_Forced) {
	
		if (_Parent_object) {

		_Parent_object->volume(_Volume);
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				_Root_object->volume(_Volume);
				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change sound volume for this object and all child objects
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter)
			(*_Iter)->volume(_Volume,false);
}

float static_entity::sound_volume() const {
	return _Sound_volume;
}

//Light specific

void static_entity::light_enabled(bool _Enabled) {

	//Change light enabled for all lights
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
	light *_Light=dynamic_cast<light*>((*_Iter)->_Object);
	
		if (_Light) _Light->enabled(_Enabled);
	}

_Light_enabled=_Enabled;
}

void static_entity::light_enabled(object *_Parent_object,bool _Enabled) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Change light enabled for this light and all child lights
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter) {
	
	light *_Light=dynamic_cast<light*>(*_Iter);
	
		if (_Light) _Light->enabled(_Enabled);
	}
}

bool static_entity::light_enabled() const {
	return _Light_enabled;
}

void static_entity::light_brightness(float _Brightness) {

	//Change light brightness for all lights
	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
	light *_Light=dynamic_cast<light*>((*_Iter)->_Object);
	
		if (_Light) _Light->brightness(_Brightness,false);
	}

_Light_brightness=_Brightness;
}

void static_entity::light_brightness(object *_Parent_object,float _Brightness,
	bool _Forced) {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

objects _Objects;
traverse(_Parent_object,_Objects);

	//Force new parent base light brightness
	if (_Forced) {
	
		if (_Parent_object) {

		light *_Light=dynamic_cast<light*>(_Parent_object);
		
			if (_Light) _Light->brightness(_Brightness);
		
		_Objects.pop_front();
		}
		else {

			//Force new base elevation for all root objects
			for (objects::const_iterator _Iter=_Objects.begin();
				_Iter!=_Objects.end();) {

			object *_Root_object=(*_Iter);

				if (_Root_object->node()->parent_node()==_Root_node) {
				
				light *_Light=dynamic_cast<light*>(_Root_object);

					if (_Light) _Light->brightness(_Brightness);

				_Iter=_Objects.erase(_Iter);
				}
				else ++_Iter;
			}
		}
	}
	
	//Change light brightness for this light and all child lights
	for (objects::const_iterator _Iter=_Objects.begin(),
		_End=_Objects.end();_Iter!=_End;++_Iter) {
	
	light *_Light=dynamic_cast<light*>(*_Iter);
	
		if (_Light) _Light->brightness(_Brightness,false);
	}
}

float static_entity::light_brightness() const {
	return _Light_brightness;
}

//Objects

object *static_entity::create_object(const std::string &_Name,int _Draw_order,
	bool _Visible) {
	return create_object(_Name,0,_Draw_order,_Visible);
}

object *static_entity::create_object(const std::string &_Name,object *_Parent_object,
	int _Draw_order,bool _Visible) {

	//Parent object doesn't exist
	if (_Parent_object && _Parent_object->owner()!=this)
		return 0;
	
	//Object already exist
	if (object_pointer(_Name)) return 0;

object *_Myobject=new object(this,_Name,_Parent_object?
	_Parent_object->node():_Root_node,_Draw_order,_Visible);

//Set default properties
_Myobject->elevation(_Elevation);
_Myobject->color_channel(_Color);

	if (_Sound_enabled) _Myobject->play();
	else _Myobject->stop();

_Myobject->volume(_Sound_volume);

create_branch(_Myobject,_Parent_object);
object_created(_Myobject);
return _Myobject;
}

object *static_entity::create_object(const std::string &_Name,int _Draw_order,
	const object &_Object) {
	return create_object(_Name,0,_Draw_order,_Object);
}

object *static_entity::create_object(const std::string &_Name,object *_Parent_object,
	int _Draw_order,const object &_Object) {

	//Parent object doesn't exist
	if (_Parent_object && _Parent_object->owner()!=this)
		return 0;
	
	//Object already exist
	if (object_pointer(_Name)) return 0;

object *_Myobject=new object(this,_Name,_Parent_object?
	_Parent_object->node():_Root_node,_Draw_order,_Object);

create_branch(_Myobject,_Parent_object);
object_created(_Myobject);
return _Myobject;
}

light *static_entity::create_light(const std::string &_Name,int _Draw_order,
	bool _Visible) {
	return create_light(_Name,0,_Draw_order,_Visible);
}

light *static_entity::create_light(const std::string &_Name,object *_Parent_object,
	int _Draw_order,bool _Visible) {

	//Parent object doesn't exist
	if (_Parent_object && _Parent_object->owner()!=this)
		return 0;
	
	//Object already exist
	if (object_pointer(_Name)) return 0;

light *_Mylight=new light(this,_Name,_Parent_object?
	_Parent_object->node():_Root_node,_Draw_order,_Visible);

//Set default properties
_Mylight->elevation(_Elevation);
_Mylight->color_channel(_Color);

	if (_Sound_enabled) _Mylight->play();
	else _Mylight->stop();

_Mylight->volume(_Sound_volume);
_Mylight->enabled(_Light_enabled);
_Mylight->brightness(_Light_brightness);

create_branch(_Mylight,_Parent_object);
object_created(_Mylight);
return _Mylight;
}

light *static_entity::create_light(const std::string &_Name,int _Draw_order,
	const light &_Light) {
	return create_light(_Name,0,_Draw_order,_Light);
}

light *static_entity::create_light(const std::string &_Name,object *_Parent_object,
	int _Draw_order,const light &_Light) {

	//Parent object doesn't exist
	if (_Parent_object && _Parent_object->owner()!=this)
		return 0;
	
	//Object already exist
	if (object_pointer(_Name)) return 0;

light *_Mylight=new light(this,_Name,_Parent_object?
	_Parent_object->node():_Root_node,_Draw_order,_Light);

create_branch(_Mylight,_Parent_object);
object_created(_Mylight);
return _Mylight;
}

void static_entity::remove_object(object *&_Object) {

	//Nothing to do
	if (_Object->owner()!=this) return;

objects _Objects;
traverse(_Object,_Objects);

_Branch *_Mybranch;

	for (objects::const_iterator _Oiter=_Objects.begin(),
		_Oend=_Objects.end();_Oiter!=_Oend;++_Oiter) {
	
		for (_Branches::iterator _Iter=_Mybranches.begin(),
			_End=_Mybranches.end();_Iter!=_End;++_Iter) {
		
		_Mybranch=(*_Iter); //Increase speed
		
			if (_Mybranch->_Object==(*_Oiter)) {
			
			object_removed(_Mybranch->_Object);
			delete _Mybranch->_Object;
			delete _Mybranch;
			
			_Mybranches.erase(_Iter);
			break;
			}
		}
	}

_Object=0; //Set referenced pointer to null
}

void static_entity::clear_objects() {

_Branch *_Mybranch;

	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter) {
	
	_Mybranch=(*_Iter); //Increase speed
	
	object_removed(_Mybranch->_Object);
	delete _Mybranch->_Object;
	delete _Mybranch;
	}

_Mybranches.clear();
}

object *static_entity::object_pointer(const std::string &_Name) const {

_Branch *_Mybranch=branch_pointer(_Name);
return (_Mybranch?_Mybranch->_Object:0);
}

object *static_entity::object_parent(const std::string &_Name) const {

_Branch *_Mybranch=branch_pointer(_Name);
return (_Mybranch?_Mybranch->_Parent_object:0);
}

void static_entity::object_pointers(objects &_Objects) const {

	for (_Branches::const_iterator _Iter=_Mybranches.begin(),
		_End=_Mybranches.end();_Iter!=_End;++_Iter)
			_Objects.push_back((*_Iter)->_Object);
}

void static_entity::object_pointers(object *_Parent_object,objects &_Objects) const {

	//Nothing to do
	if (_Parent_object && _Parent_object->owner()!=this) return;

traverse(_Parent_object,_Objects);
}