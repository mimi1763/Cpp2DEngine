/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texturable.h
---------------------------------------------------
*/

#ifndef _TEXTURABLE_
#define _TEXTURABLE_

#include "rectangle.h"

namespace lunar {

//Forward declarations
class texture_manager;
class animation_manager;

class texturable:public rectangle {

	friend texture_manager;
	friend animation_manager;

public:

	enum ROTATION { ROTATE_NONE=0, ROTATE_180=180, ROTATE_90CW=-90, ROTATE_90CCW=90 };

	enum RATIO_MODE { RATIO_KEEP_WIDTH, RATIO_KEEP_HEIGHT,
					  RATIO_FIT_INSIDE, RATIO_FULLY_COVER };

	enum CUT_MODE {	CUT_TOP_LEFT,		CUT_TOP_RIGHT,
					CUT_BOTTOM_LEFT,	CUT_BOTTOM_RIGHT };

protected:

	//Common data
	
	unsigned int _Texture_width; //Actual texture width
	unsigned int _Texture_height; //Actual texture height
	unsigned int _Gltexture_width; //OpenGL texture width (power of 2)
	unsigned int _Gltexture_height; //OpenGL texture height (power of 2)
	bool _Tileable;
	
	bool _Auto_size;
	bool _Stretch_width;
	bool _Stretch_height;
	
	bool _Flip_horizontal;
	bool _Flip_vertical;
	ROTATION _Rotation;

	//Aspect ratio

	RATIO_MODE _Ratio_mode;
	bool _Keep_ratio;
	
	//Cut/crop data
	
	CUT_MODE _Cut_mode;
	float _Crop_width;
	float _Crop_height;
	vector2 _Crop_position;
	bool _Auto_crop;
	
	//Protected constructor
	explicit texturable(bool _Visible=true);
	
	//Protected copy constructor
	texturable(const texturable &_Texturable);
	
	//Protected virtual destructor
	virtual ~texturable();
	
	void copy(const texturable &_Texturable);
	
	virtual void pre_draw();
	virtual void post_draw();
	
	virtual void update_vertices();
	virtual void update_texcoords();

public:

	texturable &operator=(const texturable &_Texturable);
	
	virtual unsigned int gltexture() const=0;
	
	//Common functions 
	
	unsigned int texture_width() const;
	unsigned int texture_height() const;
	
	unsigned int gltexture_width() const;
	unsigned int gltexture_height() const;
	
	void auto_size(bool _Auto_size);
	bool auto_size() const;
	
	void stretch(bool _Stretch_width,bool _Stretch_height);
	bool stretch_width() const;
	bool stretch_height() const;
	
	void flip(bool _Flip_horizontal,bool _Flip_vertical);
	bool flip_horizontal() const;
	bool flip_vertical() const;
	
	void rotation(const ROTATION &_Rotation);
	ROTATION rotation() const;

	//Ratio functions

	void ratio_mode(const RATIO_MODE &_Ratio_mode);
	
	void keep_ratio(bool _Keep_ratio);
	bool keep_ratio() const;
	
	//Cut/crop functions
	
	void cut_mode(const CUT_MODE &_Cut_mode);
	CUT_MODE cut_mode() const;
	
	void crop_size(float _Crop_width,float _Crop_height);
	float crop_width() const;
	float crop_height() const;
	
	void crop_position(const vector2 &_Crop_position);
	const vector2 &crop_position() const;
	
	void auto_crop(bool _Auto_crop);
	bool auto_crop() const;
};

} //namespace lunar

#endif