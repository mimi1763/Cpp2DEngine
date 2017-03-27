/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_textbox.h
---------------------------------------------------
*/

#ifndef _UI_TEXTBOX_
#define _UI_TEXTBOX_

//Dependencies (entity)
#include "ui_scrollable.h"

//Forward declarations (engine)
namespace lunar {
	class text;
} //namespace lunar

class ui_textbox:public ui_scrollable {

	friend ui_menu;

public:

	enum MODE { ALPHA_NUMERIC, ALPHA, NUMERIC };
	enum ALIGNMENT { LEFT, CENTER, RIGHT };
	enum OFF_BOUND { HIDE, TRIM };
	
	struct additional:public ui_entity::additional {
	
	static const std::string TEXT;
	static const std::string CURSOR;
	};

protected:

	MODE _Mode;
	std::string _Text_data;
	ALIGNMENT _Alignment;
	OFF_BOUND _Off_bound;
	vector2 _Text_offset;
	bool _Multiple_lines;
	unsigned int _Max_characters;
	char _Mask;
	
	unsigned int _Cursor_position;
	unsigned int _Reveal_distance;
	float _Repeat_rate;
	float _Repeat_delay;
	float _Blink_rate;
	float _Blink_hold;
	
	//Helper variables
	
	bool _Update_text;
	bool _Update_cursor;
	
	unsigned int _Left_bound;
	unsigned int _Right_bound;
	unsigned int _Repeat_key;
	bool _Blink_out;
	float _Repeat_time;
	float _Blink_time;
	
	//Protected constructor
	ui_textbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_textbox(const ui_textbox &_Textbox);
	
	//Protected alternative copy constructor
	ui_textbox(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_textbox &_Textbox);
	
	//Protected virtual destructor
	virtual ~ui_textbox();
	
	void copy(const ui_textbox &_Textbox);
	
	//Events
	
	virtual void focus();
	virtual void defocus();
	
	virtual void scroll(unsigned int _Units,bool _Up=true);
	
	virtual unsigned int scrollbar_count() const;
	virtual unsigned int scrollbar_viewcount() const;
	virtual unsigned int scrollbar_position() const;
	
	//Utility
	
	static bool clean_data(std::string &_Data,bool _Multiple_lines);
	static bool trim_data(std::string &_Data,const MODE &_Mode);
	static bool trim_data(std::string &_Data,unsigned int _Max_characters);
	static std::string mask_data(const std::string &_Data,char _Mask);
	
	void hide_text(text *_Text,unsigned int _Width);
	void trim_text(text *_Text,unsigned int _Width);
	
	//Helper
	
	std::string cursor_location(unsigned int _Cursor_position,
		unsigned int &_Line,unsigned int &_Column) const;
	unsigned int cursor_location(unsigned int _Line,
		unsigned int _Column) const;
	unsigned int cursor_location(const vector2 &_Position) const;
	
	text *text_component(object *_Object) const;
	
	void update_text();
	void update_cursor();

public:

	ui_textbox &operator=(const ui_textbox &_Textbox);
	
	virtual void update(float _Time);
	
	virtual bool key_pressed(unsigned int _Key);
	virtual bool key_released(unsigned int _Key);
	virtual bool char_pressed(char _Character);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual bool functional() const;
	
	void mode(const MODE &_Mode);
	MODE mode() const;
	
	void text_data(const std::string &_Text_data);
	void insert_data(const std::string &_Text_data,
		unsigned int _From_position=-1,
			unsigned int _To_position=-1);
	const std::string &text_data() const;
	
	void alignment(const ALIGNMENT &_Alignment);
	ALIGNMENT alignment() const;
	
	void off_bound(const OFF_BOUND &_Off_bound);
	OFF_BOUND off_bound() const;
	
	void text_offset(const vector2 &_Offset);
	const vector2 &text_offset() const;
	
	void multiple_lines(bool _Multiple_lines);
	bool multiple_lines() const;
	
	void max_characters(unsigned int _Max_characters);
	unsigned int max_characters() const;
	
	void mask(char _Mask);
	char mask() const;
	
	//Cursor
	
	void cursor_position(unsigned int _Cursor_position);
	unsigned int cursor_position() const;
	
	void reveal_distance(unsigned int _Reveal_distance);
	unsigned int reveal_distance() const;
	
	void repeat_rate(float _Repeat_rate);
	float repeat_rate() const;
	
	void repeat_delay(float _Repeat_delay);
	float repeat_delay() const;
	
	void blink_rate(float _Blink_rate);
	float blink_rate() const;
	
	void blink_hold(float _Blink_hold);
	float blink_hold() const;
	
	//Clipboard
	
	std::string cut_text(unsigned int _From_position,
		unsigned int _To_position=-1);
	std::string copy_text(unsigned int _From_position,
		unsigned int _To_position=-1) const;
	
	void paste_text(unsigned int _From_position,
		unsigned int _To_position=-1);
};

#endif