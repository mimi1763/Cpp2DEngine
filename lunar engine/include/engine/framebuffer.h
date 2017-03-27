/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: framebuffer.h
---------------------------------------------------
*/

#ifndef _FRAMEBUFFER_
#define _FRAMEBUFFER_

#include "manageable.h"

namespace lunar {

//Forward declarations
class framebuffer_manager;
class texture;

class framebuffer:public manageable,public manageable_listener {

	friend framebuffer_manager;

protected:

	unsigned int _Glframebuffer;
	unsigned int _Width;
	unsigned int _Height;
	
	unsigned int _Glrenderbuffer;
	texture *_Texture;
	
	//Protected constructor
	framebuffer(framebuffer_manager *_Owner,unsigned int _Width,
		unsigned int _Height,texture *_Texture);
	
	//Protected copy constructor
	framebuffer(const framebuffer &_Framebuffer);
	
	//Protected alternative copy constructor
	framebuffer(framebuffer_manager *_Owner,const framebuffer &_Framebuffer,
		texture *_Texture);
	
	//Protected virtual destructor
	virtual ~framebuffer();
	
	void copy(const framebuffer &_Framebuffer);
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);

public:

	framebuffer &operator=(const framebuffer &_Framebuffer);
	
	bool operator==(const framebuffer &_Framebuffer) const;
	bool operator!=(const framebuffer &_Framebuffer) const;
	
	unsigned int glframebuffer() const;
	
	unsigned int width() const;
	unsigned int height() const;
	
	texture *framebuffer_texture() const;
};

} //namespace lunar

#endif