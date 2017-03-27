/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_container.h
---------------------------------------------------
*/

#ifndef _FONT_CONTAINER_
#define _FONT_CONTAINER_

#include "file_container.h"

namespace lunar {

class font_data:public file_data {

public:

	enum ENCODING { ASCII, EXTENDED_ASCII };

protected:

	unsigned int _Face;
	unsigned int _Width;
	unsigned int _Height;
	unsigned int _Horizontal_res;
	unsigned int _Vertical_res;
	ENCODING _Encoding;
	
	void copy(const font_data &_Font_data);

public:

	//Constructor
	font_data(const std::string &_Name,const std::string &_Filename,
		unsigned int _Face=0,unsigned int _Width=12,
			unsigned int _Height=12,unsigned int _Horizontal_res=96,
				unsigned int _Vertical_res=96,const ENCODING &_Encoding=ASCII);
	
	//Copy constructor
	font_data(const font_data &_Font_data);
	
	//Virtual destructor
	virtual ~font_data();
	
	font_data &operator=(const font_data &_Font_data);
	
	void face(unsigned int _Face);
	unsigned int face() const;
	
	void size(unsigned int _Width,unsigned int _Height);
	unsigned int width() const;
	unsigned int height() const;
	
	void resolution(unsigned int _Horizontal_res,
		unsigned int _Vertical_res);
	unsigned int horizontal_res() const;
	unsigned int vertical_res() const;
	
	void encoding(const ENCODING &_Encoding);
	ENCODING encoding() const;
};

class font_container:public file_container {

	friend resource_manager;

private:

	static font_container *_Instance; //Grant resource_manager exclusive access

protected:
	
	void copy(const font_container &_Font_container);

public:

	//Constructor
	font_container();
	
	//Copy constructor
	font_container(const font_container &_Font_container);
	
	//Virtual destructor
	virtual ~font_container();
	
	font_container &operator=(const font_container &_Font_container);
	
	static font_container *instance();
	
	font_data *add_data(const font_data &_Font_data);
};

} //namespace lunar

#endif