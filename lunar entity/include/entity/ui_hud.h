/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_hud.h
---------------------------------------------------
*/

#ifndef _UI_HUD_
#define _UI_HUD_

//Dependencies (entity)
#include "ui_menu.h"

//Forward declarations (engine)
namespace lunar {

class framebuffer;
class capture;
} //namespace lunar

//Forward declarations (entity)
class ui_manager;

class ui_hud:public ui_menu {

	friend ui_manager;

public:

	struct region {
	
		friend ui_hud;
	
	public:
	
		enum MODE { HORIZONTAL, VERTICAL };
	
	private:
	
		typedef std::list<ui_groupbox*> _Elements;
		
		MODE _Mode;
		ui_groupbox *_Groupbox;
		
		bool _Update_elements;
		_Elements _Myelements;
		
		//Private constructor
		region(ui_groupbox *_Groupbox,const MODE &_Mode=HORIZONTAL);
		
		//Private copy constructor
		region(const region &_Region);
		
		//Private alternative copy constructor
		region(ui_groupbox *_Groupbox,const region &_Region);
		
		//Private destructor
		~region();
		
		void copy(const region &_Region);
		
		//Elements
		
		void update_elements();
		void swap_elements(ui_groupbox *_Element,ui_groupbox *_Element2);
	
	public:
	
		typedef _Elements::const_iterator elements_iter; //Keep data safe
		
		region &operator=(const region &_Region);
		
		void update(float _Time);
		
		void mode(const MODE &_Mode);
		MODE mode() const;
		
		void groupbox(ui_groupbox *_Groupbox);
		ui_groupbox *groupbox() const;
		
		//Elements
		
		void add_element(ui_groupbox *_Element);
		
		void remove_element(ui_groupbox *_Element);
		void clear_elements();
		
		elements_iter elements_begin() const;
		elements_iter elements_end() const;
		
		ui_groupbox *element_pointer(const std::string &_Name) const;
	};

protected:

	typedef std::list<region*> _Regions;
	
	_Regions _Myregions;
	
	//Drag/drop
	vector2 _Drag_origin;
	vector2 _Capture_origin;
	bool _Swapped;
	
	//Over
	framebuffer *_Over_framebuffer;
	render_node *_Over_node;
	
	//Drag
	ui_groupbox *_Drag_element;
	framebuffer *_Drag_framebuffer;
	render_node *_Drag_node;
	
	//Capture
	capture *_Capture;
	render_node *_Capture_node;
	render_node *_Pre_capturenode;
	render_node *_Post_capturenode;
	
	//Protected constructor
	ui_hud(ui_manager *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_hud(const ui_hud &_Hud);
	
	//Protected alternative copy constructor
	ui_hud(ui_manager *_Owner,const std::string &_Name,render_node *_Parent_node,
		const interval &_Draw_order,const ui_hud &_Hud);
	
	//Protected virtual destructor
	virtual ~ui_hud();
	
	void copy(const ui_hud &_Hud);
	
	virtual void client_changed(float _Width,float _Height);
	
	//Elements
	
	virtual void element_added(ui_element *_Element);
	virtual void element_removed(ui_element *_Element);
	
	virtual ui_element *intersect_element(const vector2 &_Position) const;
	
	region *element_owner(ui_groupbox *_Element) const;
	
	//Capture
	
	void prepare_capture(framebuffer *_Framebuffer,render_node *_Capture_node);
	
	void pre_capture(render_node *_Render_node);
	void post_capture(render_node *_Render_node);
	
	void revert_back(render_node *_Render_node);
	
	//Over/drag framebuffer
	
	void prepare_overframebuffer(render_node *_Capture_node);
	void prepare_dragframebuffer(render_node *_Capture_node);

public:

	typedef _Regions::const_iterator regions_iter; //Keep data safe
	
	ui_hud &operator=(const ui_hud &_Hud);
	
	virtual void update(float _Time);
	virtual void post_update(float _Time);
	
	virtual void mouse_pressed(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual bool mouse_released(unsigned int _Button,
		unsigned int _X,unsigned int _Y);
	virtual bool mouse_moved(unsigned int _X,
		unsigned int _Y);
	
	virtual void clean();
	
	//Regions
	
	region *create_region(ui_groupbox *_Groupbox,
		const region::MODE &_Mode=region::HORIZONTAL);
	region *create_region(ui_groupbox *_Groupbox,
		const region &_Region);
	
	void remove_region(region *&_Region);
	void clear_regions();
	
	regions_iter regions_begin() const;
	regions_iter regions_end() const;
	
	region *region_pointer(const std::string &_Name) const;
};

#endif