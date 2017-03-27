/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: render_node.h
---------------------------------------------------
*/

#ifndef _RENDER_NODE_
#define _RENDER_NODE_

#include <list>
#include <map>

#include "matrix3.h"
#include "callback.h"

namespace lunar {

//Forward declaration
class render_manager;
class attachable;

class render_node {

	friend render_manager;

public:

	enum ROTATION_ORIGIN { PARENT, LOCAL };
	
	typedef std::list<render_node*> render_nodes;

private:

	typedef std::multimap<int,render_node*> _Graph;
	typedef std::list<attachable*> _Attachables;
	
	//Common data
	
	render_node *_Parent_node;
	int _Draw_order;
	bool _Visible;
	
	//Position, rotation and scale data
	
	vector2 _Position;
	vector2 _World_position;
	
	vector2 _Direction;
	vector2 _World_direction;
	vector2 _Initial_direction;
	
	float _Rotation;
	float _World_rotation;
	
	vector2 _Scaling;
	vector2 _World_scaling;
	
	matrix3 _World_transformation;
	float _Glmatrix[16];
	
	ROTATION_ORIGIN _Rotation_origin;
	bool _Inherit_rotation;
	bool _Inherit_scaling;
	
	//Other data
	
	bool _Need_update;
	bool _Update_transformation;
	
	render_nodes _Mychildnodes;
	_Attachables _Myattachables;
	
	callback<render_node*> *_Onpredraw;
	callback<render_node*> *_Onpostdraw;
	
	static _Graph _Mygraph; //Graph with render nodes sorted by draw order
	
	//Private constructor
	explicit render_node(render_node *_Parent_node,int _Draw_order=0,
		const vector2 &_Initial_direction=vector2::UNIT_Y,
			bool _Visible=true);
	
	//Private copy constructor
	render_node(const render_node &_Render_node);
	
	//Private alternative copy constructor
	render_node(render_node *_Parent_node,const render_node &_Render_node);
	
	//Private destructor
	~render_node();
	
	void copy(const render_node &_Render_node);
	
	static void add_rendernode(render_node *_Render_node);
	static void remove_rendernode(render_node *_Render_node);
	
	void notify_update();
	void update();

public:

	typedef render_nodes::const_iterator childnodes_iter; //Keep data safe
	typedef _Attachables::const_iterator attachables_iter; //Keep data safe
	
	render_node &operator=(const render_node &_Render_node);
	
	void traverse(render_nodes &_Myrendernodes) const;
	
	//Common functions
	
	render_node *parent_node() const;
	
	void draw_order(int _Draw_order,bool _Cascade=true);
	int draw_order() const;
	
	void visible(bool _Visible,bool _Cascade=true);
	bool visible() const;
	
	bool axis_aligned();
	
	//Position, rotation and scale functions
	//Relative
	
	void translate(float _Unit);
	void translate(const vector2 &_Unit);
	
	void rotate(float _Angle);
	void scale(const vector2 &_Scale);
	
	//Absolute
	
	void lookat(const vector2 &_Position);
	
	void position(const vector2 &_Position);
	const vector2 &position() const;
	const vector2 &world_position();
	
	void direction(const vector2 &_Direction);
	const vector2 &direction() const;
	const vector2 &world_direction();
	const vector2 &initial_direction() const;
	
	void rotation(float _Angle);
	float rotation() const;
	float world_rotation();
	
	void scaling(const vector2 &_Scaling);
	const vector2 &scaling() const;
	const vector2 &world_scaling();
	
	const matrix3 &world_transformation();
	float *glmatrix();
	
	void rotation_origin(const ROTATION_ORIGIN &_Rotation_origin);
	ROTATION_ORIGIN rotation_origin() const;
	
	void inherit_rotation(bool _Inherit_rotation);
	bool inherit_rotation() const;
	
	void inherit_scaling(bool _Inherit_scaling);
	bool inherit_scaling() const;
	
	//Other functions
	
	void adopt(render_node *_Child_node);
	
	render_node *create_childnode(bool _Visible=true);
	
	render_node *create_childnode(int _Draw_order,
		bool _Visible=true);
	
	render_node *create_childnode(const vector2 &_Initial_direction,
		bool _Visible=true);
	
	render_node *create_childnode(int _Draw_order,
		const vector2 &_Initial_direction,bool _Visible=true);
	
	render_node *create_childnode(const render_node &_Render_node);
	
	void remove_childnode(render_node *&_Child_node);
	void clear_childnodes();
	
	childnodes_iter childnodes_begin() const;
	childnodes_iter childnodes_end() const;
	
	void attach(attachable *_Attachable);
	
	void detach(attachable *_Attachable);
	void detach();
	
	attachables_iter attachables_begin() const;
	attachables_iter attachables_end() const;
	
	//Callbacks
	
	void pre_draw();
	void post_draw();
	
	template <typename _Ty>
	void onpredraw(typename member_callback<_Ty,render_node*>::
		function _Function,_Ty *_Class) {
	
		if (_Onpredraw) release_onpredraw();
	
	_Onpredraw=new member_callback<_Ty,render_node*>(_Function,_Class);
	}
	
	void onpredraw(static_callback<render_node*>::function _Function);
	
	template <typename _Ty>
	void onpostdraw(typename member_callback<_Ty,render_node*>::
		function _Function,_Ty *_Class) {
	
		if (_Onpostdraw) release_onpostdraw();
	
	_Onpostdraw=new member_callback<_Ty,render_node*>(_Function,_Class);
	}
	
	void onpostdraw(static_callback<render_node*>::function _Function);
	
	void release_onpredraw();
	void release_onpostdraw();
};

} //namespace lunar

#endif