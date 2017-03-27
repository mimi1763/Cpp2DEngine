/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: font_table.h
---------------------------------------------------
*/

#ifndef _FONT_TABLE_
#define _FONT_TABLE_

#include "file_table.h"

namespace lunar {

//Forward declarations
class font_container;

class font_table:public file_table {

	friend resource_manager;

private:

	static font_table *_Instance; //Grant resource_manager exclusive access

protected:

	std::string _Face_column;
	std::string _Width_column;
	std::string _Height_column;
	std::string _Horizontalres_column;
	std::string _Verticalres_column;
	std::string _Encoding_column;
	
	font_container *_Font_container;
	
	void copy(const font_table &_Font_table);

public:

	//Constructor
	explicit font_table(font_container *_Font_container);
	
	//Copy constructor
	font_table(const font_table &_Font_table);
	
	//Virtual destructor
	virtual ~font_table();
	
	font_table &operator=(const font_table &_Font_table);
	
	static font_table *instance();
	
	virtual bool load(connector *_Connector);
	
	void columns(const std::string &_Face_column,const std::string &_Width_column,
		const std::string &_Height_column,const std::string &_Horizontalres_column,
			const std::string &_Verticalres_column,const std::string &_Encoding_column);
	
	const std::string &face_column() const;
	const std::string &width_column() const;
	const std::string &height_column() const;
	const std::string &horizontalres_column() const;
	const std::string &verticalres_column() const;
	const std::string &encoding_column() const;
};

} //namespace lunar

#endif