/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: renderable.h
---------------------------------------------------
*/

#ifndef _RENDERABLE_
#define _RENDERABLE_

#include "attachable.h"
#include "color.h"

namespace lunar {

//Forward declarations
class scene_manager;

class renderable:public attachable {

protected:

	//Common data
	
	color _Color;
	unsigned int _Draw_mode;
	unsigned int _Vertex_count;
	bool _Point_rendering;
	float _Point_size;
	
	//Geometry data
	
	float *_Vertices;
	float *_Texcoords;
	float *_Colors;
	
	unsigned int _Vertex_vbo;
	unsigned int _Texcoord_vbo;
	unsigned int _Color_vbo;
	
	bool _Update_vertices;
	bool _Update_texcoords;
	bool _Update_colors;
	bool _Reallocate_buffers;
	
	void copy(const renderable &_Renderable);
	
	virtual void update_bounds();
	
	virtual void pre_draw();
	virtual void draw();
	virtual void post_draw();
	
	virtual void update_vertices()=0;
	virtual void update_texcoords();
	virtual void update_colors();
	
	static void update_vertexbuffer(unsigned int &_Vbo,const float *_Data,
		unsigned int _Datasize);

public:

	static const unsigned int VERTEX_COMPONENTS;
	static const unsigned int TEXCOORD_COMPONENTS;
	static const unsigned int COLOR_COMPONENTS;
	
	//Constructor
	renderable(unsigned int _Draw_mode,unsigned int _Vertex_count,
		const color &_Color=color::WHITE,bool _Visible=true);
	
	//Copy constructor
	renderable(const renderable &_Renderable);
	
	//Virtual destructor
	virtual ~renderable();
	
	renderable &operator=(const renderable &_Renderable);
	
	virtual bool prepare();
	virtual geometry_stats statistics() const;
	
	//Common functions
	
	void color_value(const color &_Color);
	const color &color_value() const;
	
	unsigned int draw_mode() const;
	unsigned int vertex_count() const;
	
	void point_rendering(bool _Point_rendering);
	bool point_rendering() const;
	
	void point_size(float _Point_size);
	float point_size() const;
	
	//Geometry functions
	
	void clear();
	
	void clear_vertices();
	void clear_texcoords();
	void clear_colors();
	
	float *vertices() const;
	float *texcoords() const;
	float *colors() const;
	
	unsigned int vertex_vbo() const;
	unsigned int texcoord_vbo() const;
	unsigned int color_vbo() const;
};

} //namespace lunar

#endif