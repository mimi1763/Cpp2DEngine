/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture_manager.h
---------------------------------------------------
*/

#ifndef _TEXTURE_MANAGER_
#define _TEXTURE_MANAGER_

#include "manager.h"
#include "texture.h"

namespace lunar {

//Forward declarations
class core;

class texture_manager:public manager {

	friend core;

private:

	static texture_manager *_Instance; //Grant core exclusive access

protected:

	struct _Created_gltexture {
	
	texture_data *_Texture_data;
	unsigned int _Gltexture;
	unsigned int _Texture_width;
	unsigned int _Texture_height;
	unsigned int _Gltexture_width; //Power of 2
	unsigned int _Gltexture_height; //Power of 2
	bool _Tileable;
	};
	
	typedef std::list<_Created_gltexture> _Created_gltextures;
	
	_Created_gltextures _Mycreated_gltextures;
	texture_container *_Texture_container;
	
	void copy(const texture_manager &_Texture_manager);
	
	//FreeImage and GL texture specific
	bool create_gltexture(texture_data *_Texture_data,
		unsigned int _Texture_width,unsigned int _Texture_height);
	
	static void init_texture(texture *_Texture,
		const _Created_gltexture &_Mycreated_gltexture);

public:

	//Constructor
	texture_manager();
	
	//Copy constructor
	texture_manager(const texture_manager &_Texture_manager);
	
	//Virtual destructor
	virtual ~texture_manager();
	
	texture_manager &operator=(const texture_manager &_Texture_manager);
	
	static texture_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void texture_source(texture_container *_Texture_container);
	texture_container *texture_source() const;
	
	//Texture functions
	
	texture *create_texture(const std::string &_Name,bool _Visible=true,
		bool _Secured=false);
	
	texture *create_texture(const texture_data &_Texture_data,bool _Visible=true,
		bool _Secured=false);
	
	texture *create_texture(texture_data *_Texture_data,bool _Visible=true,
		bool _Secured=false);
	
	texture *create_texture(unsigned int _Width,unsigned int _Height,
		bool _Visible=true,bool _Secured=false);
	
	texture *create_texture(const texture &_Texture,bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif