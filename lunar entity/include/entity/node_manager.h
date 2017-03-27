/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: node_manager.h
---------------------------------------------------
*/

#ifndef _NODE_MANAGER_
#define _NODE_MANAGER_

#include <list>
#include <map>

//Dependencies (entity)
#include "interval.h"

//Forward declarations (engine)
namespace lunar {
	class render_node;
} //namespace lunar

//Forward declarations (entity)
class entity_manager;

using namespace lunar;

class node_manager {

	friend entity_manager;

private:

	typedef std::list<render_node*> _Nodes;
	typedef std::map<int,_Nodes> _Node_map;
	
	entity_manager *_Owner;
	render_node *_Node;
	_Node_map _Mynode_map;
	
	//Private constructor
	node_manager(entity_manager *_Owner,render_node *_Parent_node);
	
	//Private copy constructor
	node_manager(const node_manager &_Node_manager);
	
	//Private destructor
	~node_manager();
	
	void copy(const node_manager &_Node_manager);
	
	void store_node(render_node *_Render_node);
	
	_Node_map::iterator highest(const interval &_Draw_order);
	_Node_map::iterator lowest(const interval &_Draw_order);

public:

	node_manager &operator=(const node_manager &_Node_manager);
	
	entity_manager *owner() const;
	
	static render_node &copy_node(render_node &_Destination,
		const render_node &_Source);
	
	void populate_nodes(const interval &_Draw_order,
		unsigned int _Count);
	
	render_node *create_node(render_node *_Parent_node,
		const interval &_Draw_order,bool _Visible=true);
	
	render_node *create_node(render_node *_Parent_node,
		const render_node &_Render_node,const interval &_Draw_order,
			bool _Visible=true);
	
	void recycle_node(render_node *&_Render_node);
	
	void clear_nodes();
};

#endif