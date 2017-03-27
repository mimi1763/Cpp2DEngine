/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: node_manager.cpp
---------------------------------------------------
*/

#include "node_manager.h"

//Dependencies (engine)
#include "render_node.h"
#include "entity_manager.h"

//Private

node_manager::node_manager(entity_manager *_Owner,render_node *_Parent_node):
	_Owner(_Owner) {
	_Node=_Parent_node->create_childnode(
		_Owner->node()->draw_order(),false);
}

node_manager::node_manager(const node_manager &_Node_manager):
	_Owner(_Node_manager._Owner) {

_Node=_Node_manager._Node->parent_node()->create_childnode(
	_Node_manager._Node->draw_order(),_Node_manager._Node->visible());
copy(_Node_manager);
}

node_manager::~node_manager() {
	_Node->parent_node()->remove_childnode(_Node);
}

void node_manager::copy(const node_manager &_Node_manager) {
	//Empty - override default memcpy behaviour
}

void node_manager::store_node(render_node *_Render_node) {

_Node_map::iterator _Iter=_Mynode_map.find(_Render_node->draw_order());

	//Found draw order
	if (_Iter!=_Mynode_map.end())
		(*_Iter).second.push_back(_Render_node);
	else {
	
	_Node_map::_Pairib _Pair=_Mynode_map.insert(
		_Node_map::value_type(_Render_node->draw_order(),_Nodes()));
	(*_Pair.first).second.push_back(_Render_node);
	}
}

node_manager::_Node_map::iterator node_manager::highest(
	const interval &_Draw_order) {

_Node_map::iterator _Iter=_Mynode_map.lower_bound(_Draw_order.to());

	//Check greatest less
	if (_Iter==_Mynode_map.end() || (*_Iter).first>_Draw_order.to())
		--_Iter; //Safe for map iterator
	
	//Found iterator inside draw order interval
	if (_Iter!=_Mynode_map.end() && (*_Iter).first>=_Draw_order.from())
		return _Iter;

return _Mynode_map.end();
}

node_manager::_Node_map::iterator node_manager::lowest(
	const interval &_Draw_order) {

_Node_map::iterator _Iter=_Mynode_map.lower_bound(_Draw_order.from());

	//Found iterator inside draw order interval
	if (_Iter!=_Mynode_map.end() && (*_Iter).first<=_Draw_order.to())
		return _Iter;

return _Mynode_map.end();
}

//Public

node_manager &node_manager::operator=(const node_manager &_Node_manager) {

	//Worth doing
	if (this!=&_Node_manager) copy(_Node_manager);

return *this;
}

entity_manager *node_manager::owner() const {
	return _Owner;
}

render_node &node_manager::copy_node(render_node &_Destination,
	const render_node &_Source) {

	//Worth doing
	if (&_Destination!=&_Source) {
	
	//Don't use render_node::operator=, because this may change draw order
	
	_Destination.visible(_Source.visible(),false);
	
	_Destination.position(_Source.position());
	_Destination.rotation(_Source.rotation());
	_Destination.scaling(_Source.scaling());
	
	_Destination.rotation_origin(_Source.rotation_origin());
	_Destination.inherit_rotation(_Source.inherit_rotation());
	_Destination.inherit_scaling(_Source.inherit_scaling());
	}

return _Destination;
}

void node_manager::populate_nodes(const interval &_Draw_order,
	unsigned int _Count) {

	//Nothing to do
	if (_Count==0) return;

int _From=_Draw_order.from();
int _To=_Draw_order.to();

	//Populate in interval [from,<-to]
	if (_Draw_order.policy()==interval::HIGHEST) {
	
		for (unsigned int i=0;i<_Count;++i)
			store_node(_Node->create_childnode(
				_To-(i%(_To-_From+1)),false));
	}
	//Populate in interval [from->,to]
	else if (_Draw_order.policy()==interval::LOWEST) {
	
		for (unsigned int i=0;i<_Count;++i)
			store_node(_Node->create_childnode(
				_From+(i%(_To-_From+1)),false));
	}
}

render_node *node_manager::create_node(render_node *_Parent_node,
	const interval &_Draw_order,bool _Visible) {

	//Nothing to do
	if (!_Parent_node) return 0;
	
	if (!_Mynode_map.empty()) {
	
	_Node_map::iterator _Iter=(_Draw_order.policy()==interval::LOWEST?
		lowest(_Draw_order):highest(_Draw_order));
	
		//Found iterator inside draw order interval
		//Use recycled node
		if (_Iter!=_Mynode_map.end()) {
		
		_Nodes &_Mynodes=(*_Iter).second;
		render_node *_Render_node=_Mynodes.front();
		_Mynodes.pop_front();
		
			//Last node, remove draw order from node map
			if (_Mynodes.empty()) _Mynode_map.erase(_Iter);
		
		_Parent_node->adopt(_Render_node);
		_Render_node->visible(_Visible,false);
		return _Render_node;
		}
	}

return _Parent_node->create_childnode(_Draw_order.policy()==interval::LOWEST?
	_Draw_order.from():_Draw_order.to(),_Visible);
}

render_node *node_manager::create_node(render_node *_Parent_node,
	const render_node &_Render_node,const interval &_Draw_order,
		bool _Visible) {
	return &copy_node(*create_node(_Parent_node,_Draw_order,_Visible),
		_Render_node);
}

void node_manager::recycle_node(render_node *&_Render_node) {

	//Nothing to do
	if (!_Render_node) return;
	
	//Check if node is already recycled
	if (_Render_node->parent_node()==this->_Node) {
	
	_Render_node=0; //Recycled
	return;
	}

_Render_node->visible(false);

render_node::render_nodes _Myrender_nodes;
_Render_node->traverse(_Myrender_nodes);

render_node *_Node;

	for (render_node::render_nodes::const_iterator _Iter=_Myrender_nodes.begin(),
		_End=_Myrender_nodes.end();_Iter!=_End;++_Iter) {
	
	_Node=(*_Iter); //Increase speed
	
	//Detach all attachables
	_Node->detach();
	
	//Release callbacks
	_Node->release_onpredraw();
	_Node->release_onpostdraw();
	
	//Reset transformations
	_Node->position(vector2::ZERO);
	_Node->rotation(0.0f);
	_Node->scaling(vector2::UNIT_SCALE);
	
	_Node->rotation_origin(render_node::PARENT);
	_Node->inherit_rotation(true);
	_Node->inherit_scaling(true);
	
	//Adopt and store for recycling
	this->_Node->adopt(_Node);
	store_node(_Node);
	}

_Render_node=0; //Recycled
}

void node_manager::clear_nodes() {

_Node->clear_childnodes();
_Mynode_map.clear();
}