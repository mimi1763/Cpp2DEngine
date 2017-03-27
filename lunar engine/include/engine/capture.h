/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: capture.h
---------------------------------------------------
*/

#ifndef _CAPTURE_
#define _CAPTURE_

#include "rectangle.h"

namespace lunar {

//Forward declarations
class render_manager;
class framebuffer;
class texture;

class capture:public manageable_listener {

	friend render_manager;

public:

	enum TECHNIQUE { FBO, COPY_TEX };

protected:

	static capture *_Active;
	
	//User variables
	
	float _Width;
	float _Height;
	vector2 _Position; //BOTTOM_LEFT origin (for COPY_TEX technique)
	
	framebuffer *_Destination_framebuffer;
	texture *_Destination_texture;
	TECHNIQUE _Technique;
	
	//Helper variables
	
	float _Adjusted_width;
	float _Adjusted_height;
	vector2 _Adjusted_position;
	bool _Adjust_viewport;
	
	void copy(const capture &_Capture);
	
	virtual void unsubscribed(interface_base *_Interface);
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);
	
	void attach_framebuffer();
	void detach_framebuffer();
	
	void attach_texture();
	void detach_texture();
	
	void start(framebuffer *_Framebuffer);
	void stop(framebuffer *_Framebuffer,bool _Crop=true);
	
	void start(texture *_Texture);
	void stop(texture *_Texture,bool _Crop=true);
	
	void snapshot(texture *_Texture,bool _Crop=true);

public:

	//Constructor
	capture();
	
	//Copy constructor
	capture(const capture &_Capture);
	
	//Alternative copy constructor
	capture(const rectangle &_Rectangle);
	
	//Virtual destructor
	virtual ~capture();
	
	capture &operator=(const capture &_Capture);
	capture &operator=(const rectangle &_Rectangle);
	
	void start();
	void stop(bool _Crop=true);
	void cancel();
	
	void snapshot(bool _Crop=true);
	
	void size(float _Width,float _Height);
	float width() const;
	float height() const;
	
	void position(const vector2 &_Position,const rectangle::
		ORIGIN &_Origin=rectangle::BOTTOM_LEFT);
	const vector2 &position() const;
	
	void destination(framebuffer *_Destination);
	framebuffer *destination_framebuffer() const;
	
	void destination(texture *_Destination);
	texture *destination_texture() const;
	
	void technique(const TECHNIQUE &_Technique);
	TECHNIQUE technique() const;
};

} //namespace lunar

#endif