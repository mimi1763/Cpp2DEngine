/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_manager.h
---------------------------------------------------
*/

#ifndef _FONT_MANAGER_
#define _FONT_MANAGER_

#include "ft2build.h"
#include FT_FREETYPE_H

#ifdef HAVE_UNISTD_H
#undef HAVE_UNISTD_H
#endif

#include "manager.h"
#include "font.h"

namespace lunar {

//Forward declarations
class core;

class font_manager:public manager {

	friend core;

private:

	static font_manager *_Instance; //Grant core exclusive access

protected:

	struct _Created_face {
	
	font_data *_Font_data;
	unsigned int _Listbase;
	unsigned int _Glyphs;
	unsigned int _Height;
	unsigned int *_Advance;
	unsigned int *_Gltexture;
	};
	
	typedef std::list<_Created_face> _Created_faces;
	
	_Created_faces _Mycreated_faces;
	font_container *_Font_container;
	
	FT_Library _Library;
	bool _Supported;
	
	void copy(const font_manager &_Font_manager);
	
	//FreeType specific
	bool init_freetype();
	
	bool create_face(font_data *_Font_data);
	static void init_font(font *_Font,const _Created_face &_Mycreated_face);

public:

	//Constructor
	font_manager();
	
	//Copy constructor
	font_manager(const font_manager &_Font_manager);
	
	//Virtual destructor
	virtual ~font_manager();
	
	font_manager &operator=(const font_manager &_Font_manager);
	
	static font_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	void font_source(font_container *_Font_container);
	font_container *font_source() const;
	
	//Font functions
	
	font *create_font(const std::string &_Name,bool _Secured=false);
	font *create_font(const font_data &_Font_data,bool _Secured=false);
	font *create_font(font_data *_Font_data,bool _Secured=false);
	font *create_font(const font &_Font,bool _Secured=false);
	
	void clear_cache();
};

} //namespace lunar

#endif