/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: engine
File: text.h
---------------------------------------------------
*/

#ifndef _TEXT_
#define _TEXT_

#include "manageable.h"
#include "attachable.h"
#include "color.h"

namespace lunar {

//Forward declarations
class text_manager;
class font;

class text:public manageable,public attachable {

	friend text_manager;

public:

	enum ALIGNMENT { LEFT, CENTER, RIGHT };
	
	struct style {
	
		friend text;
		friend text_manager;
	
	private:
	
		color *_Color;
		
		void copy(const style &_Style);
		
		void color_value(const color &_Color);
	
	public:
	
		//Constructor
		style();
		
		//Copy constructor
		style(const style &_Style);
		
		//Destructor
		~style();
		
		style &operator=(const style &_Style);
		
		color *color_value() const;
	};
	
	struct element {
	
		friend text;
		friend text_manager;
	
	private:
	
		std::string _Data;
		unsigned int _Width;
		style _Style;
	
	public:
	
		//Constructor
		element();
		
		//Destructor
		~element();
		
		const std::string &data() const;
		unsigned int width() const;
		
		const style &element_style() const;
	};

protected:

	typedef std::list<element> _Elements;

public:

	struct line {
	
		friend text;
		friend text_manager;
	
	private:
	
		_Elements _Myelements;
		unsigned int _Width;
	
	public:
	
		typedef _Elements::const_iterator elements_iter; //Keep data safe
		
		//Constructor
		line();
		
		//Destructor
		~line();
		
		std::string data() const;
		unsigned int width() const;
		
		elements_iter elements_begin() const;
		elements_iter elements_end() const;
	};

protected:

	typedef std::list<line> _Lines;
	
	//Common data
	
	std::string _Text_data;
	ALIGNMENT _Alignment;
	unsigned int _Width;
	
	//Format data
	
	font *_Font;
	color _Color;
	unsigned int _Wrap_width;
	bool _Styles_enabled;
	
	int _Line_space;
	unsigned int _Max_lines;
	unsigned int _Skip_lines;
	_Lines _Mylines;
	
	//Protected constructor
	text(text_manager *_Owner,const std::string &_Text_data,font *_Font,
		const color &_Color=color::WHITE,bool _Visible=true);
	
	//Protected copy constructor
	text(const text &_Text);
	
	//Protected alternative copy constructor
	text(text_manager *_Owner,const text &_Text,font *_Font);
	
	//Protected virtual destructor
	virtual ~text();
	
	void copy(const text &_Text);
	
	virtual void status_changed();
	
	virtual void manageable_changed(manageable *_Manageable);
	virtual void manageable_statuschanged(manageable *_Manageable);
	virtual void manageable_removed(manageable *_Manageable);
	
	virtual void update_bounds();
	
	virtual void pre_draw();
	virtual void draw();
	virtual void post_draw();

public:

	typedef _Lines::const_iterator lines_iter; //Keep data safe
	
	text &operator=(const text &_Text);
	
	bool operator==(const text &_Text) const;
	bool operator!=(const text &_Text) const;
	
	virtual bool prepare();
	virtual geometry_stats statistics() const;
	
	//Common functions
	
	void text_data(const std::string &_Text_data);
	const std::string &text_data() const;
	
	void alignment(const ALIGNMENT &_Alignment);
	ALIGNMENT alignment() const;
	
	unsigned int width() const;
	unsigned int height() const;
	
	unsigned int line_height() const;
	unsigned int line_count(bool _Only_visible=false) const;
	
	//Format functions
	
	font *text_font() const;
	
	void color_value(const color &_Color);
	const color &color_value() const;
	
	void wrap_width(unsigned int _Wrap_width);
	unsigned int wrap_width() const;
	
	void styles_enabled(bool _Enabled);
	bool styles_enabled() const;
	
	//Line functions
	
	void line_space(int _Line_space);
	int line_space() const;
	
	void max_lines(unsigned int _Max_lines);
	unsigned int max_lines() const;
	
	void skip_lines(unsigned int _Skip_lines);
	unsigned int skip_lines() const;
	
	unsigned int line_capacity(unsigned int _Height) const;
	unsigned int line_distance() const;
	
	lines_iter lines_begin() const;
	lines_iter lines_end() const;
};

} //namespace lunar

#endif