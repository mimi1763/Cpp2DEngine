/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: text_manager.h
---------------------------------------------------
*/

#ifndef _TEXT_MANAGER_
#define _TEXT_MANAGER_

#include "manager.h"
#include "text.h"

//We could forward declare font_manager and include font_container.h
//But instead we include font_manager.h which includes font_container.h,
//and the user get font_manager when including text_manager.h
#include "font_manager.h"

namespace lunar {

//Forward declarations
class core;

class text_manager:public manager {

	friend core;

private:

	static text_manager *_Instance; //Grant core exclusive access

protected:

	font_manager *_Font_manager;
	
	void copy(const text_manager &_Text_manager);
	
	//Text specific
	void init_text(text *_Text) const;
	
	static void parse_styles(std::string &_Data,text::_Elements &_Myelements);
	
	static void word_wrap(std::string &_Data,font *_Font,
		unsigned int _Wrap_width);
	
	static void adjust_elements(const std::string &_Data,
		text::_Elements &_Myelements);
	static void break_elements(const text::_Elements &_Myelements,
		font *_Font,text::_Lines &_Mylines);

public:

	//Constructor
	text_manager();
	
	//Alternative constructor
	explicit text_manager(font_manager *_Font_manager);
	
	//Copy constructor
	text_manager(const text_manager &_Text_manager);
	
	//Virtual destructor
	virtual ~text_manager();
	
	text_manager &operator=(const text_manager &_Text_manager);
	
	static text_manager *instance();
	
	virtual void update(manageable *_Manageable);
	virtual void update();
	
	static unsigned int width(unsigned char _Elem,font *_Font);
	static unsigned int width(const std::string &_Data,font *_Font);
	
	static std::string trim(const std::string &_Data,font *_Font,
		unsigned int _Max_width);
	
	//Text functions
	
	text *create_text(const std::string &_Data,const std::string &_Font_name,
		const color &_Color=color::WHITE,bool _Visible=true,
			bool _Secured=false);
	
	text *create_text(const std::string &_Data,const font_data &_Font_data,
		const color &_Color=color::WHITE,bool _Visible=true,
			bool _Secured=false);
	
	text *create_text(const std::string &_Data,font_data *_Font_data,
		const color &_Color=color::WHITE,bool _Visible=true,
			bool _Secured=false);
	
	text *create_text(const text &_Text,bool _Secured=false);
};

} //namespace lunar

#endif