/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: renderable.cpp
---------------------------------------------------
*/

#include "renderable.h"

//Dependencies
#include <cmath>
#include "system.h"

#include "render_node.h"
#include "math_utility.h"
#include "support.h"

namespace lunar {

//Protected

void renderable::copy(const renderable &_Renderable) {

_Color=_Renderable._Color;
_Draw_mode=_Renderable._Draw_mode;
_Vertex_count=_Renderable._Vertex_count;
_Point_rendering=_Renderable._Point_rendering;
_Point_size=_Renderable._Point_size;
}

void renderable::update_bounds() {

vector2 _Min(_Vertices[0],_Vertices[1]); //Initial x
vector2 _Max(_Vertices[0],_Vertices[1]); //Initial y

unsigned int _Count=_Vertex_count*VERTEX_COMPONENTS;

	for (unsigned int i=2;i<_Count;i+=2) {
	
		if (_Vertices[i]<_Min.x) _Min.x=_Vertices[i];
		if (_Vertices[i]>_Max.x) _Max.x=_Vertices[i];
		
		if (_Vertices[i+1]<_Min.y) _Min.y=_Vertices[i+1];
		if (_Vertices[i+1]>_Max.y) _Max.y=_Vertices[i+1];
	}

create_bounds(_Min,_Max);
}

void renderable::pre_draw() {

	if (_Point_rendering) glPointSize(_Point_size);

float *_Vertex_data=_Vertices;
float *_Texcoord_data=_Texcoords;
float *_Color_data=_Colors;

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);

	if (support::vbo()==support::OPENGL) {
	
	glBindBuffer(GL_ARRAY_BUFFER,_Vertex_vbo);
	_Vertex_data=0; //No need for data transfer
	}
	else if (support::vbo()==support::ARB) {
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,_Vertex_vbo);
	_Vertex_data=0; //No need for data transfer
	}

glVertexPointer(VERTEX_COMPONENTS,GL_FLOAT,0,_Vertex_data);

	if (support::vbo()==support::OPENGL) {
	
	glBindBuffer(GL_ARRAY_BUFFER,_Color_vbo);
	_Color_data=0; //No need for data transfer
	}
	else if (support::vbo()==support::ARB) {
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,_Color_vbo);
	_Color_data=0; //No need for data transfer
	}

glColorPointer(COLOR_COMPONENTS,GL_FLOAT,0,_Color_data);

	//Draw with texture coordinates
	if (_Texcoord_data) {
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
		if (support::vbo()==support::OPENGL) {
		
		glBindBuffer(GL_ARRAY_BUFFER,_Texcoord_vbo);
		_Texcoord_data=0; //No need for data transfer
		}
		else if (support::vbo()==support::ARB) {
		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,_Texcoord_vbo);
		_Texcoord_data=0; //No need for data transfer
		}
	
	glTexCoordPointer(TEXCOORD_COMPONENTS,GL_FLOAT,0,_Texcoord_data);
	}
}

void renderable::draw() {
	glDrawArrays(draw_mode(),0,_Vertex_count); //Draw geometry
}

void renderable::post_draw() {

glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//Disable vertex buffer object
	if (support::vbo()==support::OPENGL)
		glBindBuffer(GL_ARRAY_BUFFER,0);
	else if (support::vbo()==support::ARB)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB,0);
}

void renderable::update_texcoords() {
	//Empty - override if needed
}

void renderable::update_colors() {

//Supports one color per geometry
//Use a shader to allow one color per vertex

unsigned int _Color_count=_Vertex_count*COLOR_COMPONENTS;

	for (unsigned int i=0;i<_Color_count;i+=COLOR_COMPONENTS) {
	
	_Colors[i]=_Color.r;
	_Colors[i+1]=_Color.g;
	_Colors[i+2]=_Color.b;
	_Colors[i+3]=_Color.a;
	}
}

void renderable::update_vertexbuffer(unsigned int &_Vbo,const float *_Data,
	unsigned int _Datasize) {

	//Create a vertex buffer object and bind it to the data
	//Vertex buffer objects stores geometry info in VRAM
	if (support::vbo()==support::OPENGL) {
	
		if (_Vbo==0) glGenBuffers(1,&_Vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER,_Vbo);
	glBufferData(GL_ARRAY_BUFFER,_Datasize*sizeof(float),_Data,
		GL_DYNAMIC_DRAW);
	}
	else if (support::vbo()==support::ARB) {
	
		if (_Vbo==0) glGenBuffersARB(1,&_Vbo);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB,_Vbo);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB,_Datasize*sizeof(float),_Data,
		GL_DYNAMIC_DRAW_ARB);
	}
}

//Public

const unsigned int renderable::VERTEX_COMPONENTS=2; //[x, y]
const unsigned int renderable::TEXCOORD_COMPONENTS=2; //[s, t]
const unsigned int renderable::COLOR_COMPONENTS=4; //[r, g, b, a]

renderable::renderable(unsigned int _Draw_mode,unsigned int _Vertex_count,
	const color &_Color,bool _Visible):attachable(_Visible),
		_Color(_Color),_Draw_mode(_Draw_mode),_Vertex_count(_Vertex_count),
			_Point_rendering(false),_Point_size(2.0f),_Vertices(0),_Texcoords(0),
				_Colors(0),_Vertex_vbo(0),_Texcoord_vbo(0),_Color_vbo(0),
					_Update_vertices(true),_Update_texcoords(false),
						_Update_colors(true),_Reallocate_buffers(false) {
	//Empty
}

renderable::renderable(const renderable &_Renderable):
	attachable(_Renderable) {

//Not copyable
_Vertices=0;
_Texcoords=0;
_Colors=0;

//Not copyable
_Vertex_vbo=0;
_Texcoord_vbo=0;
_Color_vbo=0;

//Not copyable
_Update_vertices=true;
_Update_texcoords=false;
_Update_colors=true;
_Reallocate_buffers=false;

copy(_Renderable);
}

renderable::~renderable() {
	clear();
}

renderable &renderable::operator=(const renderable &_Renderable) {

	//Worth doing
	if (this!=&_Renderable) {
	
	attachable::operator=(_Renderable); //Invoke base operator=
	
	//Check if vertex count will differ
	bool _Differ=(_Vertex_count!=_Renderable._Vertex_count);
	copy(_Renderable);
	
	_Update_vertices=true; //Update vertices
	_Update_colors=true; //Update colors
	
		//Reallocate if vertex count differs
		if (_Differ) _Reallocate_buffers=true;
	}

return *this;
}

bool renderable::prepare() {

	//Nothing to do
	if (_Vertex_count==0) {
	
	clear(); //Clean up
	return false;
	}
	
	//Update vertices
	if (_Update_vertices) {
	
		if (_Reallocate_buffers) clear_vertices();		
		if (!_Vertices) _Vertices=new float[_Vertex_count*VERTEX_COMPONENTS];
	
	update_vertices();
	update_vertexbuffer(_Vertex_vbo,_Vertices,_Vertex_count*VERTEX_COMPONENTS);
	_Update_vertices=false;
	
	_Update_bounds=true; //New vertices, refresh bounds
	}
	
	//Update texture coordinates
	if (_Update_texcoords) {
	
		if (_Reallocate_buffers) clear_texcoords();
		if (!_Texcoords) _Texcoords=new float[_Vertex_count*TEXCOORD_COMPONENTS];
	
	update_texcoords();
	update_vertexbuffer(_Texcoord_vbo,_Texcoords,_Vertex_count*TEXCOORD_COMPONENTS);
	_Update_texcoords=false;
	}
	
	//Update colors
	if (_Update_colors) {
	
		if (_Reallocate_buffers) clear_colors();
		if (!_Colors) _Colors=new float[_Vertex_count*COLOR_COMPONENTS];
	
	update_colors();
	update_vertexbuffer(_Color_vbo,_Colors,_Vertex_count*COLOR_COMPONENTS);
	_Update_colors=false;
	}

_Reallocate_buffers=false;
return attachable::prepare(); //Use base functionality
}

attachable::geometry_stats renderable::statistics() const {

geometry_stats _Geometry_stats;
_Geometry_stats.polygons=1;
_Geometry_stats.vertices=_Vertex_count;
return _Geometry_stats;
}

//Common functions

void renderable::color_value(const color &_Color) {

	if (this->_Color!=_Color) {
	
	this->_Color=_Color;
	_Update_colors=true; //Update colors
	}
}

const color &renderable::color_value() const {
	return _Color;
}

unsigned int renderable::draw_mode() const {
	return (_Point_rendering?GL_POINTS:_Draw_mode);
}

unsigned int renderable::vertex_count() const {
	return _Vertex_count;
}

void renderable::point_rendering(bool _Point_rendering) {
	this->_Point_rendering=_Point_rendering;
}

bool renderable::point_rendering() const {
	return _Point_rendering;
}

void renderable::point_size(float _Point_size) {
	this->_Point_size=_Point_size;
}

float renderable::point_size() const {
	return _Point_size;
}

//Geometry functions

void renderable::clear() {

clear_vertices();
clear_texcoords();
clear_colors();
}

void renderable::clear_vertices() {

	if (_Vertices) {
	
		//Deallocate vertex buffer object
		if (support::vbo()==support::OPENGL) glDeleteBuffers(1,&_Vertex_vbo);
		else if (support::vbo()==support::ARB) glDeleteBuffersARB(1,&_Vertex_vbo);
	
	delete[] _Vertices;
	_Vertices=0;
	_Vertex_vbo=0;
	_Update_vertices=true;
	}
}

void renderable::clear_texcoords() {

	if (_Texcoords) {
	
		//Deallocate vertex buffer object
		if (support::vbo()==support::OPENGL) glDeleteBuffers(1,&_Texcoord_vbo);
		else if (support::vbo()==support::ARB) glDeleteBuffersARB(1,&_Texcoord_vbo);
	
	delete[] _Texcoords;
	_Texcoords=0;
	_Texcoord_vbo=0;
	_Update_texcoords=true;
	}
}

void renderable::clear_colors() {

	if (_Colors) {
	
		//Deallocate vertex buffer object
		if (support::vbo()==support::OPENGL) glDeleteBuffers(1,&_Color_vbo);
		else if (support::vbo()==support::ARB) glDeleteBuffersARB(1,&_Color_vbo);
	
	delete[] _Colors;
	_Colors=0;
	_Color_vbo=0;
	_Update_colors=true;
	}
}

float *renderable::vertices() const {
	return _Vertices;
}

float *renderable::texcoords() const {
	return _Texcoords;
}

float *renderable::colors() const {
	return _Colors;
}

unsigned int renderable::vertex_vbo() const {
	return _Vertex_vbo;
}

unsigned int renderable::texcoord_vbo() const {
	return _Texcoord_vbo;
}

unsigned int renderable::color_vbo() const {
	return _Color_vbo;
}

} //namespace lunar