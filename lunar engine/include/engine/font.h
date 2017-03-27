/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font.h
---------------------------------------------------
*/

#ifndef _FONT_
#define _FONT_

#include "manageable.h"
#include "font_container.h"

namespace lunar {

//Forward declarations
class font_manager;

class font:public manageable {

	friend font_manager;

protected:

	unsigned int _Listbase;
	unsigned int _Glyphs;
	unsigned int _Height;
	unsigned int *_Advance;
	
	//Protected constructor
	font(font_manager *_Owner,font_data *_Font_data);
	
	//Protected copy constructor
	font(const font &_Font);
	
	//Protected alternative copy constructor
	font(font_manager *_Owner,const font &_Font);
	
	//Protected virtual destructor
	virtual ~font();
	
	void copy(const font &_Font);

public:

	font &operator=(const font &_Font);
	
	void data(font_data *_Font_data);
	font_data *data() const;
	
	unsigned int listbase() const;
	unsigned int glyphs() const;
	unsigned int height() const;
	unsigned int *advance() const;
};

} //namespace lunar

#endif