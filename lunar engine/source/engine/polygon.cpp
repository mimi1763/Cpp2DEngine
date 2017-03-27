/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: polygon.cpp
---------------------------------------------------
*/

#include "polygon.h"

//Dependencies
#include "system.h"

namespace lunar {

//Protected

polygon::polygon(const color &_Color,bool _Visible):
	renderable(GL_POLYGON,0,_Color,_Visible) {
	//Empty
}

polygon::polygon(const polygon &_Polygon):renderable(_Polygon) {
	copy(_Polygon);
}

polygon::~polygon() {
	//Empty
}

void polygon::copy(const polygon &_Polygon) {
	_Mypoints=_Polygon._Mypoints;
}

void polygon::update_vertices() {

	for (unsigned int i=0;i<_Vertex_count;++i) {
	
	_Vertices[i*2]=_Mypoints[i].x;
	_Vertices[i*2+1]=_Mypoints[i].y;
	}
}

void polygon::update_vertexcount() {

_Vertex_count=(unsigned int)_Mypoints.size();

_Update_vertices=true; //Update vertices
_Update_colors=true; //Update colors
_Reallocate_buffers=true; //More/less vertices, reallocate
}

//Public

polygon &polygon::operator=(const polygon &_Polygon) {

	//Worth doing
	if (this!=&_Polygon) {
	
	renderable::operator=(_Polygon); //Invoke base operator=
	
	//Check if vertex count will differ
	bool _Differ=(_Mypoints.size()!=_Polygon._Mypoints.size());
	copy(_Polygon);
	
		if (_Differ) update_vertexcount();
	}

return *this;
}

bool polygon::prepare() {

	//Nothing to do
	if (_Vertex_count<3) {
	
	clear(); //Clean up
	return false;
	}

return renderable::prepare(); //Use base functionality
}

unsigned int polygon::sides() {
	return (_Mypoints.size()>2?(unsigned int)_Mypoints.size():0);
}

void polygon::add_point(const vector2 &_Point) {

_Mypoints.push_back(_Point);
update_vertexcount();
}

void polygon::change_point(const size_t &_Index,const vector2 &_Point) {

	if (_Index<_Mypoints.size()) {
	
	_Mypoints[_Index]=_Point;
	_Update_vertices=true; //No need to reallocate
	}
}

void polygon::remove_point(const size_t &_Index) {

	if (_Index<_Mypoints.size()) {
	
	_Mypoints.erase(_Mypoints.begin()+_Index);
	update_vertexcount();
	}
}

void polygon::clear_points() {

	if (!_Mypoints.empty()) {
	
	_Mypoints.clear();
	update_vertexcount();
	}
}

} //namespace lunar