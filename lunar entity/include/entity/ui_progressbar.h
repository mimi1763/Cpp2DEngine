/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_progressbar.h
---------------------------------------------------
*/

#ifndef _UI_PROGRESSBAR_
#define _UI_PROGRESSBAR_

//Dependencies (entity)
#include "ui_element.h"

//Undefine macro min if defined
#ifdef min
#undef min
#endif

//Undefine macro max if defined
#ifdef max
#undef max
#endif

//Forward declarations (engine)
namespace lunar {
	class texturable;
} //namespace lunar

class ui_progressbar:public ui_element {

	friend ui_menu;

public:

	enum TYPE { NONE, HORIZONTAL, VERTICAL, ANIMATION };
	enum INTERPOLATION { OFF, FORWARD, BACKWARD, BIDIRECTIONAL };
	
	struct additional:public ui_entity::additional {
	
	static const std::string BAR;
	static const std::string BAR_PRECISE;
	};

protected:

	TYPE _Type;
	float _Min;
	float _Max;
	float _Progress;
	
	bool _Flip;
	bool _Locked;
	float _Snap_interval;
	
	INTERPOLATION _Interpolation;
	float _Interpolation_time;
	float _Interpolation_latency;
	std::string _Caption_format;	//Units:	0 or 0.00
									//Percent:	0% or 0.00%
	bool _Update_bar;
	bool _Interpolate_bar;
	float _Interpolated_percent;
	float _Interpolated_time;
	
	//Protected constructor
	ui_progressbar(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_progressbar(const ui_progressbar &_Progressbar);
	
	//Protected alternative copy constructor
	ui_progressbar(ui_menu *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_progressbar &_Progressbar);
	
	//Protected virtual destructor
	virtual ~ui_progressbar();
	
	void copy(const ui_progressbar &_Progressbar);
	
	texturable *bar_component(object *_Object) const;
	
	void update_bar(object *_Object,float _Percent);
	void update_bar();

public:

	ui_progressbar &operator=(const ui_progressbar &_Progressbar);
	
	virtual void update(float _Time);
	
	virtual bool key_released(unsigned int _Key);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual void mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual void mouse_scrolled(int _Delta,
		unsigned int _X,unsigned int _Y);
	
	virtual bool functional() const;
	
	void type(const TYPE &_Type);
	TYPE type() const;
	
	void range(float _Min,float _Max);
	float min() const;
	float max() const;
	
	void progress(float _Progress);
	float progress() const;
	
	void percent(float _Percent);
	float percent() const;
	
	void flip(bool _Flip);
	bool flip() const;
	
	void locked(bool _Locked);
	bool locked() const;
	
	void snap_interval(float _Snap_interval);
	float snap_interval() const;
	
	void interpolation(const INTERPOLATION &_Interpolation);
	INTERPOLATION interpolation() const;
	
	void interpolation_time(float _Interpolation_time);
	float interpolation_time() const;
	
	void interpolation_latency(float _Interpolation_latency);
	float interpolation_latency() const;
	
	void caption_format(const std::string &_Caption_format);
	const std::string &caption_format() const;
};

#endif