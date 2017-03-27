/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: texture.h
---------------------------------------------------
*/

#ifndef _TEXTURE_
#define _TEXTURE_

#include "manageable.h"
#include "texturable.h"
#include "texture_container.h"

namespace lunar {

//Forward declarations
class texture_manager;

class texture:public manageable,public texturable {

	friend texture_manager;

public:

	enum TYPE { IMAGE, CUSTOM };

protected:

	unsigned int _Gltexture;
	TYPE _Type;
	
	//Protected constructor
	texture(texture_manager *_Owner,texture_data *_Texture_data,
		bool _Visible=true);
	
	//Protected alternative constructor
	texture(texture_manager *_Owner,unsigned int _Width,
		unsigned int _Height,bool _Visible=true);
	
	//Protected copy constructor
	texture(const texture &_Texture);
	
	//Protected alternative copy constructor
	texture(texture_manager *_Owner,const texture &_Texture);
	
	//Protected virtual destructor
	virtual ~texture();
	
	void copy(const texture &_Texture);
	
	virtual void pre_draw();
	virtual void post_draw();

public:

	texture &operator=(const texture &_Texture);
	
	virtual bool prepare();
	
	void data(texture_data *_Texture_data);
	texture_data *data() const;
	
	virtual unsigned int gltexture() const;
	
	TYPE type() const;
};

} //namespace lunar

#endif