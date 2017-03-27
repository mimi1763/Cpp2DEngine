/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_hint.h
---------------------------------------------------
*/

#ifndef _UI_HINT_
#define _UI_HINT_

//Dependencies (entity)
#include "ui_entity.h"

//Forward declarations (entity)
class ui_menu;

class ui_hint:public ui_entity {

	friend ui_manager;
	friend ui_menu;

public:

	enum TYPE { FIXED_SIZE, DYNAMIC_SIZE };

protected:

	enum PHASE { FADE_IN, HOLD, FADE_OUT };
	
	TYPE _Type;
	bool _Follow;
	
	float _Fadein_delay;
	float _Fadeout_delay;
	float _Fadein_time;
	float _Fadeout_time;
	float _Hold_time;
	
	vector2 _Border_size;
	vector2 _Offset_position;
	
	//Helper variables
	
	PHASE _Phase;
	float _Phase_time;
	bool _Update_position;
	
	//Protected constructor
	ui_hint(ui_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height);
	
	//Protected copy constructor
	ui_hint(const ui_hint &_Hint);
	
	//Protected alternative copy constructor
	ui_hint(ui_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,const ui_hint &_Hint);
	
	//Protected virtual destructor
	virtual ~ui_hint();
	
	void copy(const ui_hint &_Hint);
	
	void update_size();
	void update_position();
	
	//Caption
	
	virtual void update_caption();

public:

	ui_hint &operator=(const ui_hint &_Hint);
	
	virtual void update(float _Time);
	
	void show(const std::string &_Caption);
	void hide(bool _Immediately=false);
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void follow(bool _Follow);
	bool follow() const;
	
	void fadein_delay(float _Fadein_delay);
	float fadein_delay() const;
	
	void fadeout_delay(float _Fadeout_delay);
	float fadeout_delay() const;
	
	void fadein_time(float _Fadein_time);
	float fadein_time() const;
	
	void fadeout_time(float _Fadeout_time);
	float fadeout_time() const;
	
	void hold_time(float _Hold_time);
	float hold_time() const;
	
	void border_size(const vector2 &_Border_size);
	vector2 border_size() const;
	
	void offset_position(const vector2 &_Offset_position);
	vector2 offset_position() const;
};

#endif