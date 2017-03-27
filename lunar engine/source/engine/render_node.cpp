/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_node.cpp
---------------------------------------------------
*/

#include "render_node.h"

//Dependencies
#include <cmath>

#include "attachable.h"
#include "math_utility.h"

namespace lunar {

//Private

//Initialize static std::multimap
render_node::_Graph render_node::_Mygraph;

render_node::render_node(render_node *_Parent_node,int _Draw_order,
	const vector2 &_Initial_direction,bool _Visible):
		_Parent_node(_Parent_node),_Draw_order(_Draw_order),_Visible(_Visible),
			_Direction(_Initial_direction),_World_direction(_Initial_direction),
				_Initial_direction(_Initial_direction),_Rotation(0.0f),
					_World_rotation(0.0f),_Scaling(vector2::UNIT_SCALE),
						_World_scaling(vector2::UNIT_SCALE),_Rotation_origin(PARENT),
							_Inherit_rotation(true),_Inherit_scaling(true),
								_Need_update(true),_Update_transformation(true),
									_Onpredraw(0),_Onpostdraw(0) {
	//Empty
}

render_node::render_node(const render_node &_Render_node):
	_Parent_node(_Render_node._Parent_node),
		_Initial_direction(_Render_node._Initial_direction) {

_Need_update=true; //Not copyable
_Update_transformation=true; //Not copyable

_Onpredraw=0; //No callback
_Onpostdraw=0; //No callback
copy(_Render_node);
}

render_node::render_node(render_node *_Parent_node,
	const render_node &_Render_node):_Parent_node(_Parent_node),
		_Initial_direction(_Render_node._Initial_direction) {

_Need_update=true; //Not copyable
_Update_transformation=true; //Not copyable

_Onpredraw=0; //No callback
_Onpostdraw=0; //No callback
copy(_Render_node);
}

render_node::~render_node() {

clear_childnodes();

release_onpredraw();
release_onpostdraw();
}

void render_node::copy(const render_node &_Render_node) {

_Draw_order=_Render_node._Draw_order;
_Visible=_Render_node._Visible;

_Position=_Render_node._Position;
_Direction=_Render_node._Direction;
_Rotation=_Render_node._Rotation;
_Scaling=_Render_node._Scaling;

_Rotation_origin=_Render_node._Rotation_origin;
_Inherit_rotation=_Render_node._Inherit_rotation;
_Inherit_scaling=_Render_node._Inherit_scaling;
}

void render_node::add_rendernode(render_node *_Render_node) {
	_Mygraph.insert(_Graph::value_type(_Render_node->_Draw_order,_Render_node));
}

void render_node::remove_rendernode(render_node *_Render_node) {

render_node *_Node;

	for (_Graph::iterator _Iter=_Mygraph.find(_Render_node->_Draw_order),
		_End=_Mygraph.end();_Iter!=_End;++_Iter) {
	
	_Node=(*_Iter).second; //Increase speed
	
		//Could not find render node in graph
		if (_Node->_Draw_order!=_Render_node->_Draw_order) return;
		
		//Found render node
		if (_Node==_Render_node) {
		
		_Mygraph.erase(_Iter);
		return;
		}
	}
}

void render_node::notify_update() {

_Need_update=true;

	//Notify all child nodes recursively
	for (render_nodes::const_iterator _Iter=_Mychildnodes.begin(),
		_End=_Mychildnodes.end();_Iter!=_End;++_Iter)
			(*_Iter)->notify_update();
}

void render_node::update() {

	if (_Parent_node) {
	
	float _Parent_rotation=_Parent_node->world_rotation();
	const vector2 &_Parent_scaling=_Parent_node->world_scaling();
	
	_World_rotation=(_Inherit_rotation?_Rotation+_Parent_rotation:_Rotation);
	_World_scaling=(_Inherit_scaling?_Scaling*_Parent_scaling:_Scaling);
	
	_World_direction=_Initial_direction.deviant(_World_rotation);
	
		if (_Rotation_origin==LOCAL)
			_World_position=_Position*_Parent_scaling;
		else {
		
		float _Sin_angle=math::sine(_Parent_rotation);
		float _Cos_angle=math::cosine(_Parent_rotation);
		
		//Rotate position*scaling
		_World_position=vector2(
			_Cos_angle*(_Position.x*_Parent_scaling.x)-
			_Sin_angle*(_Position.y*_Parent_scaling.y),
			_Sin_angle*(_Position.x*_Parent_scaling.x)+
			_Cos_angle*(_Position.y*_Parent_scaling.y));
		}
	
	_World_position+=_Parent_node->world_position();
	}
	else { //Root
	
	_World_position=_Position;
	_World_rotation=_Rotation;
	_World_scaling=_Scaling;
	}

_Update_transformation=true;
_Need_update=false;
}

//Public

render_node &render_node::operator=(const render_node &_Render_node) {

	//Worth doing
	if (this!=&_Render_node) {
	
	bool _Rearrange=(_Draw_order!=_Render_node._Draw_order);
	
		if (_Rearrange) remove_rendernode(this);
	
	copy(_Render_node);
	
		if (_Rearrange) add_rendernode(this);
	
	notify_update(); //Force update
	}

return *this;
}

void render_node::traverse(render_nodes &_Myrendernodes) const {

//Depth first search traverse
_Myrendernodes.push_back((render_node*)this);

	//Traverse child nodes recursively
	for (render_nodes::const_iterator _Iter=_Mychildnodes.begin(),
		_End=_Mychildnodes.end();_Iter!=_End;++_Iter)
			(*_Iter)->traverse(_Myrendernodes);
}

//Common functions

render_node *render_node::parent_node() const {
	return _Parent_node;
}

void render_node::draw_order(int _Draw_order,bool _Cascade) {

	if (this->_Draw_order!=_Draw_order) {
	
		if (_Cascade) {
		
		int _Difference=_Draw_order-this->_Draw_order;
		
		render_node *_Render_node;
		
			//Traverse child nodes recursively
			for (render_nodes::const_iterator _Iter=_Mychildnodes.begin(),
				_End=_Mychildnodes.end();_Iter!=_End;++_Iter) {
			
			_Render_node=(*_Iter); //Increase speed
			_Render_node->draw_order(_Render_node->_Draw_order+_Difference);
			}
		}
	
	remove_rendernode(this);
	this->_Draw_order=_Draw_order;
	add_rendernode(this); //Rearrange
	}
}

int render_node::draw_order() const {
	return _Draw_order;
}

void render_node::visible(bool _Visible,bool _Cascade) {

this->_Visible=_Visible;

	if (_Cascade) {
	
		//Traverse child nodes recursively
		for (render_nodes::const_iterator _Iter=_Mychildnodes.begin(),
			_End=_Mychildnodes.end();_Iter!=_End;++_Iter)
				(*_Iter)->visible(_Visible);
	}
}

bool render_node::visible() const {
	return _Visible;
}

bool render_node::axis_aligned() {
	//Axis aligned when 0, +-90, +-180, +-270 and +-360 (one degree tolerance)
	return ((int)math::radian(world_rotation())%90==0);
}

//Position, rotation and scale functions
//Relative

void render_node::translate(float _Unit) {

	if (_Unit!=0.0f) position(_Position+(_Direction*_Unit));
}

void render_node::translate(const vector2 &_Unit) {

	if (_Unit!=vector2::ZERO) {
	
	//The given unit vector is relative to UNIT_Y
	//atan2(y,x) -> atan2(1,0) is equal to half pi
	float _Angle=atan2(_Direction.y,_Direction.x)-math::HALF_PI;
	position(_Position+_Unit.deviant(_Angle));
	}
}

void render_node::rotate(float _Angle) {

	if (_Angle!=0.0f) rotation(_Rotation+_Angle);
}

void render_node::scale(const vector2 &_Scale) {

	if (_Scale!=vector2::ZERO) scaling(_Scaling+_Scale);
}

//Absolute

void render_node::lookat(const vector2 &_Position) {

vector2 _Lookat_direction=_Position-world_position();
const vector2 &_World_direction=world_direction();

rotate(atan2(_Lookat_direction.y,_Lookat_direction.x)-
	atan2(_World_direction.y,_World_direction.x));
}

void render_node::position(const vector2 &_Position) {

	if (this->_Position!=_Position) {
	
	this->_Position=_Position;
	notify_update();
	}
}

const vector2 &render_node::position() const {
	return _Position;
}

const vector2 &render_node::world_position() {

	if (_Need_update) update();

return _World_position;
}

void render_node::direction(const vector2 &_Direction) {

	if (this->_Direction!=_Direction) {
	
	rotation(atan2(_Direction.y,_Direction.x)-atan2(
		_Initial_direction.y,_Initial_direction.x)); //Update rotation
	
	//An axproximate direction is calculated by rotation
	//Override with exact value when given explicit
	this->_Direction=_Direction;
	}
}

const vector2 &render_node::direction() const {
	return _Direction;
}

const vector2 &render_node::world_direction() {

	if (_Need_update) update();

return _World_direction;
}

const vector2 &render_node::initial_direction() const {
	return _Initial_direction;
}

void render_node::rotation(float _Angle) {

	if (_Rotation!=_Angle) {
	
	_Direction=_Initial_direction.deviant(_Angle); //Update direction
	
	_Rotation=_Angle;
	notify_update();
	}
}

float render_node::rotation() const {
	return _Rotation;
}

float render_node::world_rotation() {

	if (_Need_update) update();

return _World_rotation;
}

void render_node::scaling(const vector2 &_Scaling) {

	if (this->_Scaling!=_Scaling) {
	
	this->_Scaling=_Scaling;
	notify_update();
	}
}

const vector2 &render_node::scaling() const {
	return _Scaling;
}

const vector2 &render_node::world_scaling() {

	if (_Need_update) update();

return _World_scaling;
}

const matrix3 &render_node::world_transformation() {

	if (_Need_update || _Update_transformation) {
	
	_World_transformation.transform(world_position(),world_scaling(),
		world_rotation());
	_Update_transformation=false;
	}

return _World_transformation;
}

float *render_node::glmatrix() {

const matrix3 &_Matrix=world_transformation();

//Create an OpenGL 4x4 matrix from the world transformation matrix
_Glmatrix[0]=_Matrix.m[0][0]; _Glmatrix[4]=_Matrix.m[0][1]; _Glmatrix[8]=0.0f;  _Glmatrix[12]=_Matrix.m[0][2];
_Glmatrix[1]=_Matrix.m[1][0]; _Glmatrix[5]=_Matrix.m[1][1]; _Glmatrix[9]=0.0f;  _Glmatrix[13]=_Matrix.m[1][2];
_Glmatrix[2]=0.0f;			   _Glmatrix[6]=0.0f;			  _Glmatrix[10]=1.0f; _Glmatrix[14]=0.0f;
_Glmatrix[3]=0.0f;			   _Glmatrix[7]=0.0f;			  _Glmatrix[11]=0.0f; _Glmatrix[15]=1.0f;

return _Glmatrix;
}

void render_node::rotation_origin(const ROTATION_ORIGIN &_Rotation_origin) {

	if (this->_Rotation_origin!=_Rotation_origin) {
	
	this->_Rotation_origin=_Rotation_origin;
	notify_update();
	}
}

render_node::ROTATION_ORIGIN render_node::rotation_origin() const {
	return _Rotation_origin;
}

void render_node::inherit_rotation(bool _Inherit_rotation) {

	if (this->_Inherit_rotation!=_Inherit_rotation) {
	
	this->_Inherit_rotation=_Inherit_rotation;
	notify_update();
	}
}

bool render_node::inherit_rotation() const {
	return _Inherit_rotation;
}

void render_node::inherit_scaling(bool _Inherit_scaling) {

	if (this->_Inherit_scaling!=_Inherit_scaling) {
	
	this->_Inherit_scaling=_Inherit_scaling;
	notify_update();
	}
}

bool render_node::inherit_scaling() const {
	return _Inherit_scaling;
}

//Other functions

void render_node::adopt(render_node *_Child_node) {

	//Worth doing
	if (_Child_node->_Parent_node && this!=_Child_node->_Parent_node) {
	
		for (render_nodes::iterator _Iter=_Child_node->_Parent_node->
			_Mychildnodes.begin(),_End=_Child_node->_Parent_node->
				_Mychildnodes.end();_Iter!=_End;++_Iter) {
		
			if ((*_Iter)==_Child_node) {
			
			_Child_node->_Parent_node->_Mychildnodes.erase(_Iter); //Orphan
			
			_Child_node->_Parent_node=this;
			_Child_node->notify_update();
			_Mychildnodes.push_back(_Child_node); //Adopt
			return;
			}
		}
	}
}

render_node *render_node::create_childnode(bool _Visible) {
	return create_childnode(_Draw_order+1,vector2::UNIT_Y,
		_Visible); //Draw node one layer higher than parent
}

render_node *render_node::create_childnode(int _Draw_order,
	bool _Visible) {
	return create_childnode(_Draw_order,vector2::UNIT_Y,
		_Visible);
}

render_node *render_node::create_childnode(const vector2 &_Initial_direction,
	bool _Visible) {
	return create_childnode(_Draw_order+1,_Initial_direction,
		_Visible); //Draw node one layer higher than parent
}

render_node *render_node::create_childnode(int _Draw_order,
	const vector2 &_Initial_direction,bool _Visible) {

render_node *_Child_node=new render_node(this,_Draw_order,
	_Initial_direction,_Visible);
_Mychildnodes.push_back(_Child_node); //Add node to tree
add_rendernode(_Child_node); //Add node to graph
return _Child_node;
}

render_node *render_node::create_childnode(const render_node &_Render_node) {

render_node *_Child_node=new render_node(this,_Render_node);
_Mychildnodes.push_back(_Child_node); //Add node to tree
add_rendernode(_Child_node); //Add node to graph
return _Child_node;
}

void render_node::remove_childnode(render_node *&_Child_node) {

	for (render_nodes::iterator _Iter=_Mychildnodes.begin(),
		_End=_Mychildnodes.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Child_node) {
		
		_Child_node->detach(); //Detach all attachables
		remove_rendernode(_Child_node); //Remove node from graph
		
		delete _Child_node;
		_Child_node=0; //Set referenced pointer to null
		
		_Mychildnodes.erase(_Iter);
		return;
		}
	}
}

void render_node::clear_childnodes() {

render_node *_Render_node;

	//Recursively delete all child nodes
	for (render_nodes::const_iterator _Iter=_Mychildnodes.begin(),
		_End=_Mychildnodes.end();_Iter!=_End;++_Iter) {
	
	_Render_node=(*_Iter);
	
	_Render_node->detach(); //Detach all attachables
	remove_rendernode(_Render_node); //Remove node from graph
	
	delete _Render_node;
	}

_Mychildnodes.clear();
}

render_node::childnodes_iter render_node::childnodes_begin() const {
	return _Mychildnodes.begin();
}

render_node::childnodes_iter render_node::childnodes_end() const {
	return _Mychildnodes.end();
}

void render_node::attach(attachable *_Attachable) {

	if (!_Attachable->parent_node()) {
	
	_Attachable->parent_node(this); //Set this node as parent
	_Myattachables.push_back(_Attachable);
	}
}

void render_node::detach(attachable *_Attachable) {

	for (_Attachables::iterator _Iter=_Myattachables.begin(),
		_End=_Myattachables.end();_Iter!=_End;++_Iter) {
	
		if ((*_Iter)==_Attachable) {
		
		_Attachable->parent_node(0); //Remove this node as parent
		_Myattachables.erase(_Iter); //Only erase node
		return;
		}
	}
}

void render_node::detach() {

	for (_Attachables::const_iterator _Iter=_Myattachables.begin(),
		_End=_Myattachables.end();_Iter!=_End;++_Iter)
			(*_Iter)->parent_node(0); //Remove this node as parent

_Myattachables.clear();
}

render_node::attachables_iter render_node::attachables_begin() const {
	return _Myattachables.begin();
}

render_node::attachables_iter render_node::attachables_end() const {
	return _Myattachables.end();
}

//Callbacks

void render_node::pre_draw() {

	if (_Onpredraw) (*_Onpredraw)(this);
}

void render_node::post_draw() {

	if (_Onpostdraw) (*_Onpostdraw)(this);
}

void render_node::onpredraw(static_callback<render_node*>::function _Function) {

	if (!_Onpredraw) _Onpredraw=new static_callback<render_node*>(_Function);
}

void render_node::onpostdraw(static_callback<render_node*>::function _Function) {

	if (!_Onpostdraw) _Onpostdraw=new static_callback<render_node*>(_Function);
}

void render_node::release_onpredraw() {

	if (_Onpredraw) {
	
	delete _Onpredraw;
	_Onpredraw=0;
	}
}

void render_node::release_onpostdraw() {

	if (_Onpostdraw) {
	
	delete _Onpostdraw;
	_Onpostdraw=0;
	}
}

} //namespace lunar