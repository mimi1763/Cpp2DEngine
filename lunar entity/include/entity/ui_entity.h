/*
---------------------------------------------------
This source file is part of Lunar Radiation
For the latest info, see http://www.lunargamer.com/

All rights reserved - Lunar Radiation Team
Copyright (c) 2005-2011

Author: Jan Ivar Goli
Area: entity
File: ui_entity.h
---------------------------------------------------
*/

#ifndef _UI_ENTITY_
#define _UI_ENTITY_

//Dependencies (engine)
#include "aabb.h"

//Dependencies (entity)
#include "entity.h"

//Forward declarations (engine)
namespace lunar {
	class rectangle;
} //namespace lunar

//Forward declarations (entity)
class ui_manager;

class ui_entity:public entity {

public:

	enum LAYOUT { STATIC, DYNAMIC_X_AXIS, DYNAMIC_Y_AXIS, DYNAMIC_XY_AXIS };
	enum LAYOUT_UNIT { PIXEL, PERCENT };
	
	enum GENERATE_MODE { FORCE_OFF, MODIFY_TEXCOORDS, ROTATE_NODE };
	
	enum CAPTION_ORIGIN { OUTSIDE_TOP_LEFT,		TOP_LEFT,		TOP_CENTER,		TOP_RIGHT,		OUTSIDE_TOP_RIGHT,
						  OUTSIDE_LEFT,			LEFT,			CENTER,			RIGHT,			OUTSIDE_RIGHT,
						  OUTSIDE_BOTTOM_LEFT,	BOTTOM_LEFT,	BOTTOM_CENTER,	BOTTOM_RIGHT,	OUTSIDE_BOTTOM_RIGHT };
	
	struct part {
	
	static const std::string CENTER;
	static const std::string BORDER;
	
	static const std::string TOP_LEFT;
	static const std::string TOP;
	static const std::string TOP_RIGHT;
	static const std::string LEFT;
	static const std::string RIGHT;
	static const std::string BOTTOM_LEFT;
	static const std::string BOTTOM;
	static const std::string BOTTOM_RIGHT;
	};
	
	struct additional {
		static const std::string CAPTION;
	};

protected:

	enum PART_STATUS { OKAY, NEED_UPDATE, GENERATE };
	
	rectangle *_Area;
	
	//Layout
	
	LAYOUT _Positioning;
	LAYOUT _Sizing;
	LAYOUT_UNIT _Positioning_unit[2];
	LAYOUT_UNIT _Sizing_unit[2];
	aabb _Positioning_area;
	
	//Parts
	
	vector2 _Auto_size;
	GENERATE_MODE _Generate_mode;
	
	//Caption
	
	std::string _Caption;
	float _Caption_width;
	float _Caption_height;
	vector2 _Caption_offset;
	CAPTION_ORIGIN _Caption_origin;
	
	//Helper
	
	vector2 _Static_position;
	float _Static_width;
	float _Static_height;
	bool _Update_positioning;
	bool _Update_sizing;
	
	PART_STATUS _Part_status;
	bool _Update_caption;
	
	//Protected constructor
	ui_entity(ui_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,float _Width,float _Height,
			bool _Visible=true);
	
	//Protected copy constructor
	ui_entity(const ui_entity &_Ui_entity);
	
	//Protected alternative copy constructor
	ui_entity(ui_manager *_Owner,render_node *_Parent_node,
		const interval &_Draw_order,const ui_entity &_Ui_entity);
	
	//Protected virtual destructor
	virtual ~ui_entity();
	
	void copy(const ui_entity &_Ui_entity);
	
	virtual void object_created(object *_Object);
	
	virtual void client_changed(float _Width,float _Height);
	
	void update_positioning(float _Width,float _Height);
	void update_sizing(float _Width,float _Height);
	
	//Parts
	
	static rectangle *rectangle_pointer(object *_Object);
	
	static void modify_part(object *_Object,bool _Flip_horizontal,
		bool _Flip_vertical,int _Rotation=0);
	static void reposition_part(object *_Object,const vector2 &_Position);
	
	static void resize_part(object *_Object,float _Width,
		float _Height);
	void resize_parts(object *_Center,object *_Border,object *_Top_left,
		object *_Top_right,object *_Bottom_left,object *_Bottom_right,
			object *_Top,object *_Bottom,object *_Left,object *_Right,
				float _Width,float _Height);
	
	object *generate_part(const std::string &_Name,object *_Object,
		object *_Parent_object,const vector2 &_From,const vector2 &_To);
	virtual void update_parts(const std::string &_Name="");
	
	//Caption
	
	virtual void update_caption();

public:

	ui_entity &operator=(const ui_entity &_Ui_entity);
	
	virtual void update(float _Time);
	
	//Area
	
	void size(float _Width,float _Height);
	float width() const;
	float height() const;
	
	rectangle *area();
	
	//Layout
	
	void positioning(const LAYOUT &_Positioning);
	LAYOUT positioning() const;
	
	void sizing(const LAYOUT &_Sizing);
	LAYOUT sizing() const;
	
	void positioning_unit(const LAYOUT_UNIT &_Unit);
	void positioning_unit(const LAYOUT_UNIT &_X,const LAYOUT_UNIT &_Y);
	LAYOUT_UNIT positioning_x() const;
	LAYOUT_UNIT positioning_y() const;
	
	void sizing_unit(const LAYOUT_UNIT &_Unit);
	void sizing_unit(const LAYOUT_UNIT &_X,const LAYOUT_UNIT &_Y);
	LAYOUT_UNIT sizing_x() const;
	LAYOUT_UNIT sizing_y() const;
	
	void positioning_area(const aabb &_Positioning_area);
	const aabb &positioning_area() const;
		
	//Parts
	
	void auto_size(bool _Auto_size);
	bool auto_size() const;
	
	void generate_mode(const GENERATE_MODE &_Generate_mode);
	GENERATE_MODE generate_mode() const;
	
	//Caption
	
	void caption(const std::string &_Caption);
	const std::string &caption() const;
	
	void caption_size(float _Width,float _Height);
	float caption_width() const;
	float caption_height() const;
	
	void caption_offset(const vector2 &_Caption_offset);
	const vector2 &caption_offset() const;
	
	void caption_origin(const CAPTION_ORIGIN &_Caption_origin);
	CAPTION_ORIGIN caption_origin() const;
};

#endif