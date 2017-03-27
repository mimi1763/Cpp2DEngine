/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: lighting_manager.h
---------------------------------------------------
*/

#ifndef _LIGHTING_MANAGER_
#define _LIGHTING_MANAGER_

//Dependencies (engine)
#include "capture.h"

//Forward declarations (engine)
namespace lunar {

class framebuffer;
class render_node;
} //namespace lunar

//Dependencies (entity)
#include "interval.h"

//Forward declarations (entity)
class entity_manager;

using namespace lunar;

class lighting_manager {

	friend entity_manager;

private:

	entity_manager *_Owner;
	int _Overlay_layer;
	interval _Capture_layer;
	
	//Light/shadow
	
	vector2 _Light_direction;
	color _Overlay_color;
	color _Shadowcolor_day;
	color _Shadowcolor_night;
	bool _Cast_shadows;
	bool _Lighting_changed;
	
	//Light map
	
	framebuffer *_Lightmap;
	capture _Lightmap_capture;
	
	render_node *_Node;
	render_node *_Pre_capturenode;
	render_node *_Post_capturenode;
	
	//Private constructor
	lighting_manager(entity_manager *_Owner,render_node *_Parent_node,
		int _Overlay_layer,const interval &_Capture_layer);
	
	//Private copy constructor
	lighting_manager(const lighting_manager &_Lighting_manager);
	
	//Private destructor
	~lighting_manager();
	
	void copy(const lighting_manager &_Lighting_manager);
	
	void init();
	
	void pre_draw(render_node *_Render_node);
	void post_draw(render_node *_Render_node);

public:

	lighting_manager &operator=(const lighting_manager &_Lighting_manager);
	
	entity_manager *owner() const;
	
	//Light/shadow
	
	void light_direction(const vector2 &_Light_direction);
	const vector2 &light_direction() const;
	
	void overlay_visible(bool _Visible);
	bool overlay_visible() const;
	
	void overlay_color(const color &_Color);
	const color &overlay_color() const;
	
	void overlay_opacity(float _Opacity);
	float overlay_opacity() const;
	
	void shadow_color(const color &_Color_day,const color &_Color_night);
	const color &shadowcolor_day() const;
	const color &shadowcolor_night() const;
	
	void cast_shadows(bool _Cast_shadows);
	bool cast_shadows() const;
	
	bool lighting_changed() const;
	
	//Light map
	
	void overlay_layer(int _Overlay_layer);
	int overlay_layer() const;
	
	void capture_layer(const interval &_Capture_layer);
	interval capture_layer() const;
	
	int overlay_draworder() const;
	interval capture_draworder() const;
};

#endif